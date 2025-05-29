#include "globalData.h"
_SECTION( "._entry_dataPlat_api")
/*-------------------------------------------------------------------------------------*/

/*系统参数-----------------------------------------------------------------------------*/
bool arnics_start = false;
volatile uint32_t arnics_systick = 0;
uint8_t virtual_environment = 0;  

bool eventos_want_sleep  = false;
bool business_want_sleep = false;
bool media_want_sleep    = false;
bool guard_want_sleep    = false;
bool console_want_sleep  = false;
// 系统是否允许休眠标志位
bool sys_allow_sleep = false;
/*-------------------------------------------------------------------------------------*/

/*运行与配置参数------------------------------------------------------------------------*/
 systemCfg_t   g_system_cfg = {0};         // 全局配置参数
 systemStatus_t g_system_status = {0};       // 全局运行参数
