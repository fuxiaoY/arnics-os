#include "systemConfig.h"
#include "port/arnicsPort.h"

systemCfg_t g_system_cfg = {0};


/*-------------------------------------------------------------------------------------*/

/*数据操作 ---------------------------------------------------------------------------*/
static void system_cfg_reset()
{
    
}
DEFINE_DATA_ACTION_FUNC(dataStoreSysCfg_t,g_system_cfg)


bool system_config_save(void)
{
    bool result = false;
    dataStoreSysCfg_t sys_cfg_store;
    /*-------------------------------------------*/
    // 对单一数据进行校验打包，放入连续空间中
    DATA_ACTION_FUNC(&sys_cfg_store,g_system_cfg,DATA_PACK);
    /*-------------------------------------------*/
    result = data_save(PARTITION_NAME_SYSTEM_CFG_PARA,0, (uint8_t *)&sys_cfg_store, sizeof(sys_cfg_store));
    return result;
}

bool system_config_load(void)
{
    bool result = false;
    bool need_reset_load = false;
    dataStoreSysCfg_t sys_cfg_store;
    data_load(PARTITION_NAME_SYSTEM_CFG_PARA,0, (uint8_t *)&sys_cfg_store, sizeof(sys_cfg_store));
    /*-------------------------------------------*/
    //  对单一数据从连续空间进行解包校验，取值
    // g_system_cfg
    result = DATA_ACTION_FUNC(&sys_cfg_store,g_system_cfg,DATA_LOAD);
    if(!result)    //  解包失败，需要默认值恢复
    {
        need_reset_load = true;
        system_cfg_reset();
    }
    /*-------------------------------------------*/

    // 默认值回写
    if(need_reset_load)
    {
        result = system_config_save();
    }
    return result;
}
