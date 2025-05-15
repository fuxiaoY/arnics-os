

#include "../inc/interface_adc.h"
#include "adc.h"
int adc_open(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    bsp_adc_init(self->device);
    return 0;
}
int adc_close(device_t *self)
{
    bsp_adc_close(self->device);
    return 0;
}

//注意adc采样需要uint16_t 类型
int adc_read(device_t *self,void *buf, size_t count)
{
    if(self->ds == 0)
    {
        return -1;
    }
    return bsp_adc_read(self->device,(uint16_t*)buf,count);
}
