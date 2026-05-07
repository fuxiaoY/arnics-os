#ifdef DATA_ALL_STRUCT           // 公有文档+私有文档
#include "dataPlat/dataObj/arnicsPara/arnicsPara.h"
#include "dataPlat/dataObj/systemStatus/systemStatus.h"
#include "dataPlat/dataObj/systemCfg/systemConfig.h"
#endif
#ifdef DATA_CLASS                // 数据X表
ARNICS_PARA_ENTRIES
SYSTEM_STATUS_ENTRIES
SYSTEM_CFG_ENTRIES
#endif
#ifdef DATA_PUBLIC_STRUCT         // 公有文档
#include "dataPlat/dataObj/arnicsPara/arnicsParaPublic.h"
#include "dataPlat/dataObj/systemCfg/systemConfigPublic.h"  
#include "dataPlat/dataObj/systemStatus/systemStatusPublic.h"
#endif
#ifdef DATA_LIST                  // 数据管理列表
extern const dataParaList_t unity_system_paralist[];
extern const dataParaList_t unity_global_cfg_paralist[];
extern const dataParaList_t unity_global_status_paralist[];

extern uint16_t unity_systemParaNumGet(void);
extern uint16_t unity_GlobalCfgParaNumGet(void);
extern uint16_t unity_GlobalStatParaNumGet(void);
#elif defined(DATA_LIST_INSTANCE) // 数据管理列表实例
#undef X 
#define X(auth,index,type,subtype,var,len,key) {auth,index,type,subtype,var,len,key},
const dataParaList_t unity_system_paralist[] = 
{
    ARNICS_PARA_ENTRIES
};

const dataParaList_t unity_global_cfg_paralist[] = 
{
    SYSTEM_CFG_ENTRIES
};

const dataParaList_t unity_global_status_paralist[] = 
{
    SYSTEM_STATUS_ENTRIES
};

uint16_t unity_systemParaNumGet(void)
{
    return (sizeof(unity_system_paralist) / sizeof(dataParaList_t));
}

uint16_t unity_GlobalCfgParaNumGet(void)
{
    return (sizeof(unity_global_cfg_paralist) / sizeof(dataParaList_t));
}

uint16_t unity_GlobalStatParaNumGet(void)
{
    return (sizeof(unity_global_status_paralist) / sizeof(dataParaList_t));
}

#undef X 
#endif

