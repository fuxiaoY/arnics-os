#ifndef _SYSTEM_CFG_PUBLIC_H_
#define _SYSTEM_CFG_PUBLIC_H_

#ifdef __cplusplus
extern "C" {
#endif
/* typedef private ----------------------------------------------------*/
/**
 * @brief 对外'公有文档'在此处定义
 *        Definition of publicly accessible data struct is here.
 */
typedef struct 
{
  uint32_t          save_ts;                   //保存时间戳，同时也用于判定Flash上保存的参数集是否有效
  char              device_name[20];           //设备名（唯一序号），最长不得超过16字节，通常使用15个数字的IMEI，同时也是该设备的电子标签
  char              server_url[128];           //MQTT服务端域名或IP，最长不得超过128字节
  uint16_t          server_port;               //MQTT服务端域名
  uint16_t          mqtt_keep_alive;		       //MQTT连接保持时间，单位为秒，告诉服务端，如果我多久没有发言，就将我踢掉，缺省为60秒  
  char              mqtt_key[36];              //MQTT三元组 - 产品KEY，最长不得超过32字节
  char              mqtt_secret[36];           //MQTT三元组 - 设备密钥，最长不得超过32字节  
  uint16_t          mqtt_connect_timeout;	     //MQTT建立连接超时，单位为毫秒，缺省为10000毫秒 

  char              update_url[128];            //HTTP远程刷机域名或IP，最长不得超过128字节
  uint16_t          update_port;                //HTTP远程刷机端口，缺省为1883

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


