#include "arnicsPort.h"

//根据软件平台实现串口发送中断的打开
#ifdef STM32CHIP
#include "usart.h"

void enable_debug_uart_IT_TXE(void)
{
    __HAL_UART_ENABLE_IT(&UARTDBG, UART_IT_TXE);
}
#else
void enable_debug_uart_IT_TXE(void)
{

}
#endif
