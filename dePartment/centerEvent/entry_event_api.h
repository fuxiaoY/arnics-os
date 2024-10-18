
#ifndef ENTRY_BUSINESS_API_H_
#define ENTRY_BUSINESS_API_H_
#include "../../Inc/ProjDefine.h"
#include "../../Inc/typedef.h"
#include "../../Inc/include.h"

// 函数指针类型的定义
typedef void (T_TYPEDEF(Event_Process))(void);

//typedef time_t (T_TYPEDEF(SendMessacgeToEventos_needSample))(messeage_need_sample_t* need_sample,time_t wait);
//typedef BOOL (T_TYPEDEF(GetRspMessacgeToEventos_needSample))(time_t ID,time_t wait);
// 输出函数接口结构体
typedef struct
{

    T_STRUCT_MEMBER(Event_Process);
    //T_STRUCT_MEMBER(SendMessacgeToEventos_needSample);
    //T_STRUCT_MEMBER(GetRspMessacgeToEventos_needSample);
/*-----------------------------------*/
} tEVENTEntry;

// 作为一个指向结构体的指针
extern const tEVENTEntry entry_event_list;
//  指向 函数指针
#define event_api (&entry_event_list)
#define EVENT_MICRODEF(name) (event_api->t_##name)
/*-----------------------------------*/

#define Event_Process          EVENT_MICRODEF(Event_Process)
//#define SendMessacgeToEventos_needSample          EVENT_MICRODEF(SendMessacgeToEventos_needSample)
//#define GetRspMessacgeToEventos_needSample      EVENT_MICRODEF(GetRspMessacgeToEventos_needSample)

#endif // API_H_
