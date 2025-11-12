
#include "../../common/drivers_list.h"
#include "../inc/interface_delay.h"
#include "../../bsp/bsp_def.h"
#include "dwt.h"

int delay_open(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    bsp_delay_init(self->device);
    return 0;
}

int delay_close(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }

    bsp_delay_deinit(self->device);
    return 0;
}

int delay_ctl(device_t *self, int cmd,va_list ap)
{
    if(self->ds == 0)
    {
        return -1;
    }  
    switch(cmd)
    {
        case DELAY_MS:
        {
            uint32_t delay_ms = va_arg(ap, int);
            bsp_delay_ms(self->device,delay_ms);
            return 0;
        }
        case DELAY_US:
        {
            uint32_t delay_us = va_arg(ap, int);
            bsp_delay_us(self->device,delay_us);
            return 0;
        }
        case DELAY_MONITOR_CAL:
        {
            uint32_t start = va_arg(ap, int);
            uint32_t   end = va_arg(ap, int);
            return dwt_get_elapsed_time_us(start,end);
        }
        case DELAY_MONITOR_GET:
        {
            return dwt_time_get();
        }
        case DELAY_MONITOR_CLR:
        {
            dwt_time_clear();
            return 0;
        }
        default: return -1; 
    }
}

