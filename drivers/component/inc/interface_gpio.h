#ifndef INTERFACE_GPIO_H
#define INTERFACE_GPIO_H
#ifdef __cplusplus
extern "C" {
#endif
#include "../../../Inc/ProjDefine.h"
#include "../../../Inc/typedef.h"
#include "../../common/drivers_common.h"
/* function prototypes -----------------------------------------------*/
/*
 *   int (*ds_open)(void *dev);
 *   int (*ds_close)(void *dev);
 *   int (*ds_read)(void *dev, void *buf, size_t count);
 *   int (*ds_write)(void *dev, void *buf, size_t count);
 *   int (*ds_ctl)(void *dev, int cmd, void *args);
*/
enum IO_CTL
{
  IO_TOGGLE = 0,
  IO_SET,
  IO_RESET,
  IO_READ,
};

extern int gpio_open(device_t *self);
extern int gpio_close(device_t *self);

extern int gpio_read(device_t *self,void *buf, size_t count);
extern int gpio_ctl(device_t *self, int cmd, void *args);




#ifdef __cplusplus
}
#endif
#endif /* INTERFACE_UART_H */
