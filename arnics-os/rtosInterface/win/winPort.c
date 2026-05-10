
#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#if PLATFORM_WIN
#include <windows.h>
#include <process.h>

#include "rtosInterface/rtosInterfacePublic.h"
#include "dePartment/centerEvent/centerEvent.h"
#include "dePartment/centerMedia/centerMedia.h"



typedef struct
{
    uint32_t                length;
    uint32_t             item_size;
    uint8_t*                buffer;
    uint32_t                  head;
    uint32_t                  tail;
    uint32_t                 count;
    CRITICAL_SECTION          lock;
    CONDITION_VARIABLE   not_empty;
    CONDITION_VARIABLE    not_full;
} win_queue_t;

static DWORD win_delay_to_timeout_ms_u32(uint32_t delay)
{
    if (delay == 0u)
    {
        return 0u;
    }
    if (delay == (uint32_t)BLOCK_DELAY)
    {
        return INFINITE;
    }
    return (DWORD)delay;
}

static DWORD win_delay_to_timeout_ms_time(time_t delay)
{
    if ((uint64_t)delay == 0ull)
    {
        return 0u;
    }
    if ((uint64_t)delay >= (uint64_t)BLOCK_DELAY)
    {
        return INFINITE;
    }
    if ((uint64_t)delay > (uint64_t)INFINITE)
    {
        return INFINITE;
    }
    return (DWORD)delay;
}

static uint64_t win_now_ms(void)
{
    return (uint64_t)GetTickCount64();
}

static bool win_queue_init(win_queue_t* q, uint32_t length, uint32_t item_size)
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
    InitializeCriticalSection(&q->lock);
    InitializeConditionVariable(&q->not_empty);
    InitializeConditionVariable(&q->not_full);
    return true;
}

static bool win_queue_send(win_queue_t* q, const void* item, uint32_t delay)
{
    if (q == NULL || q->buffer == NULL || q->length == 0u || q->item_size == 0u || item == NULL)
    {
        return false;
    }

    const DWORD timeout_ms = win_delay_to_timeout_ms_u32(delay);
    const uint64_t start_ms = (timeout_ms == INFINITE) ? 0ull : win_now_ms();

    EnterCriticalSection(&q->lock);
    while (q->count >= q->length)
    {
        if (timeout_ms == 0u)
        {
            LeaveCriticalSection(&q->lock);
            return false;
        }

        DWORD remaining_ms = timeout_ms;
        if (timeout_ms != INFINITE)
        {
            const uint64_t elapsed = win_now_ms() - start_ms;
            if (elapsed >= (uint64_t)timeout_ms)
            {
                LeaveCriticalSection(&q->lock);
                return false;
            }
            remaining_ms = (DWORD)((uint64_t)timeout_ms - elapsed);
        }

        const BOOL ok = SleepConditionVariableCS(&q->not_full, &q->lock, remaining_ms);
        if (!ok && GetLastError() == ERROR_TIMEOUT)
        {
            LeaveCriticalSection(&q->lock);
            return false;
        }
    }

    const size_t offset = (size_t)q->head * (size_t)q->item_size;
    memcpy(q->buffer + offset, item, (size_t)q->item_size);
    q->head = (q->head + 1u) % q->length;
    q->count++;
    WakeConditionVariable(&q->not_empty);
    LeaveCriticalSection(&q->lock);
    return true;
}

static bool win_queue_receive(win_queue_t* q, void* out, uint32_t delay)
{
    if (q == NULL || q->buffer == NULL || q->length == 0u || q->item_size == 0u || out == NULL)
    {
        return false;
    }

    const DWORD timeout_ms = win_delay_to_timeout_ms_u32(delay);
    const uint64_t start_ms = (timeout_ms == INFINITE) ? 0ull : win_now_ms();

    EnterCriticalSection(&q->lock);
    while (q->count == 0u)
    {
        if (timeout_ms == 0u)
        {
            LeaveCriticalSection(&q->lock);
            return false;
        }

        DWORD remaining_ms = timeout_ms;
        if (timeout_ms != INFINITE)
        {
            const uint64_t elapsed = win_now_ms() - start_ms;
            if (elapsed >= (uint64_t)timeout_ms)
            {
                LeaveCriticalSection(&q->lock);
                return false;
            }
            remaining_ms = (DWORD)((uint64_t)timeout_ms - elapsed);
        }

        const BOOL ok = SleepConditionVariableCS(&q->not_empty, &q->lock, remaining_ms);
        if (!ok && GetLastError() == ERROR_TIMEOUT)
        {
            LeaveCriticalSection(&q->lock);
            return false;
        }
    }

    const size_t offset = (size_t)q->tail * (size_t)q->item_size;
    memcpy(out, q->buffer + offset, (size_t)q->item_size);
    q->tail = (q->tail + 1u) % q->length;
    q->count--;
    WakeConditionVariable(&q->not_full);
    LeaveCriticalSection(&q->lock);
    return true;
}

