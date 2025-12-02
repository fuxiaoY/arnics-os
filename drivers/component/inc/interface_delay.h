#ifndef INTERFACE_DELAY_H
#define INTERFACE_DELAY_H
#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/ProjDefine.h"
#include "Inc/typedef.h"
#include "drivers/common/drivers_common.h"

/* function prototypes -----------------------------------------------*/


enum DELAYCTL
{
  DELAY_MS= 0,
  DELAY_US,
  DELAY_MONITOR_CAL,
  DELAY_MONITOR_GET,
  DELAY_MONITOR_CLR,
};

extern int delay_open(device_t *self);
extern int delay_close(device_t *self);
extern int delay_ctl(device_t *self, int cmd, va_list ap);




#ifdef __cplusplus
}
#endif
#endif /* INTERFACE_DELAY_H */
  
