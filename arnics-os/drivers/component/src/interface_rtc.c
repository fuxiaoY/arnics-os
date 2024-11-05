#include "../inc/interface_rtc.h"
#include "../../bsp/bsp_def.h"
#include "rtc.h"



int rtc_open(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    bsp_rtc_init(self->device);
    return 0;
}

extern int rtc_close(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    bsp_rtc_deinit(self->device);
    return 0;
}

extern int rtc_ctl(device_t *self, int cmd, void *args)
{
    if(self->ds == 0)
    {
        return -1;
    } 
    switch(cmd)
    {
        case RTC_GETDATETIME:
        return bsp_rtc_get_datetime(self->device,args);
        case RTC_SETDATETIME:
        return bsp_rtc_set_datetime(self->device,args);
        case RTC_SECONDUPDATE:
        return bsp_rtc_check_second_update(self->device);
        default: return -1; 
        
    }
}
