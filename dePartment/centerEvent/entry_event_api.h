
#ifndef ENTRY_BUSINESS_API_H_
#define ENTRY_BUSINESS_API_H_
#include "typedef.h"
#include "ProjDefine.h"
#include "include.h"

// 函数指针类型的定义
#ifdef __USE_EVENTOS_
typedef void (T_TYPEDEF(eventOS_Init))(void);
typedef void (T_TYPEDEF(eventKernelStart))(void);
#else
typedef void (T_TYPEDEF(Event_Process))(void);
#endif
typedef time_t (T_TYPEDEF(SendMessacgeToEventos_needSample))(messeage_need_sample_t* need_sample,time_t wait);
typedef BOOL (T_TYPEDEF(GetRspMessacgeToEventos_needSample))(time_t ID,time_t wait);
// 输出函数接口结构体
typedef struct
{
    #ifdef __USE_EVENTOS_
    T_STRUCT_MEMBER(eventOS_Init);
    T_STRUCT_MEMBER(eventKernelStart);
    #else
    T_STRUCT_MEMBER(Event_Process);
    #endif
    T_STRUCT_MEMBER(SendMessacgeToEventos_needSample);
    T_STRUCT_MEMBER(GetRspMessacgeToEventos_needSample);
/*-----------------------------------*/
} tEVENTEntry;

// 作为一个指向结构体的指针
extern const tEVENTEntry entry_event_list;
//  指向 函数指针
#define event_api (&entry_event_list)
#define EVENT_MICRODEF(name) (event_api->t_##name)
/*-----------------------------------*/
#ifdef __USE_EVENTOS_
#define eventOS_Init           EVENT_MICRODEF(eventOS_Init)
#define eventKernelStart        EVENT_MICRODEF(eventKernelStart)
#else
#define Event_Process          EVENT_MICRODEF(Event_Process)
#endif
#define SendMessacgeToEventos_needSample          EVENT_MICRODEF(SendMessacgeToEventos_needSample)
#define GetRspMessacgeToEventos_needSample      EVENT_MICRODEF(GetRspMessacgeToEventos_needSample)

#endif // API_H_
