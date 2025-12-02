#ifndef INTERFACE_DMA2D_H
#define INTERFACE_DMA2D_H
#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/ProjDefine.h"
#include "Inc/typedef.h"
#include "drivers/common/drivers_common.h"
/* function prototypes -----------------------------------------------*/
enum DMA_CTL
{
    DMA2D_FILL,
    DMA2D_COPY,
    DMA2D_FREE_COPY,
    DMA2D_BLEND,
};

extern int dma2d_open(device_t *self);
extern int dma2d_close(device_t *self);
extern int dma2d_ctl(device_t *self, int cmd, va_list ap);




#ifdef __cplusplus
}
#endif
#endif 

