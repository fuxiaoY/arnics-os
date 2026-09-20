/**
 * @file    queue_linux.c
 * @brief   队列抽象层的 Linux/POSIX(PLATFORM_LINUX) 后端。
 *
 * @details
 * 用定长环形缓冲 + pthread_mutex_t + pthread_cond_t 实现线程安全 FIFO，
 * 供主机侧开发/单元测试使用，行为与 win 后端、FreeRTOS 后端对齐：
 *   - send/recv 支持超时(毫秒，pthread_cond_timedwait)与永久等待
 *     (#BLOCK_DELAY)；0 表示不等待；
 *   - peek 仅窥探队首而不移除；count 返回当前元素数。
 * 队列控制块与数据缓冲通过 malloc 分配。
 *
 * @copyright (c) 2026 arnics-os. Licensed under the project LICENSE.
 */

#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#if PLATFORM_LINUX
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "rtosInterface/queue/queue_port.h"

/**
 * @brief Linux 环形队列控制块。
 */
typedef struct
{
    uint32_t length;            // 队列深度(元素个数)
    uint32_t item_size;         // 单个元素大小(字节)
    uint8_t* buffer;            // 数据缓冲(按值存放全部元素)
    uint32_t head;              // 队尾写入位置
    uint32_t tail;              // 队首读取位置
    uint32_t count;             // 当前元素数
    pthread_mutex_t lock;       // 保护本结构的互斥量
    pthread_cond_t not_empty;   // 队列非空，唤醒接收方
    pthread_cond_t not_full;    // 队列非满，唤醒发送方
} linux_queue_t;

/**
 * @brief 获取当前单调时间戳。
 * @return 毫秒时间戳
 */
static inline uint64_t linux_now_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000ull + (uint64_t)tv.tv_usec / 1000ull;
}

/**
 * @brief 把统一超时换算为相对毫秒。
 * @param[in] delay 统一超时(0 不等待，#BLOCK_DELAY 永久等待)
 * @return 0 不等待；-1 永久等待；正数为毫秒
 */
static int64_t linux_delay_to_timeout_ms(uint32_t delay)
{
    if (delay == 0u) return 0;
    if (delay == (uint32_t)BLOCK_DELAY) return -1;
    return (int64_t)delay;
}

/**
 * @brief 创建 Linux 队列。
 * @param[in] item_size 元素大小(字节)
 * @param[in] len       队列深度(元素个数)
 * @return 队列句柄(不透明返回)；参数非法或内存不足时为 NULL
 */
static void* linux_queue_create(uint32_t item_size, uint32_t len)
{
    const uint64_t alloc_size_64 = (uint64_t)len * (uint64_t)item_size;
    if (alloc_size_64 == 0ull || alloc_size_64 > (uint64_t)SIZE_MAX)
        return NULL;

    linux_queue_t* q = (linux_queue_t*)malloc(sizeof(linux_queue_t));
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
    pthread_mutex_init(&q->lock, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);

    return (void*)q;
}

/**
 * @brief 入队一个元素。
 * @param[in] q     队列句柄
 * @param[in] item  元素指针
 * @param[in] delay 超时(毫秒)，0 不等待，#BLOCK_DELAY 永久等待
 * @return true 成功；false 参数非法、队列满或超时
 */
static bool linux_queue_send(void* q, const void* item, uint32_t delay)
{
    linux_queue_t* queue = (linux_queue_t*)q;
    if (!queue || !queue->buffer || queue->length == 0u || queue->item_size == 0u || !item)
        return false;

    const int64_t timeout_ms = linux_delay_to_timeout_ms(delay);
    const uint64_t start_ms = (timeout_ms < 0) ? 0ull : linux_now_ms();

    pthread_mutex_lock(&queue->lock);
    while (queue->count >= queue->length)
    {
        if (timeout_ms == 0)
        {
            pthread_mutex_unlock(&queue->lock);
            return false;
        }
        if (timeout_ms < 0)
        {
            pthread_cond_wait(&queue->not_full, &queue->lock);
        }
        else
        {
            const uint64_t elapsed = linux_now_ms() - start_ms;
            if (elapsed >= (uint64_t)timeout_ms)
            {
                pthread_mutex_unlock(&queue->lock);
                return false;
            }
            struct timespec ts;
            ts.tv_sec = (time_t)((timeout_ms - (int64_t)elapsed) / 1000);
            ts.tv_nsec = (long)(((timeout_ms - (int64_t)elapsed) % 1000) * 1000000);
            const int rc = pthread_cond_timedwait(&queue->not_full, &queue->lock, &ts);
            if (rc == ETIMEDOUT)
            {
                pthread_mutex_unlock(&queue->lock);
                return false;
            }
        }
    }
    const size_t offset = (size_t)queue->head * (size_t)queue->item_size;
    memcpy(queue->buffer + offset, item, (size_t)queue->item_size);
    queue->head = (queue->head + 1u) % queue->length;
    queue->count++;
    pthread_cond_signal(&queue->not_empty);
    pthread_mutex_unlock(&queue->lock);
    return true;
}

