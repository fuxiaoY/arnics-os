#include "arnicsPara.h"
#include "port/arnicsPort.h"
#include "common/commonInclude.h"

/*-------------------------------------------------------------------------------------*/

/*系统参数-----------------------------------------------------------------------------*/
bool              arnics_start        = false;
volatile uint32_t arnics_systick      = 0;
uint8_t           virtual_environment = 0;  
bool              sys_allow_sleep     = false;
bool              eventos_want_sleep  = false;
bool              business_want_sleep = false;
bool              media_want_sleep    = false;
bool              guard_want_sleep    = false;
bool              console_want_sleep  = false;

/*-------------------------------------------------------------------------------------*/

/*数据操作 ---------------------------------------------------------------------------*/
static void virtual_environment_reset(void)
{
    virtual_environment = 0; 
}
DEFINE_DATA_ACTION_FUNC(dataStoreArnicsPara_t,virtual_environment)

static void sys_allow_sleep_reset(void)
{
    sys_allow_sleep = false; 
}
DEFINE_DATA_ACTION_FUNC(dataStoreArnicsPara_t,sys_allow_sleep)


bool arnics_para_save(void)
{
    bool result = false;
    dataStoreArnicsPara_t* p_arnics_para_store = arnicsMalloc(sizeof(dataStoreArnicsPara_t));
    /*-------------------------------------------*/
    // 对单一数据进行校验打包，放入连续空间中
    DATA_ACTION_FUNC(p_arnics_para_store,virtual_environment,DATA_PACK);
    DATA_ACTION_FUNC(p_arnics_para_store,sys_allow_sleep,DATA_PACK);
    /*-------------------------------------------*/
    result = data_save(PARTITION_NAME_ARNICS_SYS_PARA,0, (uint8_t *)p_arnics_para_store, sizeof(dataStoreArnicsPara_t));
    arnicsFree(p_arnics_para_store);
    return result;
}

bool arnics_para_load(void)
{
    bool result = false;
    bool need_reset_load = false;
    dataStoreArnicsPara_t* p_arnics_para_store = arnicsMalloc(sizeof(dataStoreArnicsPara_t));
    data_load(PARTITION_NAME_ARNICS_SYS_PARA,0, (uint8_t *)p_arnics_para_store, sizeof(p_arnics_para_store));
    /*-------------------------------------------*/
    //  对单一数据从连续空间进行解包校验，取值
    // virtual_environment
    result = DATA_ACTION_FUNC(p_arnics_para_store,virtual_environment,DATA_LOAD);
    if(!result)    //  解包失败，需要默认值恢复
    {
        need_reset_load = true;
        virtual_environment_reset();
    }
    // sys_allow_sleep
    result = DATA_ACTION_FUNC(p_arnics_para_store,sys_allow_sleep,DATA_LOAD);
    if(!result)   
    {
        need_reset_load = true;
        sys_allow_sleep_reset();
    }
    /*-------------------------------------------*/
    arnicsFree(p_arnics_para_store);
    // 默认值回写
    if(need_reset_load)
    {
        result = arnics_para_save();
    }
    return result;
}
