/**
 * @file    mutex_linux.c
 * @brief   互斥量抽象层的 Linux/POSIX(PLATFORM_LINUX) 后端。
 *
 * @details
 * 用 pthread 互斥量实现跨部门互斥量，供主机侧开发/单元测试使用：
 *   - take 支持超时(毫秒)、不等待(0)与永久等待(#BLOCK_DELAY)；
 *   - give 释放互斥量。
 * 互斥体通过 malloc 分配。
 *
 * @copyright (c) 2026 arnics-os. Licensed under the project LICENSE.
 */

#include "rtosInterface/rtosInterface.h"
#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#if PLATFORM_LINUX
#include <pthread.h>
#include <stdlib.h>

static pthread_mutex_t *g_ad_rsp_mutex = NULL;       // 行政管理队列互斥量
static pthread_mutex_t *g_eventos_rsp_mutex = NULL;  // 事件中心队列互斥量
static pthread_mutex_t *g_eventos_id_mutex = NULL;   // 事件中心 ID 互斥量
static pthread_mutex_t *g_media_rsp_mutex = NULL;    // 媒体中心队列互斥量

/**
 * @brief 创建并初始化 pthread 互斥量。
 * @return 互斥量指针；内存不足返回 NULL
 */
static pthread_mutex_t *linux_mutex_create(void)
{
    pthread_mutex_t *mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    if (mutex) pthread_mutex_init(mutex, NULL);
    return mutex;
}

/**
 * @brief 获取互斥量。
 * @param[in] mutex     互斥量指针
 * @param[in] wait_time 0 不等待，#BLOCK_DELAY 永久等待，其余为毫秒
 * @return true 成功；false 句柄无效、超时或失败
 */
static bool linux_mutex_take(pthread_mutex_t *mutex, time_t wait_time)
{
    if (mutex == NULL) return false;
    if (wait_time == BLOCK_DELAY)
    {
        return (pthread_mutex_lock(mutex) == 0);
    }
    else if (wait_time == 0)
    {
        return (pthread_mutex_trylock(mutex) == 0);
    }
    else
    {
        struct timespec ts;
        ts.tv_sec = (time_t)(wait_time / 1000);
        ts.tv_nsec = (long)((wait_time % 1000) * 1000000);
        return (pthread_mutex_timedlock(mutex, &ts) == 0);
    }
}

/**
 * @brief 释放互斥量。
 * @param[in] mutex 互斥量指针
 */
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

#endif
