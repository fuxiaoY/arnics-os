#ifndef ENTRY_RTOS_API_H_
#define ENTRY_RTOS_API_H_
#include "Inc/ProjDefine.h"
#include "Inc/typedef.h"
#include "rtosInterface/rtosInterface.h"
#ifdef _USE_FREERTOS_
#define BLOCK_DELAY 0xffffffffUL
#endif

// 函数指针类型的定义
typedef void     (T_TYPEDEF(cpuInfo))(void);
typedef void     (T_TYPEDEF(rtosTaskSuspendAll))(void);
typedef void     (T_TYPEDEF(rtosTaskResumeAll))(void);
typedef void     (T_TYPEDEF(rtosThreadDelayUntil))(uint32_t time);
typedef void     (T_TYPEDEF(rtosThreadDelay))(uint32_t ms);
typedef void     (T_TYPEDEF(rtosTaskCreate))(char* name, \
                                            rtosPriority_e priority, \
                                            void* func, \
                                            uint32_t stackSize, \
                                            void* arg);
typedef void     (T_TYPEDEF(rtosTaskSelfDelete))(void);
typedef bool     (T_TYPEDEF(rtosAdGetMsg))(void *msg, uint32_t delay);
typedef bool     (T_TYPEDEF(rtosAdSendMsg))(void *msg, uint32_t delay);
typedef bool     (T_TYPEDEF(rtosTakeMsgFromAd))(void *msg, uint32_t delay);
typedef bool     (T_TYPEDEF(rtosDeliverMsgToAd))(void *msg, uint32_t delay);
typedef bool     (T_TYPEDEF(TakeAdMsgQueueMutex))(time_t waitTime);
typedef void     (T_TYPEDEF(ReleaseAdMsgQueueMutex))(void);
typedef bool     (T_TYPEDEF(CheckAdqueueSpacesAvailable))(void);
typedef uint32_t (T_TYPEDEF(CheckAdRspMesgNum))(void);
typedef uint32_t (T_TYPEDEF(CheckAdReqMesgNum))(void);
typedef bool     (T_TYPEDEF(PeekAdRspMesg))(void *receivedMsg);
typedef bool     (T_TYPEDEF(CheckAdQueueSpacesAvailable))(void);

typedef bool     (T_TYPEDEF(rtosEventosGetMsg))(void *msg, uint32_t delay);
typedef bool     (T_TYPEDEF(rtosEventosSendMsg))(void *msg, uint32_t delay);
typedef bool     (T_TYPEDEF(rtosTakeMsgFromEventos))(void *msg, uint32_t delay);
typedef bool     (T_TYPEDEF(rtosDeliverMsgToEventos))(void *msg, uint32_t delay);
typedef bool     (T_TYPEDEF(TakeEventosMsgQueueMutex))(time_t waitTime);
typedef void     (T_TYPEDEF(ReleaseEventosMsgQueueMutex))(void);
typedef bool     (T_TYPEDEF(TakeEventosMutex))(time_t waitTime);
typedef void     (T_TYPEDEF(ReleaseEventosMutex))(void);
typedef uint32_t (T_TYPEDEF(CheckEventRspMesgNum))(void);
typedef bool     (T_TYPEDEF(PeekEventRspMesg))(void *receivedMsg);
typedef bool     (T_TYPEDEF(CheckEventQueueSpacesAvailable))(void);

typedef bool     (T_TYPEDEF(rtosMediaGetMsg))(void *msg, uint32_t delay);
typedef bool     (T_TYPEDEF(rtosMediaSendMsg))(void *msg, uint32_t delay);
typedef bool     (T_TYPEDEF(rtosTakeMsgFromMedia))(void *msg, uint32_t delay);
typedef bool     (T_TYPEDEF(rtosDeliverMsgToMedia))(void *msg, uint32_t delay);
typedef bool     (T_TYPEDEF(TakeMediaMutex))(time_t waitTime);
typedef void     (T_TYPEDEF(ReleaseMediaMsgQueueMutex))(void);
typedef uint32_t (T_TYPEDEF(CheckMediaRspMesgNum))(void);
typedef uint32_t (T_TYPEDEF(CheckMediaReqMesgNum))(void);
typedef bool     (T_TYPEDEF(PeekMediaRspMesg))(void *receivedMsg);
typedef bool     (T_TYPEDEF(CheckMediaQueueSpacesAvailable))(void);

