#ifndef _CENTER_MEDIA_H_
#define _CENTER_MEDIA_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/typedef.h"
#include "Inc/ProjDefine.h"

typedef struct {
    time_t ID_Ts;                         //消息ID，可用于检查区分消息传送
    char buf[20];                         // 可以根据需要添加更多的字段
    int length;                           // 数据长度
} mediaMessage_t;


#ifdef __cplusplus
}
#endif
#endif
