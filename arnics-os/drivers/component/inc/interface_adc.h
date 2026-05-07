#ifndef INTERFACE_ADC_H
#define INTERFACE_ADC_H
#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#ifdef DRIVERS_ENABLE_ADC
#include "drivers/common/drivers_common.h"
/* function prototypes -----------------------------------------------*/


extern int adc_open(device_t *self);
extern int adc_close(device_t *self);
extern int adc_read(device_t *self,void *buf, size_t count);


#endif
#ifdef __cplusplus
}
#endif
#endif /* INTERFACE_adc_H */