// 输出函数接口结构体
typedef struct
{
    T_STRUCT_MEMBER(cpuInfo);
    T_STRUCT_MEMBER(rtosTaskSuspendAll);
    T_STRUCT_MEMBER(rtosTaskResumeAll);
    T_STRUCT_MEMBER(rtosThreadDelayUntil);
    T_STRUCT_MEMBER(rtosThreadDelay);
    T_STRUCT_MEMBER(rtosTaskCreate);
    T_STRUCT_MEMBER(rtosTaskSelfDelete);

    T_STRUCT_MEMBER(rtosAdGetMsg);
    T_STRUCT_MEMBER(rtosAdSendMsg);
    T_STRUCT_MEMBER(rtosTakeMsgFromAd);
    T_STRUCT_MEMBER(rtosDeliverMsgToAd);
    T_STRUCT_MEMBER(TakeAdMsgQueueMutex);
    T_STRUCT_MEMBER(ReleaseAdMsgQueueMutex);
    T_STRUCT_MEMBER(CheckAdqueueSpacesAvailable);
    T_STRUCT_MEMBER(CheckAdRspMesgNum);
    T_STRUCT_MEMBER(CheckAdReqMesgNum);
    T_STRUCT_MEMBER(PeekAdRspMesg);
    T_STRUCT_MEMBER(CheckAdQueueSpacesAvailable);

    T_STRUCT_MEMBER(rtosEventosGetMsg);
    T_STRUCT_MEMBER(rtosEventosSendMsg);
    T_STRUCT_MEMBER(rtosTakeMsgFromEventos);
    T_STRUCT_MEMBER(rtosDeliverMsgToEventos);
    T_STRUCT_MEMBER(TakeEventosMsgQueueMutex);
    T_STRUCT_MEMBER(ReleaseEventosMsgQueueMutex);
    T_STRUCT_MEMBER(TakeEventosMutex);
    T_STRUCT_MEMBER(ReleaseEventosMutex);
    T_STRUCT_MEMBER(CheckEventRspMesgNum);
    T_STRUCT_MEMBER(PeekEventRspMesg);
    T_STRUCT_MEMBER(CheckEventQueueSpacesAvailable);

    T_STRUCT_MEMBER(rtosMediaGetMsg);
    T_STRUCT_MEMBER(rtosMediaSendMsg);
    T_STRUCT_MEMBER(rtosTakeMsgFromMedia);
    T_STRUCT_MEMBER(rtosDeliverMsgToMedia);
    T_STRUCT_MEMBER(TakeMediaMutex);
    T_STRUCT_MEMBER(ReleaseMediaMsgQueueMutex);
    T_STRUCT_MEMBER(CheckMediaRspMesgNum);
    T_STRUCT_MEMBER(CheckMediaReqMesgNum);
    T_STRUCT_MEMBER(PeekMediaRspMesg);
    T_STRUCT_MEMBER(CheckMediaQueueSpacesAvailable);
} tRTOSEntry;

// 作为一个指向结构体的指针
extern const tRTOSEntry entry_rtos_list;
//  指向 函数指针
#define rtos_api (&entry_rtos_list)
#define RTOS_MICRODEF(name) (rtos_api->t_##name)

