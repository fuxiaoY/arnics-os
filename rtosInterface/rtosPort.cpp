#include "rtosPort.h"

#ifdef _USE_FREERTOS_


//消息中心队列  
extern QueueHandle_t eventosReceiveQueue;
extern QueueHandle_t eventosSendQueue;
//媒体中心队列
extern QueueHandle_t MediaReceiveQueue;
extern QueueHandle_t MediaSendQueue;
//行政中心队列
extern QueueHandle_t adReceiveQueue;
extern QueueHandle_t adSendQueue;

// 消息中心信号量
extern SemaphoreHandle_t eventosSendQueue_xSemaphore; //读消息中心队列锁
extern SemaphoreHandle_t eventosID_mutex;   //消息ID锁

/*---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/
// 系统函数
void rtosTaskSuspendAll(void)
{
    vTaskSuspendAll();
}
void rtosTaskResumeAll(void)
{
    xTaskResumeAll();
}

void rtosThreadDelay(uint32_t ms)
{
    osDelay(ms);
}

/*---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/
// 消息队列

/*---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/
// 消息中心队列 媒体中心队列 休眠中心队列
// 指针数组映射

bool rtosEventQueueReceive(void *msg,uint32_t delay)
{
    return (xQueueReceive(eventosReceiveQueue, msg, delay) == pdTRUE);
}

bool rtosEventQueueSend(void *msg,uint32_t delay)
{
    return (xQueueSend(eventosSendQueue, msg, delay) == pdPASS);
}

bool rtosEventQueueTake(void *msg,uint32_t delay)
{
    return (xQueueReceive(eventosSendQueue, msg, delay) == pdTRUE);
}
bool rtosEventQueueReq(void *msg,uint32_t delay)
{
    return (xQueueSend(eventosReceiveQueue, msg, delay) == pdTRUE);
}

// 获取读消息队列互斥信号量
bool TakeEventosMsgQueueMutex(time_t waitTime)
{
    return (xSemaphoreTake(eventosSendQueue_xSemaphore, waitTime) == pdTRUE);
}

// 释放读消息队列互斥信号量
void ReleaseEventosMsgQueueMutex(void)
{
    xSemaphoreGive(eventosSendQueue_xSemaphore);
}


// 获取事件中心id互斥信号量
bool TakeEventosMutex(time_t waitTime)
{
    return (xSemaphoreTake(eventosID_mutex, waitTime) == pdTRUE);
}

// 释放事件中心id互斥信号量
void ReleaseEventosMutex(void)
{
    xSemaphoreGive(eventosID_mutex);
}

uint32_t  CheckEventRspMesgNum(void)
{
    return (uint32_t)uxQueueMessagesWaiting(eventosSendQueue);
}

bool  PeekEventRspMesg(void *receivedMsg)
{
    // 使用队列的Peek功能查看消息而不取出
    return  (uint32_t)xQueuePeek(eventosSendQueue, receivedMsg, 0)== pdTRUE;
}



#else
void rtosThreadDelay(uint32_t ms)
{
}

bool rtosNonBlockingEventQueueReceive(void *msg)
{

    return false;

}

bool rtosBlockingEventQueueReceive(void *msg)
{
    while(1);
    return false;
}

#endif // _USE_FREERTOS_
