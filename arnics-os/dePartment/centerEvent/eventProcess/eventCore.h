

/* information */
/**
  ******************************************************************************
  * @file           : EventCore.h
  * @brief          : 事件核心相关的头文件
  * 
  * 该文件定义了事件核心的相关宏定义、结构体和函数声明。
  * 主要用于事件的管理和处理。
  * 
  * @version        : 1.0.0
  * @date           : 2023-10-01
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

#include "../../../Inc/projDefine.h"
#include "../../../Inc/typedef.h"
#include "../../../dataPlat/dataPlatInclude.h"
#include "../../../core/coreInclude.h"
#include "../../../Common/commonInclude.h"

/* define ------------------------------------------------------------*/
#define EVENT_MAX_NUM 32

// 宏定义，用于清除事件标志 表示将处理过的所有事件无效化
#define CLR_EVENT_FLAG_ALL(pEvent_) \
    do                              \
    {                               \
        (pEvent_)= 0x00;     \
    } while (0)

/* typedef -----------------------------------------------------------*/
typedef struct {
    const char* name;
    void (*func)(void* argv);
    int priority;
    bool needRsp;
} RegisterEntry;

// 定义事件位映射结构体
typedef struct {
    const char* name;
    uint32_t event_bit;
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
#define EVET_START static struct pt _pt_##__func__ = {0}; struct pt *ppt = &_pt_##__func__; PT_BEGIN(ppt); static uint32_t _delay_start_ = arnics_getTick(); 
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

#define EVET_DELAY(ms)  _OUT_IF_TRUE(ppt, (uint32_t)(arnics_getTick() - _delay_start_) < (ms)); 

/* variables ---------------------------------------------------------*/
extern Message_t mesg_cache;               //事件应用消息
/* function declaration ---------------------------------------------*/
extern uint32_t SendEventCallToEventCenter(uint32_t eventflag,void *argv,size_t len, time_t wait);
extern bool GetResponseMessageFromEventCenter(time_t ID, time_t wait,void *argv);
extern bool set_event_flag(uint32_t *eventflag, const char *name) ;
extern void event_process();


#ifdef __cplusplus
}
#endif
#endif
/************************ (C) COPYRIGHT ARSTUDIO *****END OF FILE***************************/
