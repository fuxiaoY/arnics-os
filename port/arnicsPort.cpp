#include "arnicsPort.h"

//根据软件平台实现串口发送中断的打开
void enable_debug_uart_IT_TXE(void)
{
#ifdef STM32CHIP
    __HAL_UART_ENABLE_IT(&g_UARTDBG, UART_IT_TXE);
#endif
}