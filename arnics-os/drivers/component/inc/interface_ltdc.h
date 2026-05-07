#ifndef INTERFACE_LTDC_H
#define INTERFACE_LTDC_H
#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#ifdef DRIVERS_ENABLE_LTDC
#include "drivers/common/drivers_common.h"

/* function prototypes -----------------------------------------------*/



extern int ltdc_open(device_t *self);
extern int ltdc_close(device_t *self);


#endif
#ifdef __cplusplus
}
#endif
#endif /* INTERFACE_LTDC_H */
