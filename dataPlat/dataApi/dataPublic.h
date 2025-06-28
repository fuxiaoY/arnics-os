
#ifndef DATA_PUBLIC_H
#define DATA_PUBLIC_H
#ifdef __cplusplus
extern "C" {
#endif
#include "dataApi.h"

/* typedef public --------------------------------------------------*/
/**
 * @brief 对外'公有文档'在此处定义
 *        Definition of publicly accessible data struct is here.
 */
#define DATA_PUBLIC_STRUCT
#include "dataPlat/dataObj/dataClass.h"
#undef DATA_PUBLIC_STRUCT
/* systemPara ----------------------------------------------------*/


#define arnics_start            ((*(bool*)DATA_PTR(arnics_start)))
#define virtual_environment     ((*(uint8_t*)DATA_PTR(virtual_environment))) 
#define sys_allow_sleep         ((*(bool*)DATA_PTR(sys_allow_sleep)))
#define eventos_want_sleep      ((*(bool*)DATA_PTR(eventos_want_sleep)))
#define business_want_sleep     ((*(bool*)DATA_PTR(business_want_sleep)))
#define media_want_sleep        ((*(bool*)DATA_PTR(media_want_sleep)))
#define guard_want_sleep        ((*(bool*)DATA_PTR(guard_want_sleep)))
#define console_want_sleep      ((*(bool*)DATA_PTR(console_want_sleep)))


/* systemCfg  ----------------------------------------------------*/


#define g_system_cfg ((*(systemCfg_t*)DATA_PTR(g_system_cfg)))

/* systemStatus  ------------------------------------------------*/
 




#ifdef __cplusplus
}
#endif
#endif
