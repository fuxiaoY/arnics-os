#ifndef INTERFACE_GPIO_H
#define INTERFACE_GPIO_H
#ifdef __cplusplus
extern "C" {
#endif
#include "../../../Inc/ProjDefine.h"
#include "../../../Inc/typedef.h"
#include "../../common/drivers_common.h"
/* function prototypes -----------------------------------------------*/
enum IO_CTL
{
  IO_TOGGLE = 0,
  IO_SET,
  IO_RESET,
  IO_READ,
  IO_FREECFG,
  IO_NVIC_DISABLE,
};

extern int gpio_open(device_t *self);
extern int gpio_close(device_t *self);

extern int gpio_ctl(device_t *self, int cmd, va_list ap);
extern void gpio_irq(device_t *self);



#ifdef __cplusplus
}
#endif
#endif /* INTERFACE_UART_H */
