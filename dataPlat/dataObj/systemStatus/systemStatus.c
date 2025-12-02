
#include "systemStatus.h"
#include "port/arnicsPort.h"
/*-------------------------------------------------------------------------------------*/

/*运行参数-----------------------------------------------------------------------------*/
systemStatus_t g_system_status = {0};       // 全局运行参数
networkPara_t networkPara;                  // 媒体中心网络参数
char g_mqtt_wan_buf[256]       = {0};       // MQTT远程帧缓冲区
mqttPulish_t g_mqtt_publish    = {0};
mqttTopic_t g_mqtt_topic       = {0};	     // MQTT主题集
bool is_mqtt_wan_buf_ready;

/*-------------------------------------------------------------------------------------*/

/*数据操作 ---------------------------------------------------------------------------*/
void system_status_reset(void)
{

}
DEFINE_DATA_ACTION_FUNC(dataStoreSysSta_t,g_system_status)

bool system_status_save(void)
{
    bool result = false;
    dataStoreSysSta_t* p_sys_status_store = arnicsMalloc(sizeof(dataStoreSysSta_t));
    /*-------------------------------------------*/
    // 对单一数据进行校验打包，放入连续空间中
    DATA_ACTION_FUNC(p_sys_status_store,g_system_status,DATA_PACK);
    /*-------------------------------------------*/
    result = data_save(PARTITION_NAME_SYSTEM_STATUS_PARA,0, (uint8_t *)p_sys_status_store, sizeof(dataStoreSysSta_t));
    arnicsFree(p_sys_status_store);
    return result;
}

bool system_status_load(void)
{
    bool result = false;
    bool need_reset_load = false;
    dataStoreSysSta_t* p_sys_status_store = arnicsMalloc(sizeof(dataStoreSysSta_t));
    data_load(PARTITION_NAME_SYSTEM_STATUS_PARA,0, (uint8_t *)p_sys_status_store, sizeof(dataStoreSysSta_t));
    /*-------------------------------------------*/
    //  对单一数据从连续空间进行解包校验，取值
    // g_system_cfg
    result = DATA_ACTION_FUNC(p_sys_status_store,g_system_status,DATA_LOAD);
    if(!result)    //  解包失败，需要默认值恢复
    {
        printf("system_status load fail\r\n");
        need_reset_load = true;
        system_status_reset();
    }
    else
    {
        printf("system_status load succeed\r\n");
    }
    /*-------------------------------------------*/
    arnicsFree(p_sys_status_store);
    // 默认值回写
    if(need_reset_load)
    {
        result = system_status_save();
    }
    mqtt_topic_reset();
    return result;
}