/**
 * @brief 出队一个元素。
 * @param[in] q     队列句柄
 * @param[out] out  接收缓冲区
 * @param[in] delay 超时(毫秒)，0 不等待，#BLOCK_DELAY 永久等待
 * @return true 成功；false 参数非法、队列空或超时
 */
static bool linux_queue_recv(void* q, void* out, uint32_t delay)
{
    linux_queue_t* queue = (linux_queue_t*)q;
    if (!queue || !queue->buffer || queue->length == 0u || queue->item_size == 0u || !out)
        return false;

    const int64_t timeout_ms = linux_delay_to_timeout_ms(delay);
    const uint64_t start_ms = (timeout_ms < 0) ? 0ull : linux_now_ms();

    pthread_mutex_lock(&queue->lock);
    while (queue->count == 0u)
    {
        if (timeout_ms == 0)
        {
            pthread_mutex_unlock(&queue->lock);
            return false;
        }
        if (timeout_ms < 0)
        {
            pthread_cond_wait(&queue->not_empty, &queue->lock);
        }
        else
        {
            const uint64_t elapsed = linux_now_ms() - start_ms;
            if (elapsed >= (uint64_t)timeout_ms)
            {
                pthread_mutex_unlock(&queue->lock);
                return false;
            }
            struct timespec ts;
            ts.tv_sec = (time_t)((timeout_ms - (int64_t)elapsed) / 1000);
            ts.tv_nsec = (long)(((timeout_ms - (int64_t)elapsed) % 1000) * 1000000);
            const int rc = pthread_cond_timedwait(&queue->not_empty, &queue->lock, &ts);
            if (rc == ETIMEDOUT)
            {
                pthread_mutex_unlock(&queue->lock);
                return false;
            }
        }
    }
    const size_t offset = (size_t)queue->tail * (size_t)queue->item_size;
    memcpy(out, queue->buffer + offset, (size_t)queue->item_size);
    queue->tail = (queue->tail + 1u) % queue->length;
    queue->count--;
    pthread_cond_signal(&queue->not_full);
    pthread_mutex_unlock(&queue->lock);
    return true;
}

/**
 * @brief 窥探队首但不移除。
 * @param[in] q   队列句柄
 * @param[out] out 接收缓冲区
 * @return true 队列非空；false 参数非法或队列为空
 */
static bool linux_queue_peek(void* q, void* out)
{
    linux_queue_t* queue = (linux_queue_t*)q;
    if (!queue || !queue->buffer || queue->length == 0u || queue->item_size == 0u || !out)
        return false;

    pthread_mutex_lock(&queue->lock);
    if (queue->count == 0u)
    {
        pthread_mutex_unlock(&queue->lock);
        return false;
    }
    const size_t offset = (size_t)queue->tail * (size_t)queue->item_size;
    memcpy(out, queue->buffer + offset, (size_t)queue->item_size);
    pthread_mutex_unlock(&queue->lock);
    return true;
}

/**
 * @brief 查询当前消息数。
 * @param[in] q 队列句柄
 * @return 已入队元素个数；参数非法时为 0
 */
static uint32_t linux_queue_count(void* q)
{
    linux_queue_t* queue = (linux_queue_t*)q;
    if (!queue || !queue->buffer || queue->length == 0u || queue->item_size == 0u)
        return 0u;
    pthread_mutex_lock(&queue->lock);
    const uint32_t count = queue->count;
    pthread_mutex_unlock(&queue->lock);
    return count;
}

/**
 * @brief Linux 后端操作表，queue_port.c 经 CURRENT_QUEUE_OPS 引用。
 */
const queue_ops_t queue_ops_linux =
{
    .create = linux_queue_create,
    .send = linux_queue_send,
    .recv = linux_queue_recv,
    .peek = linux_queue_peek,
    .count = linux_queue_count,
};

#endif
