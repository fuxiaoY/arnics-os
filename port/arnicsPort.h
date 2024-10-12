#ifndef _ARNICSPORT_H
#define _ARNICSPORT_H

#include "../Inc/projDefine.h"
#include "../Inc/typedef.h"
#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------*/
#ifdef STM32CHIP
#include "main.h"
#define UARTDBG huart1
#endif



/*------------------------------------------------------------*/
extern void enable_debug_uart_IT_TXE(void);
/*------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif
