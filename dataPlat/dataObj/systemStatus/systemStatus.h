#ifndef _SYSTEM_STATUS_H_
#define _SYSTEM_STATUS_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "../../../Inc/typedef.h"
#include "../../../Inc/ProjDefine.h"

/* include----------------------------------------------------------*/
#include "dataPlat/dataObj/systemStatus/systemStatusPublic.h"
/* typedef private -------------------------------------------------*/
/**
 * @brief ����'˽���ĵ�'�ڴ˴�����
 *        Definition of private accessible data struct is here.
 */

/* vars   ----------------------------------------------------------*/
extern systemStatus_t g_system_status;

extern mqttTopic_t g_mqtt_topic;	   // MQTT���⼯
extern networkPara_t networkPara;      // ý�������������
extern char g_mqtt_wan_buf[256];       // MQTTԶ��֡������
extern bool is_mqtt_wan_buf_ready;
extern mqttPulish_t g_mqtt_publish;
#ifdef __cplusplus
}
#endif
#endif
