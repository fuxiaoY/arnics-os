#ifndef DATA_LINK_H
#define DATA_LINK_H

#ifdef __cplusplus
extern "C" {
#endif
#include "../../../dataPlat/dataPlatInclude.h"


#define MCT(NAME) MCT_DATA_##NAME

// INDEX名称  数据指针  数据长度
#define MCT_DATA_LIST \
    X(MCT(device_name),              g_system_cfg.device_name,            sizeof(g_system_cfg.device_name))   \
    X(MCT(server_url),               g_system_cfg.server_url,             sizeof(g_system_cfg.server_url))    \
    X(MCT(server_port),              &g_system_cfg.server_port,           sizeof(g_system_cfg.server_port))   \
    X(MCT(update_url),               g_system_cfg.update_url,             sizeof(g_system_cfg.update_url))    \
    X(MCT(update_port),              &g_system_cfg.update_port,           sizeof(g_system_cfg.update_port))   \
    X(MCT(mqtt_upload_topic),        g_mqtt_topic.upload,                 sizeof(g_mqtt_topic.upload))        \
    X(MCT(mqtt_command_topic),       g_mqtt_topic.command,                sizeof(g_mqtt_topic.command))       \
    X(MCT(mqtt_response_topic),      g_mqtt_topic.response,               sizeof(g_mqtt_topic.response))      \
    X(MCT(mqtt_config_up_topic),     g_mqtt_topic.config_up,              sizeof(g_mqtt_topic.config_up))     \
    X(MCT(mqtt_config_down_topic),   g_mqtt_topic.config_down,            sizeof(g_mqtt_topic.config_down))   \
    X(MCT(mqtt_key),                 g_system_cfg.mqtt_key,               sizeof(g_system_cfg.mqtt_key))      \
    X(MCT(mqtt_secret),              g_system_cfg.mqtt_secret,            sizeof(g_system_cfg.mqtt_secret))       \
    X(MCT(mqtt_keep_alive),          &g_system_cfg.mqtt_keep_alive,       sizeof(g_system_cfg.mqtt_keep_alive))       \
    X(MCT(mqtt_connect_timeout),     &g_system_cfg.mqtt_connect_timeout,  sizeof(g_system_cfg.mqtt_connect_timeout))  \
    X(MCT(wan_imsi),                 g_system_status.wan_imsi,            sizeof(g_system_status.wan_imsi))      \
    X(MCT(wan_imei),                 g_system_status.wan_imei,            sizeof(g_system_status.wan_imei))      \
    X(MCT(wan_ver),                  g_system_status.wan_ver,             sizeof(g_system_status.wan_ver))       \
    X(MCT(wan_iccid),                g_system_status.wan_iccid,           sizeof(g_system_status.wan_iccid))     \
    X(MCT(SignalStrength),           &networkPara.SignalStrength,         sizeof(networkPara.SignalStrength))    \
    X(MCT(REGstatus),                &networkPara.REGstatus,              sizeof(networkPara.REGstatus))         \
    X(MCT(g_mqtt_wan_buf),           g_mqtt_wan_buf,                      sizeof(g_mqtt_wan_buf))                \
    X(MCT(is_mqtt_wan_buf_ready),    &is_mqtt_wan_buf_ready,              sizeof(is_mqtt_wan_buf_ready))         \
    X(MCT(g_mqtt_publish),               &g_mqtt_publish,                         sizeof(g_mqtt_publish))                    \
// 生成枚举索引
typedef enum {
    #undef X
    #define X(name,var,len) name,
    MCT_DATA_LIST
    #undef X
    MCT_DATA_MAX_COUNT,
} mctParaIndex;

// 参数表结构体
typedef struct {
    void* pVar;
    uint32_t len;
    mctParaIndex index; // 添加索引字段
} mctParaList;


extern void *mctParaGetPtr(mctParaIndex id);
extern uint32_t mctParaGet(mctParaIndex id, void* outBuf, uint32_t bufLen);
extern uint32_t mctParaSet(mctParaIndex id, const void* inData, uint32_t dataLen);

#define MCT_PTR(ID) mctParaGetPtr(MCT(ID))
#define MCT_GET(ID,OUT_BUF,BUF_LEN) mctParaGet(MCT(ID),OUT_BUF,BUF_LEN)
#define MCT_SET(ID,IN_DATA,DATA_LEN) mctParaSet(MCT(ID),IN_DATA,DATA_LEN)
#ifdef __cplusplus
}
#endif
#endif