static bool win_queue_peek(win_queue_t* q, void* out)
{
    if (q == NULL || q->buffer == NULL || q->length == 0u || q->item_size == 0u || out == NULL)
    {
        return false;
    }

    EnterCriticalSection(&q->lock);
    if (q->count == 0u)
    {
        LeaveCriticalSection(&q->lock);
        return false;
    }
    const size_t offset = (size_t)q->tail * (size_t)q->item_size;
    memcpy(out, q->buffer + offset, (size_t)q->item_size);
    LeaveCriticalSection(&q->lock);
    return true;
}

static uint32_t win_queue_messages_waiting(win_queue_t* q)
{
    if (q == NULL || q->buffer == NULL || q->length == 0u || q->item_size == 0u)
    {
        return 0u;
    }
    EnterCriticalSection(&q->lock);
    const uint32_t count = q->count;
    LeaveCriticalSection(&q->lock);
    return count;
}

static uint32_t win_queue_spaces_available(win_queue_t* q)
{
    if (q == NULL || q->buffer == NULL || q->length == 0u || q->item_size == 0u)
    {
        return 0u;
    }
    EnterCriticalSection(&q->lock);
    const uint32_t spaces = (q->count >= q->length) ? 0u : (q->length - q->count);
    LeaveCriticalSection(&q->lock);
    return spaces;
}

static HANDLE win_mutex_create(void)
{
    return CreateMutexA(NULL, FALSE, NULL);
}

static bool win_mutex_take(HANDLE mutex, time_t wait_time)
{
    if (mutex == NULL)
    {
        return false;
    }
    const DWORD timeout_ms = win_delay_to_timeout_ms_time(wait_time);
    const DWORD result = WaitForSingleObject(mutex, timeout_ms);
    return result == WAIT_OBJECT_0;
}

static void win_mutex_give(HANDLE mutex)
{
    if (mutex == NULL)
    {
        return;
    }
    ReleaseMutex(mutex);
}

static CRITICAL_SECTION g_suspend_lock;
static INIT_ONCE g_suspend_init_once = INIT_ONCE_STATIC_INIT;

static BOOL CALLBACK win_suspend_init(PINIT_ONCE init_once, PVOID parameter, PVOID* context)
{
    (void)init_once;
    (void)parameter;
    (void)context;
    InitializeCriticalSection(&g_suspend_lock);
    return TRUE;
}

static void win_suspend_all(void)
{
    InitOnceExecuteOnce(&g_suspend_init_once, win_suspend_init, NULL, NULL);
    EnterCriticalSection(&g_suspend_lock);
}

static void win_resume_all(void)
{
    InitOnceExecuteOnce(&g_suspend_init_once, win_suspend_init, NULL, NULL);
    LeaveCriticalSection(&g_suspend_lock);
}

static int win_priority_to_native(rtosPriority_e priority)
{
    switch (priority)
    {
    case rtosPriorityIdle:
        return THREAD_PRIORITY_IDLE;
    case rtosPriorityLow:
        return THREAD_PRIORITY_BELOW_NORMAL;
    case rtosPriorityBelowNormal:
        return THREAD_PRIORITY_BELOW_NORMAL;
    case rtosPriorityNormal:
        return THREAD_PRIORITY_NORMAL;
    case rtosPriorityAboveNormal:
        return THREAD_PRIORITY_ABOVE_NORMAL;
    case rtosPriorityHigh:
        return THREAD_PRIORITY_HIGHEST;
    case rtosPriorityRealtime:
        return THREAD_PRIORITY_TIME_CRITICAL;
    default:
        return THREAD_PRIORITY_NORMAL;
    }
}

typedef void (*win_task_fn_t)(void const* argument);

typedef struct
{
    win_task_fn_t fn;
    void* arg;
} win_task_start_t;

static unsigned __stdcall win_task_entry(void* param)
{
    win_task_start_t* start = (win_task_start_t*)param;
    if (start != NULL)
    {
        win_task_fn_t fn = start->fn;
        void* arg = start->arg;
        free(start);
        if (fn != NULL)
        {
            fn(arg);
        }
    }
    _endthreadex(0u);
    return 0u;
}

static  win_queue_t     g_eventos_req_queue;
static  win_queue_t     g_eventos_rsp_queue;
static  win_queue_t     g_media_req_queue;
static  win_queue_t     g_media_rsp_queue;
static  win_queue_t     g_ad_req_queue;
static  win_queue_t     g_ad_rsp_queue;

