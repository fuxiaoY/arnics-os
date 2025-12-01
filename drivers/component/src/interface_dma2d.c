
#include "../../common/drivers_list.h"
#include "../inc/interface_dma2d.h"
#include "../../bsp/bsp_def.h"
#include "dma2d.h"


int dma2d_open(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    return bsp_dma2d_init(self->device);
}
int dma2d_close(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    return bsp_dma2d_close(self->device);
}

int dma2d_ctl(device_t *self, int cmd, va_list ap)
{
    if(self->ds == 0)
    {
        return -1;
    }
    switch(cmd)
    {
        case DMA2D_FILL:
        {
            uint32_t xPos    = (uint32_t)va_arg(ap, int);
            uint32_t yPos    = (uint32_t)va_arg(ap, int);
            uint32_t xSize   = (uint32_t)va_arg(ap, int);
            uint32_t ySize   = (uint32_t)va_arg(ap, int);
            uint32_t color   = (uint32_t)va_arg(ap, int);
            uint32_t timeout = (uint32_t)va_arg(ap, int);
            return bsp_dma2d_fill_buffer(self->device,
                                         xPos,
                                         yPos,
                                         xSize,
                                         ySize,
                                         color,
                                         timeout);
            
        }
        case DMA2D_COPY:
        {
            uint32_t* pSrc   = (uint32_t*)va_arg(ap, void*);
            uint32_t xPos    = (uint32_t)va_arg(ap, int);
            uint32_t yPos    = (uint32_t)va_arg(ap, int);
            uint32_t xSize   = (uint32_t)va_arg(ap, int);
            uint32_t ySize   = (uint32_t)va_arg(ap, int);
            uint32_t timeout = (uint32_t)va_arg(ap, int);
            return bsp_dma2d_copy_buffer(self->device,
                                         pSrc, 
                                         xPos, 
                                         yPos, 
                                         xSize, 
                                         ySize, 
                                         timeout);
        }
        case DMA2D_FREE_COPY:
        {
            uint32_t* pSrc   = (uint32_t*)va_arg(ap, void*);
            uint32_t xPos    = (uint32_t)va_arg(ap, int);
            uint32_t yPos    = (uint32_t)va_arg(ap, int);
            uint32_t xSize   = (uint32_t)va_arg(ap, int);
            uint32_t ySize   = (uint32_t)va_arg(ap, int);
            uint32_t timeout = (uint32_t)va_arg(ap, int);
            return bsp_dma2d_free_copy_buffer(self->device,
                                         pSrc, 
                                         xPos, 
                                         yPos, 
                                         xSize, 
                                         ySize, 
                                         timeout);
        }
        case DMA2D_BLEND:
        {
            uint16_t* background_buffer = (uint16_t*)va_arg(ap, void*);
            uint16_t* foreground_buffer = (uint16_t*)va_arg(ap, void*);
            uint32_t xPos               = (uint32_t)va_arg(ap, int);
            uint32_t yPos               = (uint32_t)va_arg(ap, int);
            uint32_t xSize              = (uint32_t)va_arg(ap, int);
            uint32_t ySize              = (uint32_t)va_arg(ap, int);
            uint32_t timeout            = (uint32_t)va_arg(ap, int);
            return bsp_dma2d_blending(self->device,
                                        background_buffer, 
                                        foreground_buffer, 
                                        xPos,  
                                        yPos,  
                                        xSize,  
                                        ySize,  
                                        timeout);
                    }
        default: return -1; 
    }
}

