#include "../inc/interface_uart.h"
#include "../../bsp/bsp_def.h"
#include "usart.h"

#ifdef _USE_RING_
#include "../../../thirdParty/ring/ring.h"
#endif
int uart_open(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    bsp_usart_init(self->device);
 #ifdef _USE_RING_
    uart_t *uart = (uart_t *)self->device;
    ring_init(uart->ring_tx,uart->tx_buf,uart->tx_buf_size,uart->ring_rx,uart->rx_buf,uart->rx_buf_size);
 #endif   
    return 0;
}
int uart_close(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    bsp_usart_close(self->device);
    return 0;
}

int uart_read(device_t *self,void *buf, size_t count)
{
    if(self->ds == 0)
    {
        return -1;
    }
#ifdef _USE_RING_
    uart_t *uart = (uart_t *)self->device;
    return ring_read(uart->ring_rx,buf,count);
#else
    return bsp_usart_read(self->device, buf, count);
    return 0;
#endif

}

int uart_write(device_t *self, void *buf, size_t count)
{
    if(self->ds == 0)
    {
        return -1;
    }
#ifdef _USE_RING_
    uart_t *uart = (uart_t *)self->device;
    ring_write(uart->ring_tx ,buf,count);
    bsp_uart_IT_enable(self->device);
    return 0;
#else
    return bsp_usart_write(self->device, buf, count);
#endif

}

int uart_ctl(device_t *self, int cmd,va_list ap)
{
    void* arg1 = va_arg(ap, void*);
    if(self->ds == 0)
    {
        return -1;
    }  
    switch(cmd)
    {
        case UART_DSEND:
        return bsp_uart_dirct_send(self->device,arg1);
        default: return -1; 
    }

}
void uart_irq(device_t *self)
{
    if(self->ds == 0)
    {
        return;
    }
#ifdef _USE_RING_
    bsp_uart_irq_ring(self->device);
#else
    bsp_uart_irq(self->device);
#endif
}



