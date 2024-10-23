
#ifndef ENTRY_EVENT_API_H_
#define ENTRY_EVENT_API_H_
#include "../../Inc/ProjDefine.h"
#include "../../Inc/typedef.h"


// 函数指针类型的定义
typedef void (T_TYPEDEF(event_process))(void);
typedef uint32_t (T_TYPEDEF(SendEventCallToEventCenter))(uint32_t* eventflag,time_t wait);
typedef bool (T_TYPEDEF(GetResponseMessageFromEventCenter))(time_t ID,time_t wait);
typedef bool (T_TYPEDEF(set_event_flag))(uint32_t *eventflag, const char *name) ;
// 输出函数接口结构体
typedef struct
{

    T_STRUCT_MEMBER(event_process);
    T_STRUCT_MEMBER(SendEventCallToEventCenter);
    T_STRUCT_MEMBER(GetResponseMessageFromEventCenter);    
    T_STRUCT_MEMBER(set_event_flag);
/*-----------------------------------*/
} tEVENTEntry;

// 作为一个指向结构体的指针
extern const tEVENTEntry entry_event_list;
//  指向 函数指针
#define event_api (&entry_event_list)
#define EVENT_MICRODEF(name) (event_api->t_##name)
/*-----------------------------------*/

#define event_process                          EVENT_MICRODEF(event_process)
#define SendEventCallToEventCenter             EVENT_MICRODEF(SendEventCallToEventCenter)
#define GetResponseMessageFromEventCenter      EVENT_MICRODEF(GetResponseMessageFromEventCenter)
#define set_event_flag                         EVENT_MICRODEF(set_event_flag)

#endif // API_H_
