
#ifndef _SYSTEM_STATUS_PUBLIC_H_
#define _SYSTEM_STATUS_PUBLIC_H_

#ifdef __cplusplus
extern "C" {
#endif
/* typedef private ----------------------------------------------------*/
/**
 * @brief ����'�����ĵ�'�ڴ˴�����
 *        Definition of publicly accessible data struct is here.
 */

#include "dePartment/centerBusiness/centerBusiness.h"
#include "thirdParty/modContact/common/mctStruct.h"

typedef struct 
{
  uint32_t save_ts;				         //����ʱ���
  wisdomStackPart_e work_status;          //��ǰ����״̬ 
  wisdomStackPart_e prework_status;      //�ϴε�����״̬
  char  wan_imsi[20];
  char  wan_imei[20];
  char  wan_ver[20];
  char  wan_iccid[25];
}systemStatus_t;

//MQTT����
typedef struct 
{
  char upload[128];             //�����������⣬����ó���128�ֽ�
  
  char command[128];            //���ж������⣬����ó���128�ֽ�
  char response[128];           //����Ӧ�����⣬����ó���128�ֽ�
  
  char config_up[128];          //�����������⣬����ó���128�ֽ�
  char config_down[128];        //�����������⣬����ó���128�ֽ�
}mqttTopic_t;

#undef X 
#define X(auth,index,type,subtype,var,len,key) + 1
#define SYSTEM_STATUS_ENTRIES \
X(NA, DATA(g_system_status),       TYPE_STRUCT,  TYPE_NULL, &g_system_status,                    sizeof(systemStatus_t),         "g_system_status"        )   \
X(R|W, DATA(SignalStrength),        TYPE_UINT8,   TYPE_NULL, &networkPara.SignalStrength,         sizeof(networkPara.SignalStrength),         "SignalStrength"        )   \
X(R|W, DATA(REGstatus),             TYPE_ENUM,    TYPE_NULL, &networkPara.REGstatus,              sizeof(networkPara.REGstatus),              "REGstatus"             )   \
X(R|W, DATA(g_mqtt_wan_buf),        TYPE_ARRAY,   TYPE_CHAR,  g_mqtt_wan_buf,                      sizeof(g_mqtt_wan_buf),                     "g_mqtt_wan_buf"        )  \
X(R|W, DATA(is_mqtt_wan_buf_ready), TYPE_BOOL,    TYPE_NULL, &is_mqtt_wan_buf_ready,              sizeof(is_mqtt_wan_buf_ready),              "is_mqtt_wan_buf_ready" )   \
X(R|W, DATA(g_mqtt_publish),        TYPE_STRUCT,  TYPE_NULL, &g_mqtt_publish,                     sizeof(g_mqtt_publish),                     "g_mqtt_publish"        )   \
X(R|W, DATA(mqtt_upload_topic),     TYPE_STRING,   TYPE_CHAR,  g_mqtt_topic.upload,                 sizeof(g_mqtt_topic.upload),                "mqtt_upload_topic"     )  \
X(R|W, DATA(mqtt_command_topic),    TYPE_STRING,   TYPE_CHAR,  g_mqtt_topic.command,                sizeof(g_mqtt_topic.command),               "mqtt_command_topic"    )  \
X(R|W, DATA(mqtt_response_topic),   TYPE_STRING,   TYPE_CHAR,  g_mqtt_topic.response,               sizeof(g_mqtt_topic.response),              "mqtt_response_topic"   )  \
X(R|W, DATA(mqtt_config_up_topic),  TYPE_STRING,   TYPE_CHAR,  g_mqtt_topic.config_up,              sizeof(g_mqtt_topic.config_up),             "mqtt_config_up_topic"  )  \
X(R|W, DATA(mqtt_config_down_topic),TYPE_STRING,   TYPE_CHAR,  g_mqtt_topic.config_down,            sizeof(g_mqtt_topic.config_down),           "mqtt_config_down_topic")  \
X(R|W, DATA(wan_imsi),              TYPE_STRING,   TYPE_CHAR,  g_system_status.wan_imsi,            sizeof(g_system_status.wan_imsi),           "wan_imsi"              )  \
X(R|W, DATA(wan_imei),              TYPE_STRING,   TYPE_CHAR,  g_system_status.wan_imei,            sizeof(g_system_status.wan_imei),           "wan_imei"              )  \
X(R|W, DATA(wan_ver),               TYPE_STRING,   TYPE_CHAR,  g_system_status.wan_ver,             sizeof(g_system_status.wan_ver),            "wan_ver"               )  \
X(R|W, DATA(wan_iccid),             TYPE_STRING,   TYPE_CHAR,  g_system_status.wan_iccid,           sizeof(g_system_status.wan_iccid),          "wan_iccid"             )  



#ifdef __cplusplus
}
#endif
#endif
