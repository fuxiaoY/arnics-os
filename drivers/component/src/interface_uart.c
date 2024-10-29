#include "../inc/interface_uart.h"
#include "../../common/drivers_common.h"
#include "../../bsp/bsp_def.h"



// 初始化UART
void uart_init(device_t *self)
{

    //bsp_usart_init(self->device);
}

// 关闭UART
void uart_close(device_t *self)
{
    //bsp_usart_close(self->device);
}


// 写数据
unsigned int uart_write(device_t *self, const void *buf, unsigned int len)
{
    //return bsp_usart_write(self->uart_ds, buf, len);
    return 0;
}

// 读数据
unsigned int uart_read(device_t *self, void *buf, unsigned int len)
{
    //return bsp_usart_read(self->uart_ds, buf, len);
    return 0;
}

