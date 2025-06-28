

#include "systemStatus.h"
#include "port/arnicsPort.h"

systemStatus_t g_system_status = {0};       // 全局运行参数


mqttTopic_t g_mqtt_topic = {0};	      //MQTT主题集
networkPara_t networkPara;         // 媒体中心网络参数
char g_mqtt_wan_buf[256] = {0};       // MQTT远程帧缓冲区
bool is_mqtt_wan_buf_ready = FALSE;
mqttPulish_t g_mqtt_publish = {0};


