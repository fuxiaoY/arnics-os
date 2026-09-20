/**
 * @file    mutex_freertos.c
 * @brief   互斥量抽象层的 FreeRTOS(PLATFORM_MCU) 后端。
 *
 * @details
 * 用 FreeRTOS 互斥信号量实现跨部门互斥量：
 *   - init 调用 xSemaphoreCreateMutex() 创建；
 *   - take 调用 xSemaphoreTake()，单位为 FreeRTOS tick；
 *   - give 调用 xSemaphoreGive()。
 *
 * @copyright (c) 2026 arnics-os. Licensed under the project LICENSE.
 */

#include "rtosInterface/rtosInterface.h"
#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#if PLATFORM_MCU
#include "FreeRTOS.h"
#include "semphr.h"

static SemaphoreHandle_t g_ad_rsp_mutex = NULL;       // 行政管理队列互斥量
static SemaphoreHandle_t g_eventos_rsp_mutex = NULL;  // 事件中心队列互斥量
static SemaphoreHandle_t g_eventos_id_mutex = NULL;   // 事件中心 ID 互斥量
static SemaphoreHandle_t g_media_rsp_mutex = NULL;    // 媒体中心队列互斥量

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
