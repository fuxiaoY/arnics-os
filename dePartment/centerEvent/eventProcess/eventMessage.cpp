
#include "eventMessage.h"
#include "../../../thirdParty/thirdPartyInclude.h"
#pragma arm section code="._entry_event_api"
// framecounter
static unsigned long global_id_counter = 0;
#ifdef _USE_FREERTOS_
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

/**
 * @brief  向事件中心发送消息
 * @param  无
 * @retval 
 */
time_t SendEventFlagToEventCenter(uint32_t *eventflag, time_t wait)
{
    // 发送消息到事件队列
    // 创建消息
    Message_t msg = {0};
    // 获取消息ID
    if (xSemaphoreTake(eventosID_mutex, portMAX_DELAY) == pdTRUE) 
    {
        // 生成唯一ID
        msg.ID_Ts = ++global_id_counter;
        // 释放互斥信号量
        xSemaphoreGive(eventosID_mutex);
    } 
    memcpy(msg.buf, eventflag, sizeof(uint32_t));
    // 等待时间为wait
    if (xQueueSend(eventosReceiveQueue, &msg, wait) == pdPASS)
    {
        ULOG_DEBUG("Message:needSample sent succeed! ID=%d",msg.ID_Ts);
        return msg.ID_Ts;
    }
    else
    {
        ULOG_DEBUG("Message:needSample sent timeout failed!");
        return 0;
    }
}

/**
 * @brief  向事件中心取出消息
 * @param  无
 * @retval 
 * @attention 采用信号量保证只有一个任务可以访问该函数，会自动回填消息
 */
bool GetResponseMessageFromEventCenter(time_t ID, time_t wait)
{
    Message_t receivedMsg;
    UBaseType_t numMessages;
    BaseType_t receiveStatus;
    bool foundMessage = FALSE;

    // 无限循环遍历消息队列
    while (1)
    {
        numMessages = uxQueueMessagesWaiting(eventosSendQueue);
        for (UBaseType_t i = 0; i < numMessages; i++)
        {
            // 使用队列的Peek功能查看消息而不取出
            receiveStatus = xQueuePeek(eventosSendQueue, &receivedMsg, 0);
            if (receiveStatus == pdTRUE)
            {
                if (receivedMsg.ID_Ts == ID)
                {
                    foundMessage = TRUE;
                    break;
                }
            }
        }

        // 如果找到消息，退出循环
        if (foundMessage)
        {
            break;
        }

        // 如果没有找到，延时一段时间再重试
        vTaskDelay(10);
    }

    // 获取信号量
    if (xSemaphoreTake(eventosSendQueue_xSemaphore, wait) == pdTRUE)
    {
        // 查找并取出属于自己的消息
        for (UBaseType_t i = 0; i < numMessages; i++)
        {
            receiveStatus = xQueueReceive(eventosSendQueue, &receivedMsg, portMAX_DELAY);
            if (receiveStatus == pdTRUE)
            {
                if (receivedMsg.ID_Ts == ID)
                {
                    // 找到消息，释放信号量并返回 TRUE
                    xSemaphoreGive(eventosSendQueue_xSemaphore);
                    return TRUE;
                }
                else
                {
                    // 如果不是自己的消息，重新放回队列
                    xQueueSend(eventosSendQueue, &receivedMsg, portMAX_DELAY);
                }
            }
            else
            {
                // 接收失败
                break; // 无需继续循环
            }
        }

        // 没有找到消息，释放信号量并返回 FALSE
        xSemaphoreGive(eventosSendQueue_xSemaphore);
        return FALSE;
    }
    else
    {
        // 如果获取信号量失败，返回 FALSE
        return FALSE;
    }
}
#else


#endif

