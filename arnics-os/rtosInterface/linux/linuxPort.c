#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#if PLATFORM_LINUX
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "rtosInterface/rtosInterfacePublic.h"
#include "dePartment/centerEvent/centerEvent.h"
#include "dePartment/centerMedia/centerMedia.h"
#include "dePartment/centerAdministrative/centerAdministrative.h"

typedef struct
{
    uint32_t             length;
    uint32_t          item_size;
    uint8_t*             buffer;
    uint32_t               head;
    uint32_t               tail;
    uint32_t              count;
    pthread_mutex_t        lock;
    pthread_cond_t    not_empty;
    pthread_cond_t     not_full;
} linux_queue_t;

static inline uint64_t linux_now_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000ull + (uint64_t)tv.tv_usec / 1000ull;
}

static int64_t linux_delay_to_timeout_ms(uint32_t delay)
{
    if (delay == 0u)
    {
        return 0;
    }
    if (delay == (uint32_t)BLOCK_DELAY)
    {
        return -1;
    }
    return (int64_t)delay;
}

static bool linux_queue_init(linux_queue_t* q, uint32_t length, uint32_t item_size)
{
    if (q == NULL || length == 0u || item_size == 0u)
    {
        return false;
    }

    const uint64_t alloc_size_64 = (uint64_t)length * (uint64_t)item_size;
    if (alloc_size_64 == 0ull || alloc_size_64 > (uint64_t)SIZE_MAX)
    {
        return false;
    }

    q->buffer = (uint8_t*)malloc((size_t)alloc_size_64);
    if (q->buffer == NULL)
    {
        return false;
    }

    q->length = length;
    q->item_size = item_size;
    q->head = 0u;
    q->tail = 0u;
    q->count = 0u;
    pthread_mutex_init(&q->lock, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);
    return true;
}

static bool linux_queue_send(linux_queue_t* q, const void* item, uint32_t delay)
{
    if (q == NULL || q->buffer == NULL || q->length == 0u || q->item_size == 0u || item == NULL)
    {
        return false;
    }

    const int64_t timeout_ms = linux_delay_to_timeout_ms(delay);
    const uint64_t start_ms = (timeout_ms < 0) ? 0ull : linux_now_ms();

    pthread_mutex_lock(&q->lock);
    while (q->count >= q->length)
    {
        if (timeout_ms == 0)
        {
            pthread_mutex_unlock(&q->lock);
            return false;
        }

        if (timeout_ms < 0)
        {
            pthread_cond_wait(&q->not_full, &q->lock);
        }
        else
        {
            const uint64_t elapsed = linux_now_ms() - start_ms;
            if (elapsed >= (uint64_t)timeout_ms)
            {
                pthread_mutex_unlock(&q->lock);
                return false;
            }
            struct timespec ts;
            ts.tv_sec = (time_t)((timeout_ms - (int64_t)elapsed) / 1000);
            ts.tv_nsec = (long)(((timeout_ms - (int64_t)elapsed) % 1000) * 1000000);
            const int rc = pthread_cond_timedwait(&q->not_full, &q->lock, &ts);
            if (rc == ETIMEDOUT)
            {
                pthread_mutex_unlock(&q->lock);
                return false;
            }
        }
    }

    const size_t offset = (size_t)q->head * (size_t)q->item_size;
    memcpy(q->buffer + offset, item, (size_t)q->item_size);
    q->head = (q->head + 1u) % q->length;
    q->count++;
    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->lock);
    return true;
}

static bool linux_queue_receive(linux_queue_t* q, void* out, uint32_t delay)
{
    if (q == NULL || q->buffer == NULL || q->length == 0u || q->item_size == 0u || out == NULL)
    {
        return false;
    }

    const int64_t timeout_ms = linux_delay_to_timeout_ms(delay);
    const uint64_t start_ms = (timeout_ms < 0) ? 0ull : linux_now_ms();

    pthread_mutex_lock(&q->lock);
    while (q->count == 0u)
    {
        if (timeout_ms == 0)
        {
            pthread_mutex_unlock(&q->lock);
            return false;
        }

        if (timeout_ms < 0)
        {
            pthread_cond_wait(&q->not_empty, &q->lock);
        }
        else
        {
            const uint64_t elapsed = linux_now_ms() - start_ms;
            if (elapsed >= (uint64_t)timeout_ms)
            {
                pthread_mutex_unlock(&q->lock);
                return false;
            }
            struct timespec ts;
            ts.tv_sec = (time_t)((timeout_ms - (int64_t)elapsed) / 1000);
            ts.tv_nsec = (long)(((timeout_ms - (int64_t)elapsed) % 1000) * 1000000);
            const int rc = pthread_cond_timedwait(&q->not_empty, &q->lock, &ts);
            if (rc == ETIMEDOUT)
            {
                pthread_mutex_unlock(&q->lock);
                return false;
            }
        }
    }

    const size_t offset = (size_t)q->tail * (size_t)q->item_size;
    memcpy(out, q->buffer + offset, (size_t)q->item_size);
    q->tail = (q->tail + 1u) % q->length;
    q->count--;
    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->lock);
    return true;
}

