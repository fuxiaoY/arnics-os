/* information */
/**
  ******************************************************************************
  * @file           : EeventList.h
  * @brief          : 事件列表相关的头文件
  * 
  * 
  * 主要用于员工函数注册。
  * 
  * @version        : 1.0.0
  * @date           : 2024-11-07
  ******************************************************************************
  * @attention
  *   内部员工函数： 由事件中心内核按照注册表顺序自动loop，收到相应外部需求后执行一次带参响应
  *   外部员工函数： 消息驱动型调用，收到相应外部需求后带参响应
  * 
  * Copyright (c) 2023 ARSTUDIO.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* information */
#ifndef _EVENLIST_H
#define _EVENLIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../Inc/projDefine.h"
#include "../../../Inc/typedef.h"

#define EVENT_INTERNAL_EMPLOY 1
#define EVENT_EXTERNAL_EMPLOY 2

// 定义 X 宏
#define X(func, priority, needRsp) \
    {#func, func, priority, needRsp},



extern void led0_toggle(void* argv) ;
extern void led1_action(void* argv) ;

// X-Macro 表
#define REGISTER_ENTRIES \
    X(NULL, EVENT_EXTERNAL_EMPLOY, false) \
    X(led0_toggle, EVENT_EXTERNAL_EMPLOY, true) \
    X(led1_action, EVENT_EXTERNAL_EMPLOY, true) 

#ifdef __cplusplus
}
#endif
#endif
/************************ (C) COPYRIGHT ARSTUDIO *****END OF FILE***************************/
