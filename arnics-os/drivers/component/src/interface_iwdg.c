#include "../inc/interface_iwdg.h"
#include "iwdg.h"

int iwdg_open(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    bsp_iwdg_init(self->device);
    return 0;
}
int iwdg_close(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    bsp_iwdg_deinit(self->device);
    return 0;
}
int iwdg_ctl(device_t *self, int cmd,va_list ap)
{
    if(self->ds == 0)
    {
        return -1;
    }
    switch(cmd)
    {
        case IWDG_RELOAD:
            bsp_iwdg_refresh(self->device);
            return 0;

        default:
            return -1;
    }
}
