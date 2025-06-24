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
/* typedef private -------------------------------------------------*/
/**
 * @brief ����'˽���ĵ�'�ڴ˴�����
 *        Definition of private accessible data struct is here.
 */



/* vars   ----------------------------------------------------------*/
extern bool              arnics_start;
extern volatile uint32_t arnics_systick;
extern bool              sys_allow_sleep;            // ϵͳ�Ƿ��������߱�־λ
extern uint8_t           virtual_environment;  
extern bool              eventos_want_sleep;
extern bool              business_want_sleep;
extern bool              media_want_sleep;
extern bool              guard_want_sleep;
extern bool              console_want_sleep;

#undef PUBLIC_STRUCT
#ifdef __cplusplus
}
#endif
#endif
