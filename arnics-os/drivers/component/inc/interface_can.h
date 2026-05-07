#ifndef INTERFACE_CAN_H
#define INTERFACE_CAN_H
#ifdef __cplusplus
extern "C" {
#endif
#include "../../../Inc/projDefine.h"
#include "../../../Inc/typedef.h"
#ifdef DRIVERS_ENABLE_CAN
#include "drivers/common/drivers_common.h"

/* function prototypes -----------------------------------------------*/


extern int can_open(device_t *self);
extern int can_close(device_t *self);
extern int can_read(device_t *self,void *buf, size_t count);
extern int can_write(device_t *self, void *buf, size_t count);
extern void can_irq(device_t *self);



#endif
#ifdef __cplusplus
}
#endif
#endif /* INTERFACE_can_H */
  
