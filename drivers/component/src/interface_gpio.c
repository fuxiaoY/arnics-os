#include "../inc/interface_gpio.h"
#include "gpio.h"


void gpio_init(device_t *self)
{
    if(self->ds == 0)
    {
        return;
    }
    bsp_gpio_init(self->device);
}
void gpio_close(device_t *self,unsigned int statue)
{
    if(self->ds == 0)
    {
        return;
    }
    bsp_gpio_close(self->device,statue);
}
void gpio_set(device_t *self,unsigned int statue)
{
    if(self->ds == 0)
    {
        return;
    }
    bsp_gpio_set(self->device,statue);
}

void gpio_toggle(device_t *self)
{
    bsp_gpio_toggle(self->device);
}
