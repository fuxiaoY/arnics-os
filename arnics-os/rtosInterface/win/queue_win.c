
#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#include "rtosInterface/rtosInterfacePublic.h"
#if PLATFORM_WIN
#include <windows.h>
#include <process.h>
#include "rtosInterface/queue/queue_port.h"

typedef struct {
    uint32_t length;
    uint32_t item_size;
    uint8_t* buffer;
    uint32_t head;
    uint32_t tail;
    uint32_t count;
    CRITICAL_SECTION lock;
    CONDITION_VARIABLE not_empty;
    CONDITION_VARIABLE not_full;
} win_queue_t;

static DWORD win_delay_to_timeout_ms_u32(uint32_t delay)
{
    if (delay == 0u) return 0u;
    if (delay == (uint32_t)BLOCK_DELAY) return INFINITE;
    return (DWORD)delay;
}

static uint64_t win_now_ms(void)
{
    return (uint64_t)GetTickCount64();
}

static void* win_queue_create(uint32_t item_size, uint32_t len)
{
    const uint64_t alloc_size_64 = (uint64_t)len * (uint64_t)item_size;
    if (alloc_size_64 == 0ull || alloc_size_64 > (uint64_t)SIZE_MAX)
        return NULL;

    win_queue_t* q = (win_queue_t*)malloc(sizeof(win_queue_t));
    if (!q) return NULL;

    q->buffer = (uint8_t*)malloc((size_t)alloc_size_64);
    if (!q->buffer) { free(q); return NULL; }

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

const queue_ops_t queue_ops_win = {
    .create = win_queue_create,
    .send = win_queue_send,
    .recv = win_queue_recv,
    .peek = win_queue_peek,
    .count = win_queue_count,
};

#endif
