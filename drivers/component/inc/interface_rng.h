#ifndef INTERFACE_RNG_H
#define INTERFACE_RNG_H
#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/ProjDefine.h"
#include "Inc/typedef.h"
#include "drivers/common/drivers_common.h"

/* function prototypes -----------------------------------------------*/



extern int rng_open(device_t *self);
extern int rng_close(device_t *self);
/*!!!buf需要传递为uint32_t 类型指针，否则将出现内存覆写错误!!!*/
extern int rng_read(device_t *self,void *buf, size_t count);

#ifdef __cplusplus
}
#endif
#endif /* INTERFACE_RNG_H */
