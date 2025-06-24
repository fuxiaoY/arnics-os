#ifndef _CENTER_EVENT_H_
#define _CENTER_EVENT_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/typedef.h"
#include "Inc/ProjDefine.h"


#define MAX_MESSAGE_LENGTH 40
typedef struct 
{
    uint8_t reserved;
} NULL_msg_t;

typedef struct 
{
    uint8_t a;
    uint8_t b;
} led0_toggle_t;
typedef union 
{
    /*req*/
    led0_toggle_t led0_toggle_req;
    /*rsp*/
    led0_toggle_t led0_toggle_rsp;
}led0_toggle_msg_t;
/*---------------------------------------*/
typedef struct 
{
    uint32_t total_blink_count;
} led1_action_t;
typedef union 
{
    /*req*/
    uint32_t req_blink_count;
    /*rsp*/
    led1_action_t led1_action_rsp;
}led1_action_msg_t;
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
    /* led0_toggle员工函数 */
    led0_toggle_msg_t led0_toggle_msg;  
    /*--------------------*/
    /*--------------------*/
    /* led1_actionk员工函数 */
    led1_action_msg_t led1_action_msg;
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
