#include "globalData.h"
#pragma arm section code = "._entry_dataPlat_api"
// 系统参数
bool arnics_start = false;
volatile uint32_t arnics_systick = 0;
bool eventosWantSleep = false;

// 运行与配置参数
 SytemCfg   global_cfg = {0};         // 全局配置参数
 SytemState global_state = {0};       // 全局运行参数