static bool linux_queue_peek(linux_queue_t* q, void* out)
{
    if (q == NULL || q->buffer == NULL || q->length == 0u || q->item_size == 0u || out == NULL)
    {
        return false;
    }

    pthread_mutex_lock(&q->lock);
    if (q->count == 0u)
    {
        pthread_mutex_unlock(&q->lock);
        return false;
    }
    const size_t offset = (size_t)q->tail * (size_t)q->item_size;
    memcpy(out, q->buffer + offset, (size_t)q->item_size);
    pthread_mutex_unlock(&q->lock);
    return true;
}

static uint32_t linux_queue_messages_waiting(linux_queue_t* q)
{
    if (q == NULL || q->buffer == NULL || q->length == 0u || q->item_size == 0u)
    {
        return 0u;
    }
    pthread_mutex_lock(&q->lock);
    const uint32_t count = q->count;
    pthread_mutex_unlock(&q->lock);
    return count;
}

static uint32_t linux_queue_spaces_available(linux_queue_t* q)
{
    if (q == NULL || q->buffer == NULL || q->length == 0u || q->item_size == 0u)
    {
        return 0u;
    }
    pthread_mutex_lock(&q->lock);
    const uint32_t spaces = (q->count >= q->length) ? 0u : (q->length - q->count);
    pthread_mutex_unlock(&q->lock);
    return spaces;
}

static pthread_mutex_t* linux_mutex_create(void)
{
    pthread_mutex_t* mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    if (mutex != NULL)
    {
        pthread_mutex_init(mutex, NULL);
    }
    return mutex;
}

static bool linux_mutex_take(pthread_mutex_t* mutex, time_t wait_time)
{
    if (mutex == NULL)
    {
        return false;
    }
    const int64_t timeout_ms = (wait_time == 0) ? 0 : ((wait_time == BLOCK_DELAY) ? -1 : (int64_t)wait_time);
    if (timeout_ms < 0)
    {
        return (pthread_mutex_lock(mutex) == 0);
    }
    else if (timeout_ms == 0)
    {
        return (pthread_mutex_trylock(mutex) == 0);
    }
    else
    {
        struct timespec ts;
        ts.tv_sec = (time_t)(timeout_ms / 1000);
        ts.tv_nsec = (long)((timeout_ms % 1000) * 1000000);
        return (pthread_mutex_timedlock(mutex, &ts) == 0);
    }
}

static void linux_mutex_give(pthread_mutex_t* mutex)
{
    if (mutex == NULL)
    {
        return;
    }
    pthread_mutex_unlock(mutex);
}

static pthread_mutex_t g_suspend_lock = PTHREAD_MUTEX_INITIALIZER;

static void linux_suspend_all(void)
{
    pthread_mutex_lock(&g_suspend_lock);
}

static void linux_resume_all(void)
{
    pthread_mutex_unlock(&g_suspend_lock);
}

static int linux_priority_to_native(rtosPriority_e priority)
{
    (void)priority;
    return 0;
}

typedef void (*linux_task_fn_t)(void const* argument);

typedef struct
{
    linux_task_fn_t fn;
    void* arg;
} linux_task_start_t;

static void* linux_task_entry(void* param)
{
    linux_task_start_t* start = (linux_task_start_t*)param;
    if (start != NULL)
    {
        linux_task_fn_t fn = start->fn;
        void* arg = start->arg;
        free(start);
        if (fn != NULL)
        {
            fn(arg);
        }
    }
    pthread_exit(NULL);
    return NULL;
}

static    linux_queue_t    g_eventos_req_queue;
static    linux_queue_t    g_eventos_rsp_queue;
static    linux_queue_t    g_media_req_queue;
static    linux_queue_t    g_media_rsp_queue;
static    linux_queue_t    g_ad_req_queue;
static    linux_queue_t    g_ad_rsp_queue;

