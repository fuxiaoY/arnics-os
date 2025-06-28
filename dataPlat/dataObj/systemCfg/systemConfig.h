#ifndef _SYSTEM_CFG_H_
#define _SYSTEM_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/typedef.h"
#include "Inc/ProjDefine.h"

/* include----------------------------------------------------------*/
#include "dataPlat/dataObj/systemCfg/systemConfigPublic.h"  
#include "dataPlat/dataManager/dataStore.h"
/* typedef private -------------------------------------------------*/
/**
 * @brief 对内'私有文档'在此处定义
 *        Definition of private accessible data struct is here.
 */

/* vars   ----------------------------------------------------------*/
extern systemCfg_t g_system_cfg;
/* typedef datastore ----------------------------------------------*/
/**
 * @brief 对可持续存储的空间支持
 *        Support for sustainable storage space.
 */
STRUCT_WITH_CRC(systemCfg_t,g_system_cfg);


typedef struct
{
    SOTRE_SUPPORT(g_system_cfg);
}dataStoreSysCfg_t;
#ifdef __cplusplus
}
#endif
#endif
