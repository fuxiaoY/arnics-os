#ifndef GLOBAL_DEF_H
#define GLOBAL_DEF_H
#include "../inc/typedef.h"
#include "../inc/projDefine.h"

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------系统数据平台------------------------------------*/


/*---------------------------------------------------------------------------------------*/

/*----------------------------------------行政中心----------------------------------------*/

typedef struct 
{
    time_t ID_Ts;                        //消息ID，可用于检查区分消息传送
    uint32_t sleepTime_ms;
} adMessage_t;

typedef enum
{
    AD_SLEEP,                             //休假员
} administrativePart_t;

/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------事件中心----------------------------------------*/


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

/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------媒体中心----------------------------------------*/
typedef struct {
    time_t ID_Ts;                         //消息ID，可用于检查区分消息传送
    char buf[20];                         // 可以根据需要添加更多的字段
    int length;                           // 数据长度
} mediaMessage_t;


//MQTT主题
typedef struct 
{
  char upload[128];             //上行推送主题，最长不得超过128字节
  
  char command[128];            //下行订阅主题，最长不得超过128字节
  char response[128];           //上行应答主题，最长不得超过128字节
  
  char config_up[128];          //上行配置主题，最长不得超过128字节
  char config_down[128];        //下行配置主题，最长不得超过128字节
}mqttTopic_t;
/*---------------------------------------------------------------------------------------*/

/*----------------------------------------决策层------------------------------------------*/

// 设备运行状态
typedef enum 
{
  WORK_STATUS_INIT = 0,                     // 上电初始化
}wisdomStackPart_t;


/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------配置数据---------------------------------------*/
//设备运行参数集，存放于片上Flash，同步一份到内存中常驻
typedef struct 
{
  uint32_t save_ts;                       //保存时间戳，同时也用于判定Flash上保存的参数集是否有效
  uint16_t crc;                          //校验位

}systemCfg_t;
#define SYS_CFG_SIZE	sizeof(systemCfg_t)
/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------运行数据----------------------------------------*/
//设备运行状态记录集，常驻内存中，同步存放一份到片上Flash，复位重启后读回来作为缺省状态
typedef struct 
{

  uint32_t save_ts;				            //保存时间戳
  wisdomStackPart_t work_status;			//当前运行状态 
  wisdomStackPart_t prework_status;		//上次的运行状态
  uint16_t crc;                         //校验位

}systemStatus_t;
#define SYS_STATUS_SIZE		sizeof(systemStatus_t)

#ifdef __cplusplus
}
#endif

#endif
