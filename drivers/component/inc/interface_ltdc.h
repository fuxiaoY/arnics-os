#ifndef INTERFACE_LTDC_H
#define INTERFACE_LTDC_H
#ifdef __cplusplus
extern "C" {
#endif
#include "../../../Inc/ProjDefine.h"
#include "../../../Inc/typedef.h"
#include "../../common/drivers_common.h"

/* function prototypes -----------------------------------------------*/



extern int ltdc_open(device_t *self);
extern int ltdc_close(device_t *self);


#ifdef __cplusplus
}
#endif
#endif /* INTERFACE_LTDC_H */
