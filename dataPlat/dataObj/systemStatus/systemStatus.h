#ifndef _SYSTEM_STATUS_H_
#define _SYSTEM_STATUS_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "../../../Inc/typedef.h"
#include "../../../Inc/ProjDefine.h"

/* include----------------------------------------------------------*/
#include "dataPlat/dataObj/systemStatus/systemStatusPublic.h"
#include "dataPlat/dataManager/dataStore.h"
/* typedef private -------------------------------------------------*/
/**
 * @brief 对内'私有文档'在此处定义
 *        Definition of private accessible data struct is here.
 */

/* vars   ----------------------------------------------------------*/
extern systemStatus_t g_system_status;
/* typedef datastore ----------------------------------------------*/
/**
 * @brief 对可持续存储的空间支持
 *        Support for sustainable storage space.
 */
extern mqttTopic_t g_mqtt_topic;	   // MQTT主题集
extern networkPara_t networkPara;      // 媒体中心网络参数
extern char g_mqtt_wan_buf[256];       // MQTT远程帧缓冲区
extern bool is_mqtt_wan_buf_ready;
extern mqttPulish_t g_mqtt_publish;
#ifdef __cplusplus
}
#endif
#endif
