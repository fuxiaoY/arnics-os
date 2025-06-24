#include "arnicsPara.h"

/*-------------------------------------------------------------------------------------*/

/*系统参数-----------------------------------------------------------------------------*/
bool              arnics_start = false;
volatile uint32_t arnics_systick = 0;
uint8_t           virtual_environment = 0;  
bool              sys_allow_sleep = false;
bool              eventos_want_sleep  = false;
bool              business_want_sleep = false;
bool              media_want_sleep    = false;
bool              guard_want_sleep    = false;
bool              console_want_sleep  = false;

