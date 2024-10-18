
#ifndef _EVENTCORE_H
#define _EVENTCORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../Inc/projDefine.h"
#include "../../../Inc/typedef.h"
#include "../../../core/coreInclude.h"
#pragma arm section code="._entry_event_api"


#define EVENT_TAG "event"

// 宏定义，用于设置事件标志 表示事件已完成 提交报表
#define SET_EVENT_FLAG(pEvent_, NAME_) \
    do                                 \
    {                                  \
        (pEvent_)->BITS.F_##NAME_ = 1; \
    } while (0)
// 宏定义，用于清除事件标志 表示将处理过的事件无效化
#define CLR_EVENT_FLAG(pEvent_, NAME_) \
    do                                 \
    {                                  \
        (pEvent_)->BITS.F_##NAME_ = 0; \
    } while (0)

// 宏定义，用于判断事件标志是否被置位  
#define IS_EVENT_FLAG_RISE(pEvent_, NAME_) \
    ((pEvent_)->BITS.F_##NAME_ == 1 ? true : false)

// 宏定义，用于清除事件标志 表示将处理过的所有事件无效化
#define CLR_EVENT_FLAG_ALL(pEvent_) \
    do                              \
    {                               \
        (pEvent_)->byte = 0x00;     \
    } while (0)

typedef enum
{
    OnWattingOutMsg = 1,// 等待外部消息
    ActionMsg,          // 执行命令
    SendingRspMsg,      // 返回外部消息的执行结果
}EVENT_STATE;


extern EVENT_STATE event_state;            // 状态机状态
extern time_t ID_Ts;                       // 外部消息id



extern void onWaittingOutMessage();
extern void eventAction();
extern void onSendingMessage();

#ifdef __cplusplus
}
#endif
#endif