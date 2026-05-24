#ifndef _CENTER_EVENT_H_
#define _CENTER_EVENT_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/typedef.h"
#include "Inc/projDefine.h"

#ifndef EVENT_MAX_EMPLOYEES
#define EVENT_MAX_EMPLOYEES 128U
#endif

#define EVENT_FLAG_WORD_BITS 32U
#define EVENT_FLAG_WORDS ((EVENT_MAX_EMPLOYEES + EVENT_FLAG_WORD_BITS - 1U) / EVENT_FLAG_WORD_BITS)

typedef struct
{
    uint32_t words[EVENT_FLAG_WORDS];
} eventBits_t;


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
  eventBits_t event_flag;
  eventBits_t msg_flag;
  messageUnion_u msg;
}eventFlag_t;

#define SET_MESSAGE(para,lambda)  para.msg.message_deliver.lambda; 

typedef struct 
{
    time_t ID_Ts;                         //消息ID，可用于检查区分消息传送 
    eventBits_t eventflag;                //事件
    eventBits_t msgflag;                  //消息
    messageUnion_u message_union;
} message_t;

#ifdef __cplusplus
}
#endif
#endif
