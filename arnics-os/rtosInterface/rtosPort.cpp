#include "rtosPort.h"

#ifdef _USE_FREERTOS_


//消息中心队列  
extern QueueHandle_t eventosReqQueue;
extern QueueHandle_t eventosRspQueue;
//媒体中心队列
extern QueueHandle_t MediaReqQueue;
extern QueueHandle_t MediaRspQueue;
//行政中心队列
extern QueueHandle_t adReqQueue;
extern QueueHandle_t adRspQueue;

// 消息中心信号量
extern SemaphoreHandle_t eventosRspQueue_xSemaphore; //读消息中心队列锁
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

bool rtosEventosGetMsg(void *msg,uint32_t delay)
{
    return (xQueueReceive(eventosReqQueue, msg, delay) == pdTRUE);
}

bool rtosEventosSendMsg(void *msg,uint32_t delay)
{
    return (xQueueSend(eventosRspQueue, msg, delay) == pdPASS);
}

bool rtosTakeMsgFromEventos(void *msg,uint32_t delay)
{
    return (xQueueReceive(eventosRspQueue, msg, delay) == pdTRUE);
}
bool rtosDeliverMsgToEventos(void *msg,uint32_t delay)
{
    return (xQueueSend(eventosReqQueue, msg, delay) == pdTRUE);
}

// 获取读消息队列互斥信号量
bool TakeEventosMsgQueueMutex(time_t waitTime)
{
    return (xSemaphoreTake(eventosRspQueue_xSemaphore, waitTime) == pdTRUE);
}

// 释放读消息队列互斥信号量
void ReleaseEventosMsgQueueMutex(void)
{
    xSemaphoreGive(eventosRspQueue_xSemaphore);
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
    return (uint32_t)uxQueueMessagesWaiting(eventosRspQueue);
}

bool  PeekEventRspMesg(void *receivedMsg)
{
    // 使用队列的Peek功能查看消息而不取出
    return  (uint32_t)xQueuePeek(eventosRspQueue, receivedMsg, 0)== pdTRUE;
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
