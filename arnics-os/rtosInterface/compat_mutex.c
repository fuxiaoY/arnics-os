
#include "rtosInterface/rtosInterface.h"
#include "Inc/projDefine.h"
#include "Inc/typedef.h"

#if PLATFORM_WIN
#include <windows.h>
static HANDLE g_ad_rsp_mutex = NULL;
static HANDLE g_eventos_rsp_mutex = NULL;
static HANDLE g_eventos_id_mutex = NULL;
static HANDLE g_media_rsp_mutex = NULL;

static HANDLE win_mutex_create(void)
{
    return CreateMutexA(NULL, FALSE, NULL);
}

static bool win_mutex_take(HANDLE mutex, time_t wait_time)
{
    if (mutex == NULL) return false;
    const DWORD timeout_ms = (wait_time == BLOCK_DELAY) ? INFINITE : (DWORD)wait_time;
    const DWORD result = WaitForSingleObject(mutex, timeout_ms);
    return result == WAIT_OBJECT_0;
}

static void win_mutex_give(HANDLE mutex)
{
    if (mutex == NULL) return;
    ReleaseMutex(mutex);
}

void compat_init_mutexes(void)
{
    g_ad_rsp_mutex = win_mutex_create();
    g_eventos_rsp_mutex = win_mutex_create();
    g_eventos_id_mutex = win_mutex_create();
    g_media_rsp_mutex = win_mutex_create();
}

bool TakeAdMsgQueueMutex(time_t waitTime)
{
    return win_mutex_take(g_ad_rsp_mutex, waitTime);
}

void ReleaseAdMsgQueueMutex(void)
{
    win_mutex_give(g_ad_rsp_mutex);
}

bool TakeEventosMsgQueueMutex(time_t waitTime)
{
    return win_mutex_take(g_eventos_rsp_mutex, waitTime);
}

void ReleaseEventosMsgQueueMutex(void)
{
    win_mutex_give(g_eventos_rsp_mutex);
}

bool TakeEventosMutex(time_t waitTime)
{
    return win_mutex_take(g_eventos_id_mutex, waitTime);
}

void ReleaseEventosMutex(void)
{
    win_mutex_give(g_eventos_id_mutex);
}

bool TakeMediaMutex(time_t waitTime)
{
    return win_mutex_take(g_media_rsp_mutex, waitTime);
}

void ReleaseMediaMsgQueueMutex(void)
{
    win_mutex_give(g_media_rsp_mutex);
}

#elif PLATFORM_LINUX
#include <pthread.h>
#include <stdlib.h>
static pthread_mutex_t *g_ad_rsp_mutex = NULL;
static pthread_mutex_t *g_eventos_rsp_mutex = NULL;
static pthread_mutex_t *g_eventos_id_mutex = NULL;
static pthread_mutex_t *g_media_rsp_mutex = NULL;

static pthread_mutex_t *linux_mutex_create(void)
{
    pthread_mutex_t *mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    if (mutex) pthread_mutex_init(mutex, NULL);
    return mutex;
}

static bool linux_mutex_take(pthread_mutex_t *mutex, time_t wait_time)
{
    if (mutex == NULL) return false;
    if (wait_time == BLOCK_DELAY)
        return (pthread_mutex_lock(mutex) == 0);
    else if (wait_time == 0)
        return (pthread_mutex_trylock(mutex) == 0);
    else
    {
        struct timespec ts;
        ts.tv_sec = (time_t)(wait_time / 1000);
        ts.tv_nsec = (long)((wait_time % 1000) * 1000000);
        return (pthread_mutex_timedlock(mutex, &ts) == 0);
    }
}

static void linux_mutex_give(pthread_mutex_t *mutex)
{
    if (mutex == NULL) return;
    pthread_mutex_unlock(mutex);
}

void compat_init_mutexes(void)
{
    g_ad_rsp_mutex = linux_mutex_create();
    g_eventos_rsp_mutex = linux_mutex_create();
    g_eventos_id_mutex = linux_mutex_create();
    g_media_rsp_mutex = linux_mutex_create();
}

bool TakeAdMsgQueueMutex(time_t waitTime)
{
    return linux_mutex_take(g_ad_rsp_mutex, waitTime);
}

void ReleaseAdMsgQueueMutex(void)
{
    linux_mutex_give(g_ad_rsp_mutex);
}

bool TakeEventosMsgQueueMutex(time_t waitTime)
{
    return linux_mutex_take(g_eventos_rsp_mutex, waitTime);
}

void ReleaseEventosMsgQueueMutex(void)
{
    linux_mutex_give(g_eventos_rsp_mutex);
}

bool TakeEventosMutex(time_t waitTime)
{
    return linux_mutex_take(g_eventos_id_mutex, waitTime);
}

void ReleaseEventosMutex(void)
{
    linux_mutex_give(g_eventos_id_mutex);
}

bool TakeMediaMutex(time_t waitTime)
{
    return linux_mutex_take(g_media_rsp_mutex, waitTime);
}

void ReleaseMediaMsgQueueMutex(void)
{
    linux_mutex_give(g_media_rsp_mutex);
}

#elif PLATFORM_MCU
#include "FreeRTOS.h"
#include "semphr.h"
static SemaphoreHandle_t g_ad_rsp_mutex = NULL;
static SemaphoreHandle_t g_eventos_rsp_mutex = NULL;
static SemaphoreHandle_t g_eventos_id_mutex = NULL;
static SemaphoreHandle_t g_media_rsp_mutex = NULL;

void compat_init_mutexes(void)
{
    g_ad_rsp_mutex = xSemaphoreCreateMutex();
    g_eventos_rsp_mutex = xSemaphoreCreateMutex();
    g_eventos_id_mutex = xSemaphoreCreateMutex();
    g_media_rsp_mutex = xSemaphoreCreateMutex();
}

bool TakeAdMsgQueueMutex(time_t waitTime)
{
    return (xSemaphoreTake(g_ad_rsp_mutex, waitTime) == pdTRUE);
}

void ReleaseAdMsgQueueMutex(void)
{
    xSemaphoreGive(g_ad_rsp_mutex);
}

bool TakeEventosMsgQueueMutex(time_t waitTime)
{
    return (xSemaphoreTake(g_eventos_rsp_mutex, waitTime) == pdTRUE);
}

void ReleaseEventosMsgQueueMutex(void)
{
    xSemaphoreGive(g_eventos_rsp_mutex);
}

bool TakeEventosMutex(time_t waitTime)
{
    return (xSemaphoreTake(g_eventos_id_mutex, waitTime) == pdTRUE);
}

void ReleaseEventosMutex(void)
{
    xSemaphoreGive(g_eventos_id_mutex);
}

bool TakeMediaMutex(time_t waitTime)
{
    return (xSemaphoreTake(g_media_rsp_mutex, waitTime) == pdTRUE);
}

void ReleaseMediaMsgQueueMutex(void)
{
    xSemaphoreGive(g_media_rsp_mutex);
}
#endif
