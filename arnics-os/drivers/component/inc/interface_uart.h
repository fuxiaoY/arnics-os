#ifndef INTERFACE_UART_H
#define INTERFACE_UART_H
#ifdef __cplusplus
extern "C" {
#endif
#include "../../../Inc/ProjDefine.h"
#include "../../../Inc/typedef.h"
#include "../../common/drivers_common.h"

/* function prototypes -----------------------------------------------*/

enum UART_CTL
{
  UART_DSEND = 0,
};


extern int uart_open(device_t *self);
extern int uart_close(device_t *self);
extern int uart_read(device_t *self,void *buf, size_t count);
extern int uart_write(device_t *self, void *buf, size_t count);
extern int uart_ctl(device_t *self, int cmd, va_list ap);
extern void uart_irq(device_t *self);



#ifdef __cplusplus
}
#endif
#endif /* INTERFACE_UART_H */
  
