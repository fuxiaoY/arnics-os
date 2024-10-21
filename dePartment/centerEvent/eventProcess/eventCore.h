
#ifndef _EVENTCORE_H
#define _EVENTCORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../Inc/projDefine.h"
#include "../../../Inc/typedef.h"
#include "../../../core/coreInclude.h"



#define EVENT_TAG "event"



// 宏定义，用于清除事件标志 表示将处理过的所有事件无效化
#define CLR_EVENT_FLAG_ALL(pEvent_) \
    do                              \
    {                               \
        (pEvent_)= 0x00;     \
    } while (0)

typedef uint32_t EVENT_BYTE_T // event事件处理标志

typedef struct {
    const char* name;
    void (*func)(void);
    int event_tag;
    int priority;
    bool needRsp;
} RegisterEntry;

// 定义事件位映射结构体
typedef struct {
    const char* name;
    EVENT_BYTE_T event_bit;
} EventBitMapping;

extern RegisterEntry registerTable[];

typedef enum
{
    OnWattingOutMsg = 1,// 等待外部消息
    ActionMsg,          // 执行命令
    SendingRspMsg,      // 返回外部消息的执行结果
}EVENT_STATE;


extern EVENT_STATE event_state;            // 状态机状态
extern time_t ID_Ts;                       // 外部消息id


bool set_event_flag(EVENT_BYTE_T *eventflag, const char *name) ;
extern void event_process();


#ifdef __cplusplus
}
#endif
#endif
