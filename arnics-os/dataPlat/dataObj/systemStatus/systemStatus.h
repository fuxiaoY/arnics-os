#ifndef _SYSTEM_STATUS_H_
#define _SYSTEM_STATUS_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/typedef.h"
#include "Inc/projDefine.h"

/* include----------------------------------------------------------*/
#include "dataPlat/dataObj/systemStatus/systemStatusPublic.h"
#include "dataPlat/dataManager/dataStore.h"
/* typedef private -------------------------------------------------*/
/**
 * @brief 对内'私有文档'在此处定义
 *        Definition of private accessible data struct is here.
 */

/* vars   ----------------------------------------------------------*/
extern systemStatus_t g_system_status;
/* typedef datastore ----------------------------------------------*/
/**
 * @brief 对可持续存储的空间支持
 *        Support for sustainable storage space.
 */

#ifdef __cplusplus
}
#endif
#endif
