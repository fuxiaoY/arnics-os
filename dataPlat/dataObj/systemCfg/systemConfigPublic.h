#ifndef _SYSTEM_CFG_PUBLIC_H_
#define _SYSTEM_CFG_PUBLIC_H_

#ifdef __cplusplus
extern "C" {
#endif
/* typedef private ----------------------------------------------------*/
/**
 * @brief ����'�����ĵ�'�ڴ˴�����
 *        Definition of publicly accessible data struct is here.
 */
typedef struct 
{
  uint32_t          save_ts;                   //����ʱ�����ͬʱҲ�����ж�Flash�ϱ���Ĳ������Ƿ���Ч
  char              device_name[20];           //�豸����Ψһ��ţ�������ó���16�ֽڣ�ͨ��ʹ��15�����ֵ�IMEI��ͬʱҲ�Ǹ��豸�ĵ��ӱ�ǩ
  char              server_url[128];           //MQTT�����������IP������ó���128�ֽ�
  uint16_t          server_port;               //MQTT���������
  uint16_t          mqtt_keep_alive;		       //MQTT���ӱ���ʱ�䣬��λΪ�룬���߷���ˣ�����Ҷ��û�з��ԣ��ͽ����ߵ���ȱʡΪ60��  
  char              mqtt_key[36];              //MQTT��Ԫ�� - ��ƷKEY������ó���32�ֽ�
  char              mqtt_secret[36];           //MQTT��Ԫ�� - �豸��Կ������ó���32�ֽ�  
  uint16_t          mqtt_connect_timeout;	     //MQTT�������ӳ�ʱ����λΪ���룬ȱʡΪ10000���� 

  char              update_url[128];            //HTTPԶ��ˢ��������IP������ó���128�ֽ�
  uint16_t          update_port;                //HTTPԶ��ˢ���˿ڣ�ȱʡΪ1883

}systemCfg_t;


#undef X 
#define X(auth,index,type,subtype,var,len,key) + 1
#define SYSTEM_CFG_ENTRIES \
X(NA,  DATA(g_system_cfg),          TYPE_STRUCT , TYPE_NULL,      &g_system_cfg,                      sizeof(g_system_cfg)                      , "g_system_cfg" )         \
X(R|W,  DATA(device_name),            TYPE_STRING, TYPE_CHAR,      g_system_cfg.device_name,            sizeof(g_system_cfg.device_name)          , "device_name")           \
X(R|W,  DATA(server_url),             TYPE_STRING, TYPE_CHAR,      g_system_cfg.server_url,             sizeof(g_system_cfg.server_url)           , "server_url" )           \
X(R|W,  DATA(server_port),            TYPE_UINT16, TYPE_NULL,      &g_system_cfg.server_port,          sizeof(g_system_cfg.server_port)          ,"server_port")            \
X(R|W,  DATA(update_url),             TYPE_STRING, TYPE_CHAR,      g_system_cfg.update_url,             sizeof(g_system_cfg.update_url)           , "update_url" )           \
X(R|W,  DATA(update_port),            TYPE_UINT16, TYPE_NULL,      &g_system_cfg.update_port,          sizeof(g_system_cfg.update_port)          ,"update_port")            \
X(R|W,  DATA(mqtt_key),               TYPE_STRING, TYPE_CHAR,      g_system_cfg.mqtt_key,               sizeof(g_system_cfg.mqtt_key)             , "mqtt_key"   )           \
X(R|W,  DATA(mqtt_secret),            TYPE_STRING, TYPE_CHAR,      g_system_cfg.mqtt_secret,            sizeof(g_system_cfg.mqtt_secret)          , "mqtt_secret" )          \
X(R|W,  DATA(mqtt_keep_alive),        TYPE_UINT16, TYPE_NULL,      &g_system_cfg.mqtt_keep_alive,       sizeof(g_system_cfg.mqtt_keep_alive)     , "mqtt_keep_alive")       \
X(R|W,  DATA(mqtt_connect_timeout),   TYPE_UINT16, TYPE_NULL,      &g_system_cfg.mqtt_connect_timeout,  sizeof(g_system_cfg.mqtt_connect_timeout), "mqtt_connect_timeout")  

#ifdef __cplusplus
}
#endif
#endif


