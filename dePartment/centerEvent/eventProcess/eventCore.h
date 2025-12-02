

/* information */
/**
  ******************************************************************************
  * @file           : EventCore.h
  * @brief          : 事件核心相关的头文件
  * 
  * 该文件定义了事件核心的相关宏定义、结构体和函数声明。
  * 主要用于事件的管理和处理。
  * 
  * @version        : 1.0.2
  * @date           : 2025-06-19
  ******************************************************************************
  * @attention
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
#ifndef _EVENTCORE_H
#define _EVENTCORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#include "dataPlat/dataPlatInclude.h"
#include "core/coreInclude.h"
#include "Common/commonInclude.h"
#include "dePartment/centerEvent/centerEvent.h"
/* define ------------------------------------------------------------*/

#define EVENT_VERSION "1.0.2"
// 宏定义，用于清除事件标志 表示将处理过的所有事件无效化
#define CLR_EVENT_FLAG_ALL(pEvent_) \
    do                              \
    {                               \
        (pEvent_)= 0x00;     \
    } while (0)
    
// 判断事件标志是否清除
#define IS_EVENT_FLAG_CLR(pEvent_) \
    ((pEvent_ == 0x00) ? true : false) 
/* typedef -----------------------------------------------------------*/
typedef struct {
    const char* name;
    void (*func)(void* argv);
    int employ_kind;
    size_t msg_struct_offset;
    size_t msg_struct_size;
    bool needRsp;
} RegisterEntry;

// 定义事件位映射结构体
typedef struct {
    const char* name;
    int employ_kind;
    size_t msg_struct_offset;
    size_t msg_struct_size;
    uint32_t event_bit;
    bool needRsp;
    //status
    bool is_running;
    void* task_argv;
} EventBitMapping;



typedef enum
{
    OnWattingOutMsg = 1,// 等待外部消息
    ActionMsg,          // 执行命令
    SendingRspMsg,      // 返回外部消息的执行结果
}EVENT_STATE;
/* define ------------------------------------------------------------*/
#define PT_END_NORETURN(pt) LC_END((pt)->lc); PT_YIELD_FLAG = 0; \
                   PT_INIT(pt); }
// 封装 _START 和 _END
#define EVET_START static struct pt _pt_##__func__ = {0}; struct pt *ppt = &_pt_##__func__; PT_BEGIN(ppt); static uint32_t _delay_start_ = 0; _delay_start_ = arnics_getTick(); 
#define EVET_END PT_END_NORETURN(ppt);


#define EV_WAIT_UNTIL(pt, condition) \
  do {						         \
    LC_SET((pt)->lc);				 \
    if(!(condition)) {			 	 \
      return ;			             \
    }      \
    else   \
    {      \
        _delay_start_ = arnics_getTick(); \
    }						\
  } while(0)
#define _OUT_IF_TRUE(pt, cond)  EV_WAIT_UNTIL((pt), !(cond))
// 事件延时只允许在内部员工中使用，外部员工使用将直接退出
#define EVET_DELAY(ms)  _OUT_IF_TRUE(ppt, (uint32_t)(arnics_getTick() - _delay_start_) < (ms)); 

/* variables ---------------------------------------------------------*/
extern message_t mesg_cache;               //事件应用消息
/* function declaration ---------------------------------------------*/
extern uint32_t SendEventCallToEventCenter(eventFlag_t eventflag,time_t wait);
extern bool GetResponseMessageFromEventCenter(time_t ID, time_t wait,void *argv);
/**
 * @brief  设置事件标志为指定的事件，清除之前所有事件。
 *
 *         根据事件名称查找对应的索引，并将 eventflag 设置为仅包含该事件。
 *         如果 `ismsg` 为 true，则同时设置 msg_flag。
 *
 * @param[in,out] eventflag  指向事件标志结构体的指针。
 * @param[in]     name       要设置的事件名称（必须存在于注册表中）。
 * @param[in]     ismsg      是否将该事件标记为需要消息传递。
 *
 * @return bool              成功设置事件返回 true；
 *                           如果未找到对应事件或参数无效则返回 false。
 *
 * @note   清空 eventflag 和 msg_flag，然后仅设置指定的事件。
 *
 */
extern bool add_event_flag(eventFlag_t *eventflag, const char *name,bool ismsg) ;
/**
 * @brief  向指定的事件标志中添加一个事件。
 *
 *         根据事件名称查找对应的索引，并将其添加到 eventflag 中。
 *         如果 `ismsg` 为 true，则同时将该事件标记为需要消息传递。
 *
 * @param[in,out] eventflag  指向事件标志结构体的指针。
 * @param[in]     name       要添加的事件名称（必须存在于注册表中）。
 * @param[in]     ismsg      是否将该事件标记为需要消息传递。
 *
 * @return bool              成功添加事件返回 true；
 *                           如果未找到对应事件或参数无效则返回 false。
 *
 * @note   该函数不会清除原有的事件标志，仅在原有基础上添加新事件。
 *
 */
extern bool set_event_flag(eventFlag_t *eventflag, const char *name,bool ismsg) ;
extern void event_process();


#ifdef __cplusplus
}
#endif
#endif
/************************ (C) COPYRIGHT ARSTUDIO *****END OF FILE***************************/
