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



/* 外设初始化任务清单 */
const tTaskFunc initTaskList[] =
{


};

/*运行时配置参数、运行时状态记录预加载*/
const tTaskFunc preloadTaskList[] =
{


};

/*外挂设备初始化*/
const tTaskFunc deviceTaskList[] =
{


};
