#ifndef _GLOBALDATA_H_
#define _GLOBALDATA_H_
#include "globalDef.h"

#ifdef __cplusplus
extern "C" {
#endif


// 系统参数
extern bool arnics_start;
extern volatile uint32_t arnics_systick;

// 运行与配置参数
 extern SytemCfg global_cfg;       // 全局配置参数
 extern SytemState global_state;   // 全局运行参数


#ifdef __cplusplus
}
#endif
#endif
