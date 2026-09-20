
#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#if PLATFORM_WIN
#include <windows.h>
#include <process.h>

#include "rtosInterface/rtosInterfacePublic.h"
#include "rtosInterface/queue/queue_port.h"
#include "common/TaskTimer.h"
#include "dePartment/centerEvent/centerEvent.h"
#include "dePartment/centerMedia/centerMedia.h"
#include "dePartment/centerAdministrative/centerAdministrative.h"


static uint64_t win_now_ms(void)
{
    return (uint64_t)GetTickCount64();
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

void os_task_create(void)
{
    /* 统一队列初始化 */
    queue_init_all();
    /* 统一互斥锁初始化 */
    compat_init_mutexes();

    /* 任务创建 */
    rtosTaskCreate("ConsleTask", rtosPriorityNormal,       (void*)StartConsleTask,    1024u, NULL);
    rtosTaskCreate("EventTask",  rtosPriorityNormal,        (void*)StartEventTask,    640u,  NULL);
    rtosTaskCreate("MediaTask",  rtosPriorityNormal,        (void*)StartMediaTask,    640u,  NULL);
    rtosTaskCreate("MainTask",   rtosPriorityNormal,        (void*)StartMaintTask,    640u,  NULL);
    rtosTaskCreate("GuardTask",    rtosPriorityHigh,        (void*)StartGuardTask,    512u,  NULL);
    rtosTaskCreate("AdTask",       rtosPriorityHigh,        (void*)StartAdTask,       128u,  NULL);

    printf(" Windows os initialized successfully.\r\n");
}
/**
 * @brief 请求1ms定时器分辨率
 * @return unsigned int 0 成功，其他值 失败
 * @note 该函数在Windows系统中使用timeBeginPeriod API请求1ms定时器分辨率
 * @note 该函数在Windows系统中仅在系统启动时调用一次，后续调用将返回0
*/
typedef unsigned int(WINAPI* winmm_time_begin_period_fn)(unsigned int uPeriod);

static void win_request_1ms_timer_resolution(void)
{
    HMODULE h_winmm = LoadLibraryA("winmm.dll");
    if (h_winmm == NULL)
    {
        return;
    }
    winmm_time_begin_period_fn timeBeginPeriodFn = (winmm_time_begin_period_fn)GetProcAddress(h_winmm, "timeBeginPeriod");
    if (timeBeginPeriodFn != NULL)
    {
        (void)timeBeginPeriodFn(1u);
    }
}

static unsigned __stdcall win_systick_thread(void* arg)
{
    (void)arg;
    win_request_1ms_timer_resolution();

    HANDLE h_timer = CreateWaitableTimer(NULL, FALSE, NULL);
    if (h_timer == NULL)
    {
        return 0u;
    }

    LARGE_INTEGER due_time;
    due_time.QuadPart = -10000LL;
    if (!SetWaitableTimer(h_timer, &due_time, 1, NULL, NULL, FALSE))
    {
        CloseHandle(h_timer);
        return 0u;
    }

    uint64_t last_ms = GetTickCount64();
    while (1)
    {
        (void)WaitForSingleObject(h_timer, INFINITE);
        const uint64_t now_ms = GetTickCount64();
        const uint64_t delta_ms = now_ms - last_ms;
        if (delta_ms != 0ull)
        {
            arnics_addTick((uint32_t)((delta_ms > (uint64_t)UINT32_MAX) ? (uint64_t)UINT32_MAX : delta_ms));
            last_ms = now_ms;
        }
    }
    return 0u;
}

static void arnics_systick_start(void)
{
    static volatile LONG started = 0;
    if (InterlockedCompareExchange(&started, 1, 0) != 0)
    {
        return;
    }

    uintptr_t handle = _beginthreadex(NULL, 0u, win_systick_thread, NULL, 0u, NULL);
    if (handle != 0u)
    {
        CloseHandle((HANDLE)handle);
    }
}
void win_os_init(void)
{
    arnics_systick_start();
    rtosTaskCreate("initTask",   rtosPriorityRealtime,        (void*)StartInitTask,       500u,  NULL);
}


#endif 
