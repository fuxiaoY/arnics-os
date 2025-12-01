
#include "../../common/drivers_list.h"
#include "../inc/interface_ltdc.h"
#include "ltdc.h"

int ltdc_open(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    return bsp_ltdc_init(self->device);
}
int ltdc_close(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    return bsp_ltdc_close(self->device);
}


