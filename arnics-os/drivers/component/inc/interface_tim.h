#ifndef INTERFACE_TIM_H
#define INTERFACE_TIM_H
#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/projDefine.h"
#include "Inc/typedef.h"


/* function prototypes -----------------------------------------------*/

enum TIM_CTL
{
  TIM_RELOAD = 0,
};

#ifdef DRIVERS_ENABLE_TIM
#include "drivers/common/drivers_common.h"
extern int tim_open(device_t *self);
extern int tim_close(device_t *self);
extern int tim_ctl(device_t *self, int cmd, va_list ap);
extern void tim_irq(device_t *self);



#endif
#ifdef __cplusplus
}
#endif
#endif /* INTERFACE_TIM_H */
  
