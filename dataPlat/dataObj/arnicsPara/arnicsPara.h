#ifndef _ARNICS_PARA_
#define _ARNICS_PARA_

#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/typedef.h"
#include "Inc/ProjDefine.h"
#define PUBLIC_STRUCT
/* include----------------------------------------------------------*/
#include "dataPlat/dataObj/arnicsPara/arnicsParaPublic.h"
#include "dataPlat/dataManager/dataStore.h"
/* typedef private -------------------------------------------------*/
/**
 * @brief ����'˽���ĵ�'�ڴ˴�����
 *        Definition of private accessible data struct is here.
 */
/* vars   ----------------------------------------------------------*/
extern bool              arnics_start;
extern bool              sys_allow_sleep;            // ϵͳ�Ƿ��������߱�־λ
extern uint8_t           virtual_environment;  
extern bool              eventos_want_sleep;
extern bool              business_want_sleep;
extern bool              media_want_sleep;
extern bool              guard_want_sleep;
extern bool              console_want_sleep;
/* typedef datastore ----------------------------------------------*/
/**
 * @brief �Կɳ����洢�Ŀռ�֧��
 *        Support for sustainable storage space.
 */
STRUCT_WITH_CRC(bool,sys_allow_sleep);
STRUCT_WITH_CRC(uint8_t,virtual_environment);

typedef struct
{
    SOTRE_SUPPORT(sys_allow_sleep);
    SOTRE_SUPPORT(virtual_environment);
}dataStoreArnicsPara_t;



#undef PUBLIC_STRUCT
#ifdef __cplusplus
}
#endif
#endif
