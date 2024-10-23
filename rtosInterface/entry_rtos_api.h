#ifndef ENTRY_RTOS_API_H_
#define ENTRY_RTOS_API_H_
#include "../Inc/ProjDefine.h"
#include "../Inc/typedef.h"
#ifdef _USE_FREERTOS_
#define BLOCK_DELAY 0xffffffffUL
#endif

// 函数指针类型的定义
typedef bool (T_TYPEDEF(rtosEventQueueReceive))(void *msg, uint32_t delay);
typedef bool (T_TYPEDEF(rtosEventQueueSend))(void *msg, uint32_t delay);
typedef bool (T_TYPEDEF(rtosEventQueueTake))(void *msg, uint32_t delay);
typedef void (T_TYPEDEF(rtosThreadDelay))(uint32_t ms);
typedef bool (T_TYPEDEF(TakeEventosMsgQueueMutex))(time_t waitTime);
typedef void (T_TYPEDEF(ReleaseEventosMsgQueueMutex))(void);
typedef bool (T_TYPEDEF(TakeEventosMutex))(time_t waitTime);
typedef void (T_TYPEDEF(ReleaseEventosMutex))(void);
typedef uint32_t (T_TYPEDEF(CheckEventRspMesgNum))(void);
typedef bool (T_TYPEDEF(PeekEventRspMesg))(void *receivedMsg);

// 输出函数接口结构体
typedef struct
{
    T_STRUCT_MEMBER(rtosEventQueueReceive);
    T_STRUCT_MEMBER(rtosEventQueueSend);
    T_STRUCT_MEMBER(rtosEventQueueTake);
    T_STRUCT_MEMBER(rtosThreadDelay);
    T_STRUCT_MEMBER(TakeEventosMsgQueueMutex);
    T_STRUCT_MEMBER(ReleaseEventosMsgQueueMutex);
    T_STRUCT_MEMBER(TakeEventosMutex);
    T_STRUCT_MEMBER(ReleaseEventosMutex);
    T_STRUCT_MEMBER(CheckEventRspMesgNum);
    T_STRUCT_MEMBER(PeekEventRspMesg);
} tRTOSEntry;

// 作为一个指向结构体的指针
extern const tRTOSEntry entry_rtos_list;
//  指向 函数指针
#define rtos_api (&entry_rtos_list)
#define RTOS_MICRODEF(name) (rtos_api->t_##name)

// 宏定义
#define rtosEventQueueReceive                     RTOS_MICRODEF(rtosEventQueueReceive)
#define rtosEventQueueSend                        RTOS_MICRODEF(rtosEventQueueSend)
#define rtosEventQueueTake                        RTOS_MICRODEF(rtosEventQueueTake)
#define rtosThreadDelay                           RTOS_MICRODEF(rtosThreadDelay)
#define TakeEventosMsgQueueMutex                  RTOS_MICRODEF(TakeEventosMsgQueueMutex)
#define ReleaseEventosMsgQueueMutex               RTOS_MICRODEF(ReleaseEventosMsgQueueMutex)
#define TakeEventosMutex                          RTOS_MICRODEF(TakeEventosMutex)
#define ReleaseEventosMutex                       RTOS_MICRODEF(ReleaseEventosMutex)
#define CheckEventRspMesgNum                      RTOS_MICRODEF(CheckEventRspMesgNum)
#define PeekEventRspMesg                          RTOS_MICRODEF(PeekEventRspMesg)

#endif // ENTRY_RTOS_API_H_
