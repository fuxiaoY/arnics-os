#ifndef INTERFACE_UART_H
#define INTERFACE_UART_H
#ifdef __cplusplus
extern "C" {
#endif
#include "../../../Inc/ProjDefine.h"
#include "../../../Inc/typedef.h"
#include "../../common/drivers_common.h"

/* function prototypes -----------------------------------------------*/
extern void uart_init(device_t *self);
extern void uart_close(device_t *self);
extern unsigned int uart_write(device_t *self, const void *buf, unsigned int len);
extern unsigned int uart_read(device_t *self, void *buf, unsigned int len);

extern unsigned int uart_para_change(device_t *self, void *buf, unsigned int len);




/* demo *
char send_buf[] = "Hello, UART!";
device_t my_uart;
int main(){
    uart_init("uart1",&my_uart, 115200, 8, 1, 0, false);
    uart_write(&my_uart, send_buf, sizeof(send_buf) - 1);
}
 * demo */

#ifdef __cplusplus
}
#endif
#endif /* INTERFACE_UART_H */
