
#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#if PLATFORM_MCU
#include "FreeRTOS.h"
#include "queue.h"
#include "rtosInterface/queue/queue_port.h"

static void* freertos_queue_create(uint32_t item_size, uint32_t len)
{
    return (void*)xQueueCreate(len, item_size);
}

static bool freertos_queue_send(void* q, const void* item, uint32_t delay)
{
    return (xQueueSend((QueueHandle_t)q, item, delay) == pdPASS);
}

static bool freertos_queue_recv(void* q, void* out, uint32_t delay)
{
    return (xQueueReceive((QueueHandle_t)q, out, delay) == pdTRUE);
}

static bool freertos_queue_peek(void* q, void* out)
{
    return (xQueuePeek((QueueHandle_t)q, out, 0) == pdTRUE);
}

static uint32_t freertos_queue_count(void* q)
{
    return (uint32_t)uxQueueMessagesWaiting((QueueHandle_t)q);
}

const queue_ops_t queue_ops_freertos = {
    .create = freertos_queue_create,
    .send = freertos_queue_send,
    .recv = freertos_queue_recv,
    .peek = freertos_queue_peek,
    .count = freertos_queue_count,
};

#endif