static pthread_mutex_t*    g_eventos_rsp_queue_mutex;
static pthread_mutex_t*    g_eventos_id_mutex;
static pthread_mutex_t*    g_media_rsp_queue_mutex;
static pthread_mutex_t*    g_ad_rsp_queue_mutex;

void cpuInfo(void)
{
    printf("CPU/OS: Linux, tick=%llu ms\r\n", (unsigned long long)linux_now_ms());
}

void rtosTaskSuspendAll(void)
{
    linux_suspend_all();
}

void rtosTaskResumeAll(void)
{
    linux_resume_all();
}

void rtosThreadDelayUntil(uint32_t time)
{
    static uint64_t next_wake_ms = 0ull;
    const uint64_t now = linux_now_ms();
    if (next_wake_ms == 0ull)
    {
        next_wake_ms = now;
    }
    next_wake_ms += (uint64_t)time;
    if (next_wake_ms > now)
    {
        const uint64_t delta = next_wake_ms - now;
        usleep(((delta > 2000ull) ? 2000ull * 1000ull : delta * 1000ull));
    }
}

void rtosThreadDelay(uint32_t ms)
{
    usleep(ms * 1000);
}

void rtosTaskCreate(char* name,
                    rtosPriority_e priority,
                    void* func,
                    uint32_t stackSize,
                    void* arg)
{
    (void)name;
    if (func == NULL || stackSize == 0u)
    {
        return;
    }

    linux_task_start_t* start = (linux_task_start_t*)malloc(sizeof(linux_task_start_t));
    if (start == NULL)
    {
        return;
    }
    start->fn = (linux_task_fn_t)func;
    start->arg = arg;

    pthread_t thread;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, (size_t)stackSize);

    const int rc = pthread_create(&thread, &attr, linux_task_entry, start);
    pthread_attr_destroy(&attr);
    if (rc != 0)
    {
        free(start);
        return;
    }

    (void)linux_priority_to_native;
    pthread_detach(thread);
}

void rtosTaskSelfDelete(void)
{
    pthread_exit(NULL);
}

bool rtosAdGetMsg(void* msg, uint32_t delay)
{
    return linux_queue_receive(&g_ad_req_queue, msg, delay);
}

bool rtosAdSendMsg(void* msg, uint32_t delay)
{
    return linux_queue_send(&g_ad_rsp_queue, msg, delay);
}

bool rtosTakeMsgFromAd(void* msg, uint32_t delay)
{
    return linux_queue_receive(&g_ad_rsp_queue, msg, delay);
}

bool rtosDeliverMsgToAd(void* msg, uint32_t delay)
{
    return linux_queue_send(&g_ad_req_queue, msg, delay);
}

bool TakeAdMsgQueueMutex(time_t waitTime)
{
    return linux_mutex_take(g_ad_rsp_queue_mutex, waitTime);
}

void ReleaseAdMsgQueueMutex(void)
{
    linux_mutex_give(g_ad_rsp_queue_mutex);
}

bool CheckAdQueueSpacesAvailable(void)
{
    return linux_queue_spaces_available(&g_ad_req_queue) != 0u;
}

uint32_t CheckAdRspMesgNum(void)
{
    return linux_queue_messages_waiting(&g_ad_rsp_queue);
}

uint32_t CheckAdReqMesgNum(void)
{
    return linux_queue_messages_waiting(&g_ad_req_queue);
}

bool PeekAdRspMesg(void* receivedMsg)
{
    return linux_queue_peek(&g_ad_rsp_queue, receivedMsg);
}

bool CheckAdQueueSpacesAvailable_(void)
{
    return linux_queue_spaces_available(&g_ad_req_queue) != 0u;
}

bool rtosEventosGetMsg(void* msg, uint32_t delay)
{
    return linux_queue_receive(&g_eventos_req_queue, msg, delay);
}

bool rtosEventosSendMsg(void* msg, uint32_t delay)
{
    return linux_queue_send(&g_eventos_rsp_queue, msg, delay);
}

bool rtosTakeMsgFromEventos(void* msg, uint32_t delay)
{
    return linux_queue_receive(&g_eventos_rsp_queue, msg, delay);
}

bool rtosDeliverMsgToEventos(void* msg, uint32_t delay)
{
    return linux_queue_send(&g_eventos_req_queue, msg, delay);
}

