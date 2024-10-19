
#include "eventMessage.h"
#pragma arm section code="._entry_event_api"
// 非阻塞式查询队列是否有消息
BaseType_t NonBlockingQueueReceive(QueueHandle_t queue, Message_t *message)
{
    // 使用 xQueueReceive 函数，等待时间为 500
    return xQueueReceive(queue, message, 500);
}
// 非阻塞式查询队列是否有消息
BaseType_t BlockingQueueReceive(QueueHandle_t queue, Message_t *message)
{
    // 使用 xQueueReceive 函数，等待时间为 永久
    return xQueueReceive(queue, message, portMAX_DELAY);
}
