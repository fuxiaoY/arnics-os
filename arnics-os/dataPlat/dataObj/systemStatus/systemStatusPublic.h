
#ifndef _SYSTEM_STATUS_PUBLIC_H_
#define _SYSTEM_STATUS_PUBLIC_H_

#ifdef __cplusplus
extern "C" {
#endif
/* typedef private ----------------------------------------------------*/
/**
 * @brief 对外'公有文档'在此处定义
 *        Definition of publicly accessible data struct is here.
 */

#include "dePartment/centerBusiness/centerBusiness.h"

typedef struct 
{
  uint32_t          save_ts;                  

}systemStatus_t;

#undef X 
#define X(auth,index,type,subtype,var,len,key) + 1
#define SYSTEM_STATUS_ENTRIES \
X(NA, DATA(g_system_status),       TYPE_STRUCT,  TYPE_NULL, &g_system_status,                    sizeof(systemStatus_t),         "g_system_status"        )   




#ifdef __cplusplus
}
#endif
#endif
