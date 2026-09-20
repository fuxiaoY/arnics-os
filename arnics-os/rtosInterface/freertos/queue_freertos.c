/**
 * @file    queue_freertos.c
 * @brief   队列抽象层的 FreeRTOS(PLATFORM_MCU) 后端。
 *
 * @details
 * 把 #queue_ops_t 的 5 个操作直接桥接到 FreeRTOS 原生队列 API：
 *   - create -> xQueueCreate()
 *   - send   -> xQueueSend()
 *   - recv   -> xQueueReceive()
 *   - peek   -> xQueuePeek()
 *   - count  -> uxQueueMessagesWaiting()
 *
 * 参数语义与 FreeRTOS 保持一致：
 *   - @a len 为队列深度(元素个数)，@a item_size 为每元素字节数；
 *   - 超时 @a delay 单位为 FreeRTOS tick(通常配置为 1ms/tick)；
 *   - 队列存储与队列控制块由 FreeRTOS 堆(动态分配)提供。
 *
 * @copyright (c) 2026 arnics-os. Licensed under the project LICENSE.
 */

#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#if PLATFORM_MCU
#include "FreeRTOS.h"
#include "queue.h"
#include "rtosInterface/queue/queue_port.h"

/**
 * @brief 创建 FreeRTOS 队列。
 * @param[in] item_size 元素大小(字节)
 * @param[in] len       队列深度(元素个数)
 * @return QueueHandle_t(不透明返回)；内存不足时为 NULL
 */
static void* freertos_queue_create(uint32_t item_size, uint32_t len)
{
    return (void*)xQueueCreate(len, item_size);
}

/**
 * @brief 入队一个元素。
 * @param[in] q     队列句柄
 * @param[in] item  元素指针
 * @param[in] delay 超时(tick)，0 不等待
 * @return true 成功；false 队列满/超时
 */
static bool freertos_queue_send(void* q, const void* item, uint32_t delay)
{
    return (xQueueSend((QueueHandle_t)q, item, delay) == pdPASS);
}

/**
 * @brief 出队一个元素。
 * @param[in] q     队列句柄
 * @param[out] out  接收缓冲区
 * @param[in] delay 超时(tick)
 * @return true 成功；false 队列空/超时
 */
static bool freertos_queue_recv(void* q, void* out, uint32_t delay)
{
    return (xQueueReceive((QueueHandle_t)q, out, delay) == pdTRUE);
}

/**
 * @brief 窥探队首但不移除。
 * @param[in] q   队列句柄
 * @param[out] out 接收缓冲区
 * @return true 队列非空；false 队列为空
 */
static bool freertos_queue_peek(void* q, void* out)
{
    return (xQueuePeek((QueueHandle_t)q, out, 0) == pdTRUE);
}

/**
 * @brief 查询当前消息数。
 * @param[in] q 队列句柄
 * @return 已入队元素个数
 */
static uint32_t freertos_queue_count(void* q)
{
    return (uint32_t)uxQueueMessagesWaiting((QueueHandle_t)q);
}

/**
 * @brief FreeRTOS 后端操作表，queue_port.c 经 CURRENT_QUEUE_OPS 引用。
 */
const queue_ops_t queue_ops_freertos = 
{
    .create = freertos_queue_create,
    .send = freertos_queue_send,
    .recv = freertos_queue_recv,
    .peek = freertos_queue_peek,
    .count = freertos_queue_count,
};

#endif
