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
#endif // _USE_FREERTOS_

extern void rtosTaskSuspendAll(void);
extern void rtosTaskResumeAll(void);
extern void rtosThreadDelayUntil(uint32_t time);
extern void rtosThreadDelay(uint32_t ms);

extern bool rtosEventosGetMsg(void *msg,uint32_t delay);
extern bool rtosEventosSendMsg(void *msg,uint32_t delay);
extern bool rtosTakeMsgFromEventos(void *msg,uint32_t delay);
extern bool rtosDeliverMsgToEventos(void *msg,uint32_t delay);

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