#define cpuInfo                                   RTOS_MICRODEF(cpuInfo)
#define rtosTaskSuspendAll                        RTOS_MICRODEF(rtosTaskSuspendAll)
#define rtosTaskResumeAll                         RTOS_MICRODEF(rtosTaskResumeAll)
#define rtosThreadDelayUntil                      RTOS_MICRODEF(rtosThreadDelayUntil)
#define rtosThreadDelay                           RTOS_MICRODEF(rtosThreadDelay)
#define rtosTaskCreate                            RTOS_MICRODEF(rtosTaskCreate)
#define rtosTaskSelfDelete                        RTOS_MICRODEF(rtosTaskSelfDelete)

#define rtosAdGetMsg                              RTOS_MICRODEF(rtosAdGetMsg)
#define rtosAdSendMsg                             RTOS_MICRODEF(rtosAdSendMsg)
#define rtosTakeMsgFromAd                         RTOS_MICRODEF(rtosTakeMsgFromAd)
#define rtosDeliverMsgToAd                        RTOS_MICRODEF(rtosDeliverMsgToAd)
#define TakeAdMsgQueueMutex                       RTOS_MICRODEF(TakeAdMsgQueueMutex)
#define ReleaseAdMsgQueueMutex                    RTOS_MICRODEF(ReleaseAdMsgQueueMutex)
#define CheckAdqueueSpacesAvailable               RTOS_MICRODEF(CheckAdqueueSpacesAvailable)
#define CheckAdRspMesgNum                         RTOS_MICRODEF(CheckAdRspMesgNum)
#define CheckAdReqMesgNum                         RTOS_MICRODEF(CheckAdReqMesgNum)
#define PeekAdRspMesg                             RTOS_MICRODEF(PeekAdRspMesg)
#define CheckAdQueueSpacesAvailable               RTOS_MICRODEF(CheckAdQueueSpacesAvailable)

#define rtosEventosGetMsg                         RTOS_MICRODEF(rtosEventosGetMsg)
#define rtosEventosSendMsg                        RTOS_MICRODEF(rtosEventosSendMsg)
#define rtosTakeMsgFromEventos                    RTOS_MICRODEF(rtosTakeMsgFromEventos)
#define rtosDeliverMsgToEventos                   RTOS_MICRODEF(rtosDeliverMsgToEventos)
#define TakeEventosMsgQueueMutex                  RTOS_MICRODEF(TakeEventosMsgQueueMutex)
#define ReleaseEventosMsgQueueMutex               RTOS_MICRODEF(ReleaseEventosMsgQueueMutex)
#define TakeEventosMutex                          RTOS_MICRODEF(TakeEventosMutex)
#define ReleaseEventosMutex                       RTOS_MICRODEF(ReleaseEventosMutex)
#define CheckEventRspMesgNum                      RTOS_MICRODEF(CheckEventRspMesgNum)
#define PeekEventRspMesg                          RTOS_MICRODEF(PeekEventRspMesg)
#define CheckEventQueueSpacesAvailable            RTOS_MICRODEF(CheckEventQueueSpacesAvailable)

#define rtosMediaGetMsg                           RTOS_MICRODEF(rtosMediaGetMsg)
#define rtosMediaSendMsg                          RTOS_MICRODEF(rtosMediaSendMsg)
#define rtosTakeMsgFromMedia                      RTOS_MICRODEF(rtosTakeMsgFromMedia)
#define rtosDeliverMsgToMedia                     RTOS_MICRODEF(rtosDeliverMsgToMedia)
#define TakeMediaMutex                            RTOS_MICRODEF(TakeMediaMutex)
#define ReleaseMediaMsgQueueMutex                 RTOS_MICRODEF(ReleaseMediaMsgQueueMutex)
#define CheckMediaRspMesgNum                      RTOS_MICRODEF(CheckMediaRspMesgNum)
#define CheckMediaReqMesgNum                      RTOS_MICRODEF(CheckMediaReqMesgNum)
#define PeekMediaRspMesg                          RTOS_MICRODEF(PeekMediaRspMesg)
#define CheckMediaQueueSpacesAvailable            RTOS_MICRODEF(CheckMediaQueueSpacesAvailable)

#endif // ENTRY_RTOS_API_H_
