/**
 * @file    mutex_win.c
 * @brief   互斥量抽象层的 Windows(PLATFORM_WIN) 后端。
 *
 * @details
 * 用 Win32 互斥体实现跨部门互斥量，供主机侧开发/单元测试使用：
 *   - take 支持超时(毫秒)与永久等待(#BLOCK_DELAY -> INFINITE)；
 *   - give 释放互斥体。
 *
 * @copyright (c) 2026 arnics-os. Licensed under the project LICENSE.
 */

#include "rtosInterface/rtosInterface.h"
#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#if PLATFORM_WIN
#include <windows.h>

static HANDLE g_ad_rsp_mutex = NULL;       // 行政管理队列互斥量
static HANDLE g_eventos_rsp_mutex = NULL; // 事件中心队列互斥量
static HANDLE g_eventos_id_mutex = NULL;  // 事件中心 ID 互斥量
static HANDLE g_media_rsp_mutex = NULL;   // 媒体中心队列互斥量

/**
 * @brief 创建 Win32 互斥体。
 * @return 互斥体句柄；失败返回 NULL
 */
static HANDLE win_mutex_create(void)
{
    return CreateMutexA(NULL, FALSE, NULL);
}

/**
 * @brief 获取互斥体。
 * @param[in] mutex     互斥体句柄
 * @param[in] wait_time 超时(毫秒)，#BLOCK_DELAY 永久等待
 * @return true 成功；false 句柄无效、超时或失败
 */
static bool win_mutex_take(HANDLE mutex, time_t wait_time)
{
    if (mutex == NULL) return false;
    const DWORD timeout_ms = (wait_time == BLOCK_DELAY) ? INFINITE : (DWORD)wait_time;
    const DWORD result = WaitForSingleObject(mutex, timeout_ms);
    return result == WAIT_OBJECT_0;
}

/**
 * @brief 释放互斥体。
 * @param[in] mutex 互斥体句柄
 */
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

#endif
