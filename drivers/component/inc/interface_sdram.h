#ifndef INTERFACE_SDRAM_H
#define INTERFACE_SDRAM_H
#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/ProjDefine.h"
#include "Inc/typedef.h"
#include "drivers/common/drivers_common.h"
/* function prototypes -----------------------------------------------*/


extern int sdram_open(device_t *self);
extern int sdram_close(device_t *self);




#ifdef __cplusplus
}
#endif
#endif 

