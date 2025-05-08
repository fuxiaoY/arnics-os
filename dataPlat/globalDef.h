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
    uint8_t a;
    uint8_t b;
} led0_toggle_t;


typedef struct 
{
    uint32_t total_blink_count;
} led1_action_t;


typedef union 
{
    /*--------------------*/
    /* led0_toggle员工函数 */
    /*req*/
    led0_toggle_t led0_toggle_req;
    /*rsp*/
    led0_toggle_t led0_toggle_rsp;
    /*--------------------*/
    /*--------------------*/
    /* led1_actionk员工函数 */
    /*req*/
    uint32_t req_blink_count;
    /*rsp*/
    led1_action_t led1_action_rsp;
    /*--------------------*/
} messge_deliver_t;
typedef struct 
{
    SMALLOC(messge_deliver_t, message_deliver, MAX_MESSAGE_LENGTH);
} MessageUnion;



typedef struct 
{
    time_t ID_Ts;                         //消息ID，可用于检查区分消息传送 
    uint32_t eventflag;                   //事件
    char buf[MAX_MESSAGE_LENGTH];         // 可以根据需要添加更多的字段
    int length;                           // 数据长度
} Message_t;

/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------媒体中心----------------------------------------*/
typedef struct {
    time_t ID_Ts;                         //消息ID，可用于检查区分消息传送
    char buf[20];                         // 可以根据需要添加更多的字段
    int length;                           // 数据长度
} MediaMessage_t;

/*---------------------------------------------------------------------------------------*/

/*----------------------------------------决策层------------------------------------------*/

// 设备运行状态
typedef enum 
{
  WORKSTAT_INIT = 0,                     // 上电初始化
}wisdomStackPart_t;


/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------配置数据---------------------------------------*/
//设备运行参数集，存放于片上Flash，同步一份到内存中常驻
typedef struct 
{
  uint32_t SaveTs;                       //保存时间戳，同时也用于判定Flash上保存的参数集是否有效
  uint16_t crc;                          //校验位

}SytemCfg;
#define SytemCfg_SIZE	sizeof(SytemCfg)
/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------运行数据----------------------------------------*/
//设备运行状态记录集，常驻内存中，同步存放一份到片上Flash，复位重启后读回来作为缺省状态
typedef struct 
{
  uint32_t SaveTs;				        //保存时间戳
  wisdomStackPart_t WorkStat;			//当前运行状态 
  wisdomStackPart_t PreWorkStat;		//上次的运行状态
  uint16_t crc;                         //校验位

}SytemState;
#define SytemState_SIZE		sizeof(SytemState)

#ifdef __cplusplus
}
#endif

#endif
