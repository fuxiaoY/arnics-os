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
extern uint8_t virtual_environment;  
extern bool eventos_want_sleep;
extern bool business_want_sleep;
extern bool media_want_sleep;
extern bool guard_want_sleep;
extern bool console_want_sleep;
// 系统是否允许休眠标志位
extern bool sys_allow_sleep;
/*-------------------------------------------------------------------------------------*/

/*运行与配置参数------------------------------------------------------------------------*/
extern systemCfg_t g_system_cfg;       // 全局配置参数
extern systemStatus_t g_system_status; // 全局运行参数
extern mqttTopic_t g_mqtt_topic;	   // MQTT主题集
extern NetworkPara_t networkPara;      // 媒体中心网络参数
extern char g_mqtt_wan_buf[256];       // MQTT远程帧缓冲区
extern bool is_mqtt_wan_buf_ready;
extern MqttPulish_t g_mqtt_publish;

#ifdef __cplusplus
}
#endif
#endif
