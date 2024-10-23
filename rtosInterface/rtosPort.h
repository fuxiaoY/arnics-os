#ifndef _RTOSPORT_H_
#define _RTOSPORT_H_    

#ifdef __cplusplus
extern "C" {
#endif
#include "../Inc/ProjDefine.h"
#include "../Inc/typedef.h"

#ifdef _USE_FREERTOS_
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"



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

#endif // _USE_FREERTOS_


extern void rtosThreadDelay(uint32_t ms);

extern bool rtosEventQueueReceive(void *msg,uint32_t delay);
extern bool rtosEventQueueSend(void *msg,uint32_t delay);
extern bool rtosEventQueueTake(void *msg,uint32_t delay);


// 获取读消息队列互斥信号量
extern bool TakeEventosMsgQueueMutex(time_t waitTime);
// 释放读消息队列互斥信号量
extern void ReleaseEventosMsgQueueMutex(void);
// 获取事件中心id互斥信号量
extern bool TakeEventosMutex(time_t waitTime);
// 释放事件中心id互斥信号量
extern void ReleaseEventosMutex(void);
extern uint32_t  CheckEventRspMesgNum(void);
extern bool  PeekEventRspMesg(void *receivedMsg);


#ifdef __cplusplus
}
#endif


#endif
