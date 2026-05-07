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
  uint32_t          save_ts;                   
  char              device_name[20];         
  char              server_url[128];           
  uint16_t          server_port;               
  uint16_t          mqtt_keep_alive;		      
  char              mqtt_key[36];              
  char              mqtt_secret[36];           
  uint16_t          mqtt_connect_timeout;	     

  char              update_url[128];           
  uint16_t          update_port;               

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


