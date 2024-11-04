
#ifndef _EVENLIST_H
#define _EVENLIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../Inc/projDefine.h"
#include "../../../Inc/typedef.h"

// 定义 X 宏
#define X(func, priority, needRsp) \
    {#func, func, priority, needRsp},



extern void led_action(void* argv) ;
extern void battery_check(void* argv) ;

// X-Macro 表
#define REGISTER_ENTRIES \
    X(battery_check, 1, true) \
    X(led_action, 1, false) 

#ifdef __cplusplus
}
#endif
#endif
