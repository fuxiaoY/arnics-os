#ifndef ENTRY_RTOS_API_H_
#define ENTRY_RTOS_API_H_
#include "../Inc/ProjDefine.h"
#include "../Inc/typedef.h"
#ifdef _USE_FREERTOS_
#define BLOCK_DELAY 0xffffffffUL
#endif

// 函数指针类型的定义
typedef bool (T_TYPEDEF(rtosEventosGetMsg))(void *msg, uint32_t delay);
typedef bool (T_TYPEDEF(rtosEventosSendMsg))(void *msg, uint32_t delay);
typedef bool (T_TYPEDEF(rtosTakeMsgFromEventos))(void *msg, uint32_t delay);
typedef bool (T_TYPEDEF(rtosDeliverMsgToEventos))(void *msg, uint32_t delay);
typedef void (T_TYPEDEF(rtosThreadDelayUntil))(uint32_t ms);
typedef void (T_TYPEDEF(rtosThreadDelay))(uint32_t ms);
typedef bool (T_TYPEDEF(TakeEventosMsgQueueMutex))(time_t waitTime);
typedef void (T_TYPEDEF(ReleaseEventosMsgQueueMutex))(void);
typedef bool (T_TYPEDEF(TakeEventosMutex))(time_t waitTime);
typedef void (T_TYPEDEF(ReleaseEventosMutex))(void);
typedef uint32_t (T_TYPEDEF(CheckEventRspMesgNum))(void);
typedef bool (T_TYPEDEF(PeekEventRspMesg))(void *receivedMsg);
typedef uint32_t (T_TYPEDEF(CheckMediaRspMesgNum))(void);
typedef bool (T_TYPEDEF(TakeMediaMutex))(time_t waitTime);
typedef bool (T_TYPEDEF(rtosMediaGetMsg))(void *msg, uint32_t delay);
typedef void (T_TYPEDEF(ReleaseMediaMsgQueueMutex))(void);
typedef bool (T_TYPEDEF(CheckMediaQueueSpacesAvailable))(void);
typedef bool (T_TYPEDEF(rtosDeliverMsgToMedia))(void *msg, uint32_t delay);

// 输出函数接口结构体
typedef struct
{
    T_STRUCT_MEMBER(rtosEventosGetMsg);
    T_STRUCT_MEMBER(rtosEventosSendMsg);
    T_STRUCT_MEMBER(rtosTakeMsgFromEventos);
    T_STRUCT_MEMBER(rtosDeliverMsgToEventos);
    T_STRUCT_MEMBER(rtosThreadDelayUntil);
    T_STRUCT_MEMBER(rtosThreadDelay);
    T_STRUCT_MEMBER(TakeEventosMsgQueueMutex);
    T_STRUCT_MEMBER(ReleaseEventosMsgQueueMutex);
    T_STRUCT_MEMBER(TakeEventosMutex);
    T_STRUCT_MEMBER(ReleaseEventosMutex);
    T_STRUCT_MEMBER(CheckEventRspMesgNum);
    T_STRUCT_MEMBER(PeekEventRspMesg);
    T_STRUCT_MEMBER(CheckMediaRspMesgNum);
    T_STRUCT_MEMBER(TakeMediaMutex);
    T_STRUCT_MEMBER(rtosMediaGetMsg);
    T_STRUCT_MEMBER(ReleaseMediaMsgQueueMutex);
    T_STRUCT_MEMBER(CheckMediaQueueSpacesAvailable);
    T_STRUCT_MEMBER(rtosDeliverMsgToMedia);

} tRTOSEntry;

// 作为一个指向结构体的指针
extern const tRTOSEntry entry_rtos_list;
//  指向 函数指针
#define rtos_api (&entry_rtos_list)
#define RTOS_MICRODEF(name) (rtos_api->t_##name)

// 宏定义
#define rtosEventosGetMsg                         RTOS_MICRODEF(rtosEventosGetMsg)
#define rtosEventosSendMsg                        RTOS_MICRODEF(rtosEventosSendMsg)
#define rtosTakeMsgFromEventos                    RTOS_MICRODEF(rtosTakeMsgFromEventos)
#define rtosDeliverMsgToEventos                   RTOS_MICRODEF(rtosDeliverMsgToEventos)
#define rtosThreadDelayUntil                      RTOS_MICRODEF(rtosThreadDelayUntil)
#define rtosThreadDelay                           RTOS_MICRODEF(rtosThreadDelay)
#define TakeEventosMsgQueueMutex                  RTOS_MICRODEF(TakeEventosMsgQueueMutex)
#define ReleaseEventosMsgQueueMutex               RTOS_MICRODEF(ReleaseEventosMsgQueueMutex)
#define TakeEventosMutex                          RTOS_MICRODEF(TakeEventosMutex)
#define ReleaseEventosMutex                       RTOS_MICRODEF(ReleaseEventosMutex)
#define CheckEventRspMesgNum                      RTOS_MICRODEF(CheckEventRspMesgNum)
#define PeekEventRspMesg                          RTOS_MICRODEF(PeekEventRspMesg)
#define CheckMediaRspMesgNum                      RTOS_MICRODEF(CheckMediaRspMesgNum)
#define TakeMediaMutex                            RTOS_MICRODEF(TakeMediaMutex)
#define rtosMediaGetMsg                           RTOS_MICRODEF(rtosMediaGetMsg)
#define ReleaseMediaMsgQueueMutex                 RTOS_MICRODEF(ReleaseMediaMsgQueueMutex)
#define CheckMediaQueueSpacesAvailable            RTOS_MICRODEF(CheckMediaQueueSpacesAvailable)
#define rtosDeliverMsgToMedia                     RTOS_MICRODEF(rtosDeliverMsgToMedia)

#endif // ENTRY_RTOS_API_H_