bool TakeEventosMsgQueueMutex(time_t waitTime)
{
    return linux_mutex_take(g_eventos_rsp_queue_mutex, waitTime);
}

void ReleaseEventosMsgQueueMutex(void)
{
    linux_mutex_give(g_eventos_rsp_queue_mutex);
}

bool TakeEventosMutex(time_t waitTime)
{
    return linux_mutex_take(g_eventos_id_mutex, waitTime);
}

void ReleaseEventosMutex(void)
{
    linux_mutex_give(g_eventos_id_mutex);
}

uint32_t CheckEventRspMesgNum(void)
{
    return linux_queue_messages_waiting(&g_eventos_rsp_queue);
}

bool PeekEventRspMesg(void* receivedMsg)
{
    return linux_queue_peek(&g_eventos_rsp_queue, receivedMsg);
}

bool CheckEventQueueSpacesAvailable(void)
{
    return linux_queue_spaces_available(&g_eventos_req_queue) != 0u;
}

bool rtosMediaGetMsg(void* msg, uint32_t delay)
{
    return linux_queue_receive(&g_media_req_queue, msg, delay);
}

bool rtosMediaSendMsg(void* msg, uint32_t delay)
{
    return linux_queue_send(&g_media_rsp_queue, msg, delay);
}

bool rtosTakeMsgFromMedia(void* msg, uint32_t delay)
{
    return linux_queue_receive(&g_media_rsp_queue, msg, delay);
}

bool rtosDeliverMsgToMedia(void* msg, uint32_t delay)
{
    return linux_queue_send(&g_media_req_queue, msg, delay);
}

bool TakeMediaMutex(time_t waitTime)
{
    return linux_mutex_take(g_media_rsp_queue_mutex, waitTime);
}

void ReleaseMediaMsgQueueMutex(void)
{
    linux_mutex_give(g_media_rsp_queue_mutex);
}

uint32_t CheckMediaRspMesgNum(void)
{
    return linux_queue_messages_waiting(&g_media_rsp_queue);
}

uint32_t CheckMediaReqMesgNum(void)
{
    return linux_queue_messages_waiting(&g_media_req_queue);
}

bool PeekMediaRspMesg(void* receivedMsg)
{
    return linux_queue_peek(&g_media_rsp_queue, receivedMsg);
}

bool CheckMediaQueueSpacesAvailable(void)
{
    return linux_queue_spaces_available(&g_media_req_queue) != 0u;
}

void os_task_create(void)
{
    (void)linux_queue_init(&g_eventos_req_queue, 3u, (uint32_t)sizeof(message_t));
    (void)linux_queue_init(&g_eventos_rsp_queue, 3u, (uint32_t)sizeof(message_t));

    (void)linux_queue_init(&g_media_req_queue,   3u, (uint32_t)sizeof(mediaMessage_t));
    (void)linux_queue_init(&g_media_rsp_queue,   3u, (uint32_t)sizeof(mediaMessage_t));

    (void)linux_queue_init(&g_ad_req_queue,      1u, (uint32_t)sizeof(adMessage_t));
    (void)linux_queue_init(&g_ad_rsp_queue,      1u, (uint32_t)sizeof(adMessage_t));

    g_eventos_rsp_queue_mutex = linux_mutex_create();
    g_eventos_id_mutex        = linux_mutex_create();
    g_media_rsp_queue_mutex   = linux_mutex_create();
    g_ad_rsp_queue_mutex      = linux_mutex_create();

    rtosTaskCreate("ConsleTask", rtosPriorityNormal,  (void*)StartConsleTask, 1024u, NULL);
    rtosTaskCreate("EventTask",   rtosPriorityNormal, (void*)StartEventTask,   640u, NULL);
    rtosTaskCreate("MediaTask",   rtosPriorityNormal, (void*)StartMediaTask,   640u, NULL);
    rtosTaskCreate("MainTask",    rtosPriorityNormal, (void*)StartMaintTask,   640u, NULL);
    rtosTaskCreate("GuardTask",      rtosPriorityLow, (void*)StartGuardTask,   512u, NULL);
    rtosTaskCreate("AdTask",    rtosPriorityRealtime, (void*)StartAdTask,      128u, NULL);

    printf(" Linux os initialized successfully.\r\n");
}

void linux_os_init(void)
{
    rtosTaskCreate("initTask",   rtosPriorityRealtime,        (void*)StartInitTask,       500u,  NULL);
}

#endif
