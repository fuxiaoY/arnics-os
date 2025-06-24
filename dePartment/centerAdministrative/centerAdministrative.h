#ifndef _CENTER_ADMINISTRATIVE_H_
#define _CENTER_ADMINISTRATIVE_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/typedef.h"
#include "Inc/ProjDefine.h"


typedef struct 
{
    time_t ID_Ts;                        //消息ID，可用于检查区分消息传送
    uint32_t sleepTime_ms;
} adMessage_t;

typedef enum
{
    AD_SLEEP,                             //休假员
} administrativePart_t;

#ifdef __cplusplus
}
#endif
#endif
