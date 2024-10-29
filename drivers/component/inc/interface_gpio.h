#ifndef INTERFACE_GPIO_H
#define INTERFACE_GPIO_H
#ifdef __cplusplus
extern "C" {
#endif
#include "../../../Inc/ProjDefine.h"
#include "../../../Inc/typedef.h"
#include "../../common/drivers_common.h"

/* function prototypes -----------------------------------------------*/
extern void gpio_init(device_t *self);
extern void gpio_close(device_t *self);
extern bool gpio_set(device_t *self, int statue);





#ifdef __cplusplus
}
#endif
#endif /* INTERFACE_UART_H */
