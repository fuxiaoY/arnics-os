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

typedef struct {
    time_t ID_Ts;    //消息ID，可用于检查区分消息传送
    uint32_t sleepTime_ms;
} adMessage_t;

/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------事件中心----------------------------------------*/


#define MAX_MESSAGE_LENGTH 40


typedef struct 
{
    uint8_t a;
    uint8_t b;
} test_req_t;


typedef struct 
{
    uint32_t a;
    uint32_t b;
} test_rsp_t;


typedef union 
{
    /* test员工函数 */
    test_req_t test_req;
    test_rsp_t test_rsp;

    /* check员工函数 */
    uint8_t check_req;
    uint32_t check_rsp;

} messge_deliver_t;
typedef struct 
{
    SMALLOC(messge_deliver_t, message_deliver, MAX_MESSAGE_LENGTH);
} MessageUnion;



typedef struct {
    time_t ID_Ts;    //消息ID，可用于检查区分消息传送 
    uint32_t eventflag; //事件
    char buf[MAX_MESSAGE_LENGTH];  // 可以根据需要添加更多的字段
    int length;     // 数据长度
} Message_t;

/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------媒体中心----------------------------------------*/
typedef struct {
    time_t ID_Ts;    //消息ID，可用于检查区分消息传送
    char buf[20];  // 可以根据需要添加更多的字段
    int length;     // 数据长度
} MediaMessage_t;

/*---------------------------------------------------------------------------------------*/

/*----------------------------------------决策层------------------------------------------*/

// 设备运行状态
typedef enum 
{
  WORKSTAT_INIT = 0,   // 上电初始化
}ENUM_WORKSTATE;


/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------配置数据---------------------------------------*/
//设备运行参数集，存放于片上Flash，同步一份到内存中常驻
typedef struct 
{
  uint32_t SaveTs;					//保存时间戳，同时也用于判定Flash上保存的参数集是否有效

  //校验位
  uint16_t crc;    

}SytemCfg;
#define SytemCfg_SIZE	sizeof(SytemCfg)
/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------运行数据----------------------------------------*/
//设备运行状态记录集，常驻内存中，同步存放一份到片上Flash，复位重启后读回来作为缺省状态
typedef struct 
{
  uint32_t SaveTs;				//保存时间戳，同时也用于判定Flash上保存的参数集是否有效

  ENUM_WORKSTATE WorkStat;			//当前运行状态 参考ENUM_WORKSTATE定义
  ENUM_WORKSTATE PreWorkStat;		//上次的运行状态 参考ENUM_WORKSTATE定义


  //校验位
  uint16_t crc;    

}SytemState;
#define SytemState_SIZE		sizeof(SytemState)

#ifdef __cplusplus
}
#endif

#endif
