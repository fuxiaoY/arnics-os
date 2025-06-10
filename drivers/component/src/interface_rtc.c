#include "../../common/drivers_list.h"
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

int rtc_close(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    bsp_rtc_deinit(self->device);
    return 0;
}

int rtc_ctl(device_t *self, int cmd, va_list ap)
{
    if(self->ds == 0)
    {
        return -1;
    } 
switch(cmd) {
    case RTC_GETDATETIME: 
    {
        void* arg1 = va_arg(ap, void*);
        return bsp_rtc_get_datetime(self->device, arg1);
    }
    case RTC_SETDATETIME: 
    {
        void* arg1 = va_arg(ap, void*);
        return bsp_rtc_set_datetime(self->device, arg1);
    }
    case RTC_SECONDUPDATE: 
    {
        void* arg1 = va_arg(ap, void*);
        return bsp_rtc_check_second_update(self->device);
    }
    case RTC_ALARM_SET: 
    {
        uint32_t sleep_ms = (uint32_t)va_arg(ap, int); 
        bsp_rtc_alarm_set(self->device, sleep_ms);
        break;
    }
    case RTC_ALARM_CLEAR: 
    {
        bsp_rtc_alarm_clear(self->device);
        break;
    }
    case RTC_WAKEUP_SET: 
    {
        uint32_t sleep_ms = (uint32_t)va_arg(ap, int); 
        bsp_rtc_wakeup_set(self->device, sleep_ms);
        break;
    }
    case RTC_WAKEUP_CLEAR: 
    {
        bsp_rtc_wakeup_clear(self->device);
        break;
    }
    default: 
        return -1;
}
    return 0;
}

void rtc_irq(device_t *self)
{
    if(self->ds == 0)
    {
        return;
    }
    bsp_rtc_irq(self->device);
}
