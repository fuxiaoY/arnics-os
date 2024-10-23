
#ifndef _EVENTCORE_H
#define _EVENTCORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../Inc/projDefine.h"
#include "../../../Inc/typedef.h"
#include "../../../dataPlat/entry_dataPlat_api.h"

#define EVENT_MAX_NUM 32

// 宏定义，用于清除事件标志 表示将处理过的所有事件无效化
#define CLR_EVENT_FLAG_ALL(pEvent_) \
    do                              \
    {                               \
        (pEvent_)= 0x00;     \
    } while (0)


typedef struct {
    const char* name;
    void (*func)(void);
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



extern Message_t mesg_cache;               //事件应用消息

extern time_t SendEventFlagToEventCenter(uint32_t *eventflag, time_t wait);
extern bool GetResponseMessageFromEventCenter(time_t ID, time_t wait);
extern bool set_event_flag(uint32_t *eventflag, const char *name) ;
extern void event_process();


#ifdef __cplusplus
}
#endif
#endif
