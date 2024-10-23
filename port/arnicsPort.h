#ifndef _ARNICSPORT_H
#define _ARNICSPORT_H


#ifdef __cplusplus
extern "C" {
#endif

#include "../Inc/projDefine.h"
#include "../Inc/typedef.h"

/*------------------------------------------------------------*/
#ifdef STM32CHIP
#include "main.h"
#include "usart.h"
#define UARTDBG huart1
#endif



/*------------------------------------------------------------*/
extern void enable_debug_uart_IT_TXE(void);
/*------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif
