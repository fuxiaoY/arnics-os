#include "../inc/interface_gpio.h"
#include "gpio.h"



extern int gpio_read(device_t *self,void *buf, size_t count);
extern int gpio_ctl(device_t *self, int cmd, void *args);
int gpio_open(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    bsp_gpio_init(self->device);
    return 0;
}
int gpio_close(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    bsp_gpio_close(self->device);
    return 0;
}


int gpio_ctl(device_t *self, int cmd, void *args)
{
    if(self->ds == 0)
    {
        return -1;
    }
    switch(cmd)
    {
        case IO_TOGGLE:
        {
            bsp_gpio_toggle(self->device);
            break;
        }
        case IO_SET:
        {
            bsp_gpio_set(self->device,1);
            break;
        }
        case IO_RESET:
        {
            bsp_gpio_set(self->device,0);
            break;
        }
        case IO_READ:
        {
            return bsp_gpio_get(self->device);

        }
        default: return -1; 
    }
    return 0;
}
