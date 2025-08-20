#include "../../common/drivers_list.h"
#include "../inc/interface_tim.h"
#include "../../bsp/bsp_def.h"
#include "tim.h"


int tim_open(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    return bsp_tim_init(self->device);
}
int tim_close(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    return bsp_tim_close(self->device);

}


int tim_ctl(device_t *self, int cmd, va_list ap)
{
    if(self->ds == 0)
    {
        return -1;
    }
    return 0;
}
void tim_irq(device_t *self)
{
    if(self->ds == 0)
    {
        return;
    }
    bsp_tim_irq(self->device);
}
