

#include "systemStatus.h"
#include "port/arnicsPort.h"

systemStatus_t g_system_status = {0};       // ȫ�����в���


mqttTopic_t g_mqtt_topic = {0};	      //MQTT���⼯
networkPara_t networkPara;         // ý�������������
char g_mqtt_wan_buf[256] = {0};       // MQTTԶ��֡������
bool is_mqtt_wan_buf_ready = FALSE;
mqttPulish_t g_mqtt_publish = {0};


