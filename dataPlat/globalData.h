#ifndef _GLOBALDATA_H_
#define _GLOBALDATA_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "globalDef.h"
#include "../thirdParty/modContact/mctInclude.h"
/*-------------------------------------------------------------------------------------*/

/*系统参数-----------------------------------------------------------------------------*/
extern bool arnics_start;
extern volatile uint32_t arnics_systick;
extern uint8_t VirtualEnvironment;  
extern bool eventosWantSleep;
extern bool BusinessWantSleep;
extern bool MediaWantSleep;
extern bool GuardWantSleep;
extern bool ConsoleWantSleep;
// 系统是否允许休眠标志位
extern bool ALLOWSLEEP;
/*-------------------------------------------------------------------------------------*/

/*运行与配置参数------------------------------------------------------------------------*/
extern SytemCfg global_cfg;       // 全局配置参数
extern SytemState global_state;   // 全局运行参数
extern ST_MQTopic g_MQTopic;	  // MQTT主题集
extern NetworkPara_t networkPara; // 媒体中心网络参数
extern char mqttWanBuf[256];       // MQTT远程帧缓冲区
extern bool mqttWanBufReady;
extern MqttPulish_t mqttWanPub;

#ifdef __cplusplus
}
#endif
#endif