static       HANDLE     g_eventos_rsp_queue_mutex;
static       HANDLE     g_eventos_id_mutex;
static       HANDLE     g_media_rsp_queue_mutex;
static       HANDLE     g_ad_rsp_queue_mutex;

/*-系统监控-------------------------------------------------------------------------------*/
void cpuInfo(void)
{
    printf("CPU/OS: Windows, tick=%llu ms\r\n", (unsigned long long)GetTickCount64());
}
/*---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/

// 系统函数
void rtosTaskSuspendAll(void)
{
    win_suspend_all();
}
void rtosTaskResumeAll(void)
{
    win_resume_all();
}
void rtosThreadDelayUntil(uint32_t time)
{
    __declspec(thread) static uint64_t next_wake_ms = 0ull;
    const uint64_t now = win_now_ms();
    if (next_wake_ms == 0ull)
    {
        next_wake_ms = now;
    }
    next_wake_ms += (uint64_t)time;
    if (next_wake_ms > now)
    {
        const uint64_t delta = next_wake_ms - now;
        Sleep((DWORD)((delta > (uint64_t)INFINITE) ? INFINITE : (DWORD)delta));
    }
}

void rtosThreadDelay(uint32_t ms)
{
    Sleep((DWORD)ms);
}
void rtosTaskCreate(char* name, \
                            rtosPriority_e priority, \
                            void* func, \
                            uint32_t stackSize, \
                            void* arg)
{
    (void)name;
    if (func == NULL || stackSize == 0u)
    {
        return;
    }

    win_task_start_t* start = (win_task_start_t*)malloc(sizeof(win_task_start_t));
    if (start == NULL)
    {
        return;
    }
    start->fn = (win_task_fn_t)func;
    start->arg = arg;

    uintptr_t handle = _beginthreadex(NULL, (unsigned)stackSize, win_task_entry, start, 0u, NULL);
    if (handle == 0u)
    {
        free(start);
        return;
    }

    SetThreadPriority((HANDLE)handle, win_priority_to_native(priority));
    CloseHandle((HANDLE)handle);
}
void rtosTaskSelfDelete(void)
{
    _endthreadex(0u);
}

/*---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/
// 消息队列

/*---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/
// 行政管理部门队列
bool rtosAdGetMsg(void *msg,uint32_t delay)
{
    return win_queue_receive(&g_ad_req_queue, msg, delay);
}
bool rtosAdSendMsg(void *msg,uint32_t delay)
{
    return win_queue_send(&g_ad_rsp_queue, msg, delay);
}
bool rtosTakeMsgFromAd(void *msg,uint32_t delay)
{
    return win_queue_receive(&g_ad_rsp_queue, msg, delay);
}
bool rtosDeliverMsgToAd(void *msg,uint32_t delay)
{
    return win_queue_send(&g_ad_req_queue, msg, delay);
}
// 获取读队列互斥信号量
bool TakeAdMsgQueueMutex(time_t waitTime)
{
    return win_mutex_take(g_ad_rsp_queue_mutex, waitTime);
}

// 释放读队列互斥信号量
void ReleaseAdMsgQueueMutex(void)
{
    win_mutex_give(g_ad_rsp_queue_mutex);
}


// 检查请求队列中是否有可用空间
bool CheckAdqueueSpacesAvailable(void)
{
    return win_queue_spaces_available(&g_ad_req_queue) != 0u;
}
uint32_t  CheckAdRspMesgNum(void)
{
    return win_queue_messages_waiting(&g_ad_rsp_queue);
}

uint32_t  CheckAdReqMesgNum(void)
{
    return win_queue_messages_waiting(&g_ad_req_queue);
}
bool  PeekAdRspMesg(void *receivedMsg)
{
    // 使用队列的Peek功能查看消息而不取出
    return win_queue_peek(&g_ad_rsp_queue, receivedMsg);
}

// 检查请求队列中是否有可用空间
bool CheckAdQueueSpacesAvailable(void)
{
    return win_queue_spaces_available(&g_ad_req_queue) != 0u;
}

/*---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/
// 事件中心队列

bool rtosEventosGetMsg(void* msg, uint32_t delay)
{
    return win_queue_receive(&g_eventos_req_queue, msg, delay);
}

bool rtosEventosSendMsg(void *msg,uint32_t delay)
{
    return win_queue_send(&g_eventos_rsp_queue, msg, delay);
}

bool rtosTakeMsgFromEventos(void *msg,uint32_t delay)
{
    return win_queue_receive(&g_eventos_rsp_queue, msg, delay);
}
bool rtosDeliverMsgToEventos(void *msg,uint32_t delay)
{
    return win_queue_send(&g_eventos_req_queue, msg, delay);
}

// 获取读队列互斥信号量
bool TakeEventosMsgQueueMutex(time_t waitTime)
{
    return win_mutex_take(g_eventos_rsp_queue_mutex, waitTime);
}

// 释放读队列互斥信号量
void ReleaseEventosMsgQueueMutex(void)
{
    win_mutex_give(g_eventos_rsp_queue_mutex);
}


// 获取事件中心id互斥信号量
bool TakeEventosMutex(time_t waitTime)
{
    return win_mutex_take(g_eventos_id_mutex, waitTime);
}

// 释放事件中心id互斥信号量
void ReleaseEventosMutex(void)
{
    win_mutex_give(g_eventos_id_mutex);
}

uint32_t  CheckEventRspMesgNum(void)
{
    return win_queue_messages_waiting(&g_eventos_rsp_queue);
}

bool  PeekEventRspMesg(void *receivedMsg)
{
    // 使用队列的Peek功能查看消息而不取出
    return win_queue_peek(&g_eventos_rsp_queue, receivedMsg);
}

// 检查请求队列中是否有可用空间
bool CheckEventQueueSpacesAvailable(void)
{
    return win_queue_spaces_available(&g_eventos_req_queue) != 0u;
}
/*---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/
// 媒体中心队列
bool rtosMediaGetMsg(void *msg,uint32_t delay)
{
    return win_queue_receive(&g_media_req_queue, msg, delay);
}

bool rtosMediaSendMsg(void *msg,uint32_t delay)
{
    return win_queue_send(&g_media_rsp_queue, msg, delay);
}

bool rtosTakeMsgFromMedia(void *msg,uint32_t delay)
{
    return win_queue_receive(&g_media_rsp_queue, msg, delay);
}

bool rtosDeliverMsgToMedia(void *msg,uint32_t delay)
{
    return win_queue_send(&g_media_req_queue, msg, delay);
}

// 获取读队列互斥信号量
bool TakeMediaMutex(time_t waitTime)
{
    return win_mutex_take(g_media_rsp_queue_mutex, waitTime);
}
// 释放读队列互斥信号量
void ReleaseMediaMsgQueueMutex(void)
{
    win_mutex_give(g_media_rsp_queue_mutex);
}

uint32_t  CheckMediaRspMesgNum(void)
{
    return win_queue_messages_waiting(&g_media_rsp_queue);
}

uint32_t  CheckMediaReqMesgNum(void)
{
    return win_queue_messages_waiting(&g_media_req_queue);
}
bool  PeekMediaRspMesg(void *receivedMsg)
{
    // 使用队列的Peek功能查看消息而不取出
    return win_queue_peek(&g_media_rsp_queue, receivedMsg);
}
// 检查请求队列中是否有可用空间
bool CheckMediaQueueSpacesAvailable(void)
{
    return win_queue_spaces_available(&g_media_req_queue) != 0u;
}

void win_os_init(void)
{
    const uint32_t ad_item_size_fallback = 64u;

    /* 事件中心队列初始化 */
    (void)win_queue_init(&g_eventos_req_queue, 3u, (uint32_t)sizeof(message_t));
    (void)win_queue_init(&g_eventos_rsp_queue, 3u, (uint32_t)sizeof(message_t));

    /* 媒体中心队列初始化 */
    (void)win_queue_init(&g_media_req_queue, 3u, (uint32_t)sizeof(mediaMessage_t));
    (void)win_queue_init(&g_media_rsp_queue, 3u, (uint32_t)sizeof(mediaMessage_t));

    /* 行政管理部门队列初始化 */
    (void)win_queue_init(&g_ad_req_queue, 1u, ad_item_size_fallback);
    (void)win_queue_init(&g_ad_rsp_queue, 1u, ad_item_size_fallback);

    /* 互斥信号量创建 */
    g_eventos_rsp_queue_mutex = win_mutex_create();
    g_eventos_id_mutex        = win_mutex_create();
    g_media_rsp_queue_mutex   = win_mutex_create();
    g_ad_rsp_queue_mutex      = win_mutex_create();

    /* 任务创建 */
    rtosTaskCreate("ConsleTask", rtosPriorityNormal,       (void*)StartConsleTask,    1024u, NULL);
    rtosTaskCreate("EventTask",  rtosPriorityNormal,        (void*)StartEventTask,    640u,  NULL);
    rtosTaskCreate("MediaTask",  rtosPriorityNormal,        (void*)StartMediaTask,    640u,  NULL);
    rtosTaskCreate("MainTask",   rtosPriorityNormal,        (void*)StartMaintTask,    640u,  NULL);
    rtosTaskCreate("GuardTask",     rtosPriorityLow,        (void*)StartGuardTask,    512u,  NULL);
    rtosTaskCreate("AdTask",   rtosPriorityRealtime,        (void*)StartAdTask,       128u,  NULL);

    printf(" Windows os initialized successfully.\r\n");
}


#endif 
