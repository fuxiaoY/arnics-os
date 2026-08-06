#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#if PLATFORM_LINUX
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <time.h>
#include <sys/timerfd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "rtosInterface/rtosInterfacePublic.h"
#include "rtosInterface/queue/queue_port.h"
#include "common/TaskTimer.h"
#include "dePartment/centerEvent/centerEvent.h"
#include "dePartment/centerMedia/centerMedia.h"
#include "dePartment/centerAdministrative/centerAdministrative.h"

static inline uint64_t linux_now_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000ull + (uint64_t)tv.tv_usec / 1000ull;
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

void os_task_create(void)
{
    /* 统一队列初始化 */
    queue_init_all();
    /* 统一互斥锁初始化 */
    compat_init_mutexes();

    rtosTaskCreate("ConsleTask", rtosPriorityNormal,  (void*)StartConsleTask, 1024u, NULL);
    rtosTaskCreate("EventTask",   rtosPriorityNormal, (void*)StartEventTask,   640u, NULL);
    rtosTaskCreate("MediaTask",   rtosPriorityNormal, (void*)StartMediaTask,   640u, NULL);
    rtosTaskCreate("MainTask",    rtosPriorityNormal, (void*)StartMaintTask,   640u, NULL);
    rtosTaskCreate("GuardTask",      rtosPriorityLow, (void*)StartGuardTask,   512u, NULL);
    rtosTaskCreate("AdTask",    rtosPriorityRealtime, (void*)StartAdTask,      128u, NULL);

    printf(" Linux os initialized successfully.\r\n");
}

/**
 * @brief Linux 平台下的 1ms systick 刷新线程(基于 timerfd)
 *
 * @details
 * - 使用 `timerfd_create(CLOCK_MONOTONIC, ...)` 创建内核定时器 fd，并配置为 1ms 周期触发。
 * - 线程通过阻塞 `read(fd, &expirations, sizeof(expirations))` 等待定时器到期。
 * - `expirations` 表示自上次 read 以来累计到期次数(可能 > 1，代表线程/系统调度延迟导致错过若干个周期)。
 * - 每次读到的到期次数会累加到 `arnics_systick`(通过 `arnics_addTick`)。
 *
 * @note
 * - 该线程不保证“每 1ms 准时唤醒一次”，但能保证 tick 按实际累计到期次数推进。
 * - 使用 `CLOCK_MONOTONIC` 避免系统时间调整(NTP/手动改时钟)导致的回退或跳变影响超时计算。
 *
 * @param[in] arg 线程参数(未使用)
 * @return 始终返回 NULL(线程长期运行；出现错误时提前返回)
 */
static void* linux_systick_thread(void* arg)
{
    (void)arg;

    /* 创建 timerfd：把“时间事件”转换成可读的文件描述符 */
    const int fd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC);
    if (fd < 0)
    {
        return NULL;
    }

    /* 配置 1ms 周期：it_value 为首次到期时间，it_interval 为后续周期 */
    struct itimerspec its;
    memset(&its, 0, sizeof(its));
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 1000000L;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 1000000L;
    if (timerfd_settime(fd, 0, &its, NULL) != 0)
    {
        close(fd);
        return NULL;
    }

    while (1)
    {
        uint64_t expirations = 0ull;
        /* read 将阻塞直到至少发生 1 次到期；返回值为累计到期次数 */
        const ssize_t n = read(fd, &expirations, sizeof(expirations));
        if (n == (ssize_t)sizeof(expirations))
        {
            /* 到期次数可能很大，避免 uint32_t 溢出(arnics_addTick 形参为 uint32_t) */
            const uint64_t add = (expirations > (uint64_t)UINT32_MAX) ? (uint64_t)UINT32_MAX : expirations;
            if (add != 0ull)
            {
                arnics_addTick((uint32_t)add);
            }
        }
        else if (n < 0 && errno == EINTR)
        {
            /* 被信号中断：继续等待下一次到期 */
            continue;
        }
        else
        {
            /* 其它错误：退出线程(例如 fd 被关闭/系统异常) */
            break;
        }
    }
    close(fd);
    return NULL;
}

static pthread_once_t g_linux_systick_once = PTHREAD_ONCE_INIT;

/**
 * @brief pthread_once 的实际启动函数
 *
 * @details
 * - 创建并 detach systick 线程，避免主线程需要 join。
 * - 由 `linux_systick_start()` 保证只执行一次。
 */
static void linux_systick_start_impl(void)
{
    pthread_t tid;
    if (pthread_create(&tid, NULL, linux_systick_thread, NULL) == 0)
    {
        pthread_detach(tid);
    }
}

/**
 * @brief 启动 Linux systick 刷新线程(保证只启动一次)
 */
static void linux_systick_start(void)
{
    pthread_once(&g_linux_systick_once, linux_systick_start_impl);
}

void linux_os_init(void)
{
    linux_systick_start();
    rtosTaskCreate("initTask",   rtosPriorityRealtime,        (void*)StartInitTask,       500u,  NULL);
}

#endif
