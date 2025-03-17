#ifndef _GLOBALDATA_H_
#define _GLOBALDATA_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "globalDef.h"

/*-------------------------------------------------------------------------------------*/

/*系统参数-----------------------------------------------------------------------------*/
extern bool arnics_start;
extern volatile uint32_t arnics_systick;
extern uint8_t VirtualEnvironment;  
extern bool eventosWantSleep;
extern bool BusinessWantSleep;
extern bool MediaWantSleep;
extern bool GuardWantSleep;
extern bool ConsoleWantSleep;

/*-------------------------------------------------------------------------------------*/

/*运行与配置参数------------------------------------------------------------------------*/
extern SytemCfg global_cfg;       // 全局配置参数
extern SytemState global_state;   // 全局运行参数


#ifdef __cplusplus
}
#endif
#endif
