#ifndef DATA_LINK_H
#define DATA_LINK_H

#ifdef __cplusplus
extern "C" {
#endif
#include "../../../dataPlat/dataPlatInclude.h"


#define MCT(NAME) MCT_DATA_##NAME

// INDEX名称  数据指针  数据长度
#define MCT_DATA_LIST \
    X(MCT(devName),           global_cfg.DevName,         sizeof(global_cfg.DevName))         \
    X(MCT(serverURL),         global_cfg.ServerURL,       sizeof(global_cfg.ServerURL))       \
    X(MCT(serverPort),        &global_cfg.ServerPort,     sizeof(global_cfg.ServerPort))      \
    X(MCT(updateURL),         global_cfg.UpdateURL,       sizeof(global_cfg.UpdateURL))       \
    X(MCT(updatePort),        &global_cfg.UpdatePort,     sizeof(global_cfg.UpdatePort))      \
    X(MCT(mqttUploadTopic),   g_MQTopic.Send,             sizeof(g_MQTopic.Send))             \
    X(MCT(mqttRevTopic),      g_MQTopic.Recv,             sizeof(g_MQTopic.Recv))             \
    X(MCT(mqttRspTopic),      g_MQTopic.Resp,             sizeof(g_MQTopic.Resp))             \
    X(MCT(mqttCfgUpTopic),    g_MQTopic.ConfigUp,         sizeof(g_MQTopic.ConfigUp))         \
    X(MCT(mqttCfgDnTopic),    g_MQTopic.ConfigDn,         sizeof(g_MQTopic.ConfigDn))         \
    X(MCT(mqttProductKey),    global_cfg.ProductKey,      sizeof(global_cfg.ProductKey))      \
    X(MCT(mqttDevSecret),     global_cfg.DevSecret,       sizeof(global_cfg.DevSecret))       \
    X(MCT(mqttKeepAlive),     &global_cfg.KeepAlive,      sizeof(global_cfg.KeepAlive))       \
    X(MCT(mqttConnectTimeout),&global_cfg.ConnectTimeOut, sizeof(global_cfg.ConnectTimeOut))  \
    X(MCT(imsi),              global_state.wan_imsi,      sizeof(global_state.wan_imsi))      \
    X(MCT(imei),              global_state.wan_imei,      sizeof(global_state.wan_imei))      \
    X(MCT(ver),               global_state.wan_ver,       sizeof(global_state.wan_ver))       \
    X(MCT(iccid),             global_state.wan_iccid,     sizeof(global_state.wan_iccid))     \
    X(MCT(signal),            &networkPara.SignalStrength,sizeof(networkPara.SignalStrength)) \
    X(MCT(netStatus),         &networkPara.REGstatus,     sizeof(networkPara.REGstatus))      \
    X(MCT(mqttWanBuf),        mqttWanBuf,                 sizeof(mqttWanBuf))                 \
    X(MCT(mqttWanBufReady),   &mqttWanBufReady,           sizeof(mqttWanBufReady))            \
    X(MCT(mqttWanPub),        &mqttWanPub,                sizeof(mqttWanPub))                 \
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
