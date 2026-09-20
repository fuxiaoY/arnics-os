/**
 * @file    queue_win.c
 * @brief   队列抽象层的 Windows(PLATFORM_WIN) 后端。
 *
 * @details
 * 用定长环形缓冲 + CRITICAL_SECTION + CONDITION_VARIABLE 实现一个
 * 行为贴近 FreeRTOS 队列的线程安全 FIFO，供主机侧开发/单元测试使用：
 *   - send/recv 支持超时(毫秒)与永久等待(#BLOCK_DELAY -> INFINITE)；
 *   - 满/空时在条件变量上阻塞，并由对端唤醒；
 *   - peek 仅窥探队首而不移除；count 返回当前元素数。
 * 队列控制块与数据缓冲通过 malloc 分配。
 *
 * @copyright (c) 2026 arnics-os. Licensed under the project LICENSE.
 */

#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#include "rtosInterface/rtosInterfacePublic.h"
#if PLATFORM_WIN
#include <windows.h>
#include <process.h>
#include "rtosInterface/queue/queue_port.h"

/**
 * @brief Windows 环形队列控制块。
 */
typedef struct
{
    uint32_t length;               // 队列深度(元素个数)
    uint32_t item_size;            // 单个元素大小(字节)
    uint8_t* buffer;               // 数据缓冲(按值存放全部元素)
    uint32_t head;                 // 队尾写入位置
    uint32_t tail;                 // 队首读取位置
    uint32_t count;                // 当前元素数
    CRITICAL_SECTION lock;         // 保护本结构的临界区
    CONDITION_VARIABLE not_empty;  // 队列非空，唤醒接收方
    CONDITION_VARIABLE not_full;   // 队列非满，唤醒发送方
} win_queue_t;

/**
 * @brief 把统一超时换算为 Windows 等待超时。
 * @param[in] delay 统一超时(0 不等待，#BLOCK_DELAY 永久等待)
 * @return 0 不等待；INFINITE 永久等待；正数为毫秒
 */
static DWORD win_delay_to_timeout_ms_u32(uint32_t delay)
{
    if (delay == 0u) return 0u;
    if (delay == (uint32_t)BLOCK_DELAY) return INFINITE;
    return (DWORD)delay;
}

/**
 * @brief 获取当前单调时间戳。
 * @return 毫秒时间戳
 */
static uint64_t win_now_ms(void)
{
    return (uint64_t)GetTickCount64();
}

/**
 * @brief 创建 Windows 队列。
 * @param[in] item_size 元素大小(字节)
 * @param[in] len       队列深度(元素个数)
 * @return 队列句柄(不透明返回)；参数非法或内存不足时为 NULL
 */
static void* win_queue_create(uint32_t item_size, uint32_t len)
{
    const uint64_t alloc_size_64 = (uint64_t)len * (uint64_t)item_size;
    if (alloc_size_64 == 0ull || alloc_size_64 > (uint64_t)SIZE_MAX)
        return NULL;

    win_queue_t* q = (win_queue_t*)malloc(sizeof(win_queue_t));
    if (!q) return NULL;

    q->buffer = (uint8_t*)malloc((size_t)alloc_size_64);
    if (!q->buffer)
    {
        free(q);
        return NULL;
    }

    q->length = len;
    q->item_size = item_size;
    q->head = 0u;
    q->tail = 0u;
    q->count = 0u;
    InitializeCriticalSection(&q->lock);
    InitializeConditionVariable(&q->not_empty);
    InitializeConditionVariable(&q->not_full);

    return (void*)q;
}

/**
 * @brief 入队一个元素。
 * @param[in] q     队列句柄
 * @param[in] item  元素指针
 * @param[in] delay 超时(毫秒)，0 不等待，#BLOCK_DELAY 永久等待
 * @return true 成功；false 参数非法、队列满或超时
 */
