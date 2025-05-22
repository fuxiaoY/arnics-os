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
#include "freertosDef.h"
#endif // _USE_FREERTOS_
extern void cpuInfo(void);
extern void vApplicationMallocFailedHook( void ) ;
extern void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName );
extern void rtosTaskSuspendAll(void);
extern void rtosTaskResumeAll(void);
extern void rtosThreadDelayUntil(uint32_t time);
extern void rtosThreadDelay(uint32_t ms);

/*---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/
// 行政管理部门队列
extern bool rtosAdGetMsg(void *msg,uint32_t delay);
extern bool rtosAdSendMsg(void *msg,uint32_t delay);
extern bool rtosTakeMsgFromAd(void *msg,uint32_t delay);
extern bool rtosDeliverMsgToAd(void *msg,uint32_t delay);
// 获取读队列互斥信号量
extern bool TakeAdMsgQueueMutex(time_t waitTime);
// 释放读队列互斥信号量
extern void ReleaseAdMsgQueueMutex(void);
// 检查请求队列中是否有可用空间
extern bool CheckAdqueueSpacesAvailable(void);
extern uint32_t  CheckAdRspMesgNum(void);
extern uint32_t  CheckAdReqMesgNum(void);
extern bool  PeekAdRspMesg(void *receivedMsg);
// 检查请求队列中是否有可用空间
extern bool CheckAdQueueSpacesAvailable(void);
/*---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/
// 事件中心队列
extern bool rtosEventosGetMsg(void *msg,uint32_t delay);
extern bool rtosEventosSendMsg(void *msg,uint32_t delay);
extern bool rtosTakeMsgFromEventos(void *msg,uint32_t delay);
extern bool rtosDeliverMsgToEventos(void *msg,uint32_t delay);
// 获取读队列互斥信号量
extern bool TakeEventosMsgQueueMutex(time_t waitTime);
// 释放读队列互斥信号量
extern void ReleaseEventosMsgQueueMutex(void);
// 获取事件中心id互斥信号量
extern bool TakeEventosMutex(time_t waitTime);
// 释放事件中心id互斥信号量
extern void ReleaseEventosMutex(void);
extern uint32_t  CheckEventRspMesgNum(void);
extern bool  PeekEventRspMesg(void *receivedMsg);
// 检查请求队列中是否有可用空间
extern bool CheckEventQueueSpacesAvailable(void);
/*---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/
// 媒体中心队列
extern bool rtosMediaGetMsg(void *msg,uint32_t delay);
extern bool rtosMediaSendMsg(void *msg,uint32_t delay);
extern bool rtosTakeMsgFromMedia(void *msg,uint32_t delay);
extern bool rtosDeliverMsgToMedia(void *msg,uint32_t delay);
// 获取读队列互斥信号量
extern bool TakeMediaMutex(time_t waitTime);
// 释放读队列互斥信号量
extern void ReleaseMediaMsgQueueMutex(void);
extern uint32_t  CheckMediaRspMesgNum(void);
extern uint32_t  CheckMediaReqMesgNum(void);
extern bool  PeekMediaRspMesg(void *receivedMsg);
// 检查请求队列中是否有可用空间
extern bool CheckMediaQueueSpacesAvailable(void);

#ifdef __cplusplus
}
#endif


#endif
