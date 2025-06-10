#include "../../common/drivers_list.h"
#include "../inc/interface_gpio.h"
#include "gpio.h"




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


int gpio_ctl(device_t *self, int cmd, va_list ap)
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
        case IO_FREECFG:
        {
            uint32_t gpio_mode = (uint32_t)va_arg(ap, int);
            uint32_t gpio_pull = (uint32_t)va_arg(ap, int);
            uint32_t gpio_speed = (uint32_t)va_arg(ap, int);    
            uint8_t  gpio_PinState = (uint8_t)va_arg(ap, int); 
            bsp_gpio_free_cfg(self->device,gpio_mode,gpio_pull,gpio_speed,gpio_PinState);
            break;
        }
        case IO_NVIC_DISABLE:
        {
            bsp_gpio_irq_disable(self->device);
            break;
        }
        default: return -1; 
    }
    return 0;
}
void gpio_irq(device_t *self)
{
    if(self->ds == 0)
    {
        return;
    }
    bsp_gpio_irq(self->device);
}
