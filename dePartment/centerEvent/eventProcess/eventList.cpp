#include "eventCore.h"
#pragma arm section code="._entry_event_api"
/*
* 用户自行管理是否回复事件
*/
//用户事件
RegisterEntry registerTable[] = 
{
    {"led_action", led_action, EVENT_TAG, 1,false},
    {"battery_check", battery_check, EVENT_TAG, 1,true}
};






// 定义函数
void led_action(void) 
{
    // 函数实现
}

void battery_check(void) 
{
    // 函数实现
}


