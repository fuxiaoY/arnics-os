
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

typedef struct {
    uint32_t length;
    uint32_t item_size;
    uint8_t* buffer;
    uint32_t head;
    uint32_t tail;
    uint32_t count;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} linux_queue_t;

static inline uint64_t linux_now_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000ull + (uint64_t)tv.tv_usec / 1000ull;
}

static int64_t linux_delay_to_timeout_ms(uint32_t delay)
{
    if (delay == 0u) return 0;
    if (delay == (uint32_t)BLOCK_DELAY) return -1;
    return (int64_t)delay;
}

static void* linux_queue_create(uint32_t item_size, uint32_t len)
{
    const uint64_t alloc_size_64 = (uint64_t)len * (uint64_t)item_size;
    if (alloc_size_64 == 0ull || alloc_size_64 > (uint64_t)SIZE_MAX)
        return NULL;

    linux_queue_t* q = (linux_queue_t*)malloc(sizeof(linux_queue_t));
    if (!q) return NULL;

    q->buffer = (uint8_t*)malloc((size_t)alloc_size_64);
    if (!q->buffer) { free(q); return NULL; }

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

const queue_ops_t queue_ops_linux = {
    .create = linux_queue_create,
    .send = linux_queue_send,
    .recv = linux_queue_recv,
    .peek = linux_queue_peek,
    .count = linux_queue_count,
};

#endif