static bool win_queue_send(void* q, const void* item, uint32_t delay)
{
    win_queue_t* queue = (win_queue_t*)q;
    if (!queue || !queue->buffer || queue->length == 0u || queue->item_size == 0u || !item)
        return false;

    const DWORD timeout_ms = win_delay_to_timeout_ms_u32(delay);
    const uint64_t start_ms = (timeout_ms == INFINITE) ? 0ull : win_now_ms();

    EnterCriticalSection(&queue->lock);
    while (queue->count >= queue->length)
    {
        if (timeout_ms == 0u)
        {
            LeaveCriticalSection(&queue->lock);
            return false;
        }
        DWORD remaining_ms = timeout_ms;
        if (timeout_ms != INFINITE)
        {
            const uint64_t elapsed = win_now_ms() - start_ms;
            if (elapsed >= (uint64_t)timeout_ms)
            {
                LeaveCriticalSection(&queue->lock);
                return false;
            }
            remaining_ms = (DWORD)((uint64_t)timeout_ms - elapsed);
        }
        const BOOL ok = SleepConditionVariableCS(&queue->not_full, &queue->lock, remaining_ms);
        if (!ok && GetLastError() == ERROR_TIMEOUT)
        {
            LeaveCriticalSection(&queue->lock);
            return false;
        }
    }
    const size_t offset = (size_t)queue->head * (size_t)queue->item_size;
    memcpy(queue->buffer + offset, item, (size_t)queue->item_size);
    queue->head = (queue->head + 1u) % queue->length;
    queue->count++;
    WakeConditionVariable(&queue->not_empty);
    LeaveCriticalSection(&queue->lock);
    return true;
}

/**
 * @brief 出队一个元素。
 * @param[in] q     队列句柄
 * @param[out] out  接收缓冲区
 * @param[in] delay 超时(毫秒)，0 不等待，#BLOCK_DELAY 永久等待
 * @return true 成功；false 参数非法、队列空或超时
 */
static bool win_queue_recv(void* q, void* out, uint32_t delay)
{
    win_queue_t* queue = (win_queue_t*)q;
    if (!queue || !queue->buffer || queue->length == 0u || queue->item_size == 0u || !out)
        return false;

    const DWORD timeout_ms = win_delay_to_timeout_ms_u32(delay);
    const uint64_t start_ms = (timeout_ms == INFINITE) ? 0ull : win_now_ms();

    EnterCriticalSection(&queue->lock);
    while (queue->count == 0u)
    {
        if (timeout_ms == 0u)
        {
            LeaveCriticalSection(&queue->lock);
            return false;
        }
        DWORD remaining_ms = timeout_ms;
        if (timeout_ms != INFINITE)
        {
            const uint64_t elapsed = win_now_ms() - start_ms;
            if (elapsed >= (uint64_t)timeout_ms)
            {
                LeaveCriticalSection(&queue->lock);
                return false;
            }
            remaining_ms = (DWORD)((uint64_t)timeout_ms - elapsed);
        }
        const BOOL ok = SleepConditionVariableCS(&queue->not_empty, &queue->lock, remaining_ms);
        if (!ok && GetLastError() == ERROR_TIMEOUT)
        {
            LeaveCriticalSection(&queue->lock);
            return false;
        }
    }
    const size_t offset = (size_t)queue->tail * (size_t)queue->item_size;
    memcpy(out, queue->buffer + offset, (size_t)queue->item_size);
    queue->tail = (queue->tail + 1u) % queue->length;
    queue->count--;
    WakeConditionVariable(&queue->not_full);
    LeaveCriticalSection(&queue->lock);
    return true;
}

/**
 * @brief 窥探队首但不移除。
 * @param[in] q   队列句柄
 * @param[out] out 接收缓冲区
 * @return true 队列非空；false 参数非法或队列为空
 */
static bool win_queue_peek(void* q, void* out)
{
    win_queue_t* queue = (win_queue_t*)q;
    if (!queue || !queue->buffer || queue->length == 0u || queue->item_size == 0u || !out)
        return false;

    EnterCriticalSection(&queue->lock);
    if (queue->count == 0u)
    {
        LeaveCriticalSection(&queue->lock);
        return false;
    }
    const size_t offset = (size_t)queue->tail * (size_t)queue->item_size;
    memcpy(out, queue->buffer + offset, (size_t)queue->item_size);
    LeaveCriticalSection(&queue->lock);
    return true;
}

/**
 * @brief 查询当前消息数。
 * @param[in] q 队列句柄
 * @return 已入队元素个数；参数非法时为 0
 */
static uint32_t win_queue_count(void* q)
{
    win_queue_t* queue = (win_queue_t*)q;
    if (!queue || !queue->buffer || queue->length == 0u || queue->item_size == 0u)
        return 0u;
    EnterCriticalSection(&queue->lock);
    const uint32_t count = queue->count;
    LeaveCriticalSection(&queue->lock);
    return count;
}

/**
 * @brief Windows 后端操作表，queue_port.c 经 CURRENT_QUEUE_OPS 引用。
 */
const queue_ops_t queue_ops_win =
{
    .create = win_queue_create,
    .send = win_queue_send,
    .recv = win_queue_recv,
    .peek = win_queue_peek,
    .count = win_queue_count,
};

#endif
