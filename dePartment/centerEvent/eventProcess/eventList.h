
#ifndef _EVENLIST_H
#define _EVENLIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../Inc/projDefine.h"
#include "../../../Inc/typedef.h"
#include "eventCore.h"
// 定义 X 宏
#define X(func, priority, needRsp) \
    {#func, func, priority, needRsp},



extern void led_action(void) ;
extern void battery_check(void) ;

// X-Macro 表
#define REGISTER_ENTRIES \
    X(led_action, 1, false) \
    X(battery_check, 1, true)


#ifdef __cplusplus
}
#endif
#endif
