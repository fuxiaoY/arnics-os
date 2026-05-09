#ifndef _CENTER_EVENT_H_
#define _CENTER_EVENT_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/typedef.h"
#include "Inc/projDefine.h"


#define MAX_MESSAGE_LENGTH 40
typedef struct 
{
    uint8_t reserved;
} NULL_msg_t;
/*---------------------------------------*/

/*---------------------------------------*/

/*
 * 事件中心员工函数命名规则：
 * 消息名遵循规则：<事件中心员工函数名>_msg
 * 
*/
typedef struct 
{
    NULL_msg_t NULL_msg;
    /*--------------------*/
    /* xxx员工函数 */

    /*--------------------*/

} messge_deliver_t;
typedef struct 
{
    SMALLOC(messge_deliver_t, message_deliver, MAX_MESSAGE_LENGTH);
} messageUnion_u;

typedef struct{
  uint32_t event_flag;
  uint32_t msg_flag;
  messageUnion_u msg;
}eventFlag_t;

#define SET_MESSAGE(para,lambda)  para.msg.message_deliver.lambda; 

typedef struct 
{
    time_t ID_Ts;                         //消息ID，可用于检查区分消息传送 
    uint32_t eventflag;                   //事件
    uint32_t msgflag;                     //消息
    messageUnion_u message_union;
} message_t;

#ifdef __cplusplus
}
#endif
#endif
