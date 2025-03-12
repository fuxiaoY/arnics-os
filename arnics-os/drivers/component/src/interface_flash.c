
#include "../inc/interface_flash.h"
#include "flash.h"
/*
extern int bsp_flash_read(flash_t *dev, uint8_t *buf, size_t size);

extern int bsp_flash_write(flash_t *dev, const uint8_t *buf, size_t size);

extern int flash_erase(flash_t *dev);
*/
int flash_open(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    return 0;
}
int flash_close(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    return 0;
}
int flash_read(device_t *self,void *buf, size_t count)
{
    if(self->ds == 0)
    {
        return -1;
    }
    return bsp_flash_read(self->device,buf,count);
}
int flash_write(device_t *self, void *buf, size_t count)
{
    if(self->ds == 0)
    {
        return -1;
    }
    return bsp_flash_write(self->device,buf,count);
}
int flash_ctl(device_t *self, int cmd, va_list ap)
{
    void* arg1 = va_arg(ap, void*);
    if(self->ds == 0)
    {
        return -1;
    }
    switch(cmd)
    {
        case FLASH_EREASE:
        {
            return flash_erase(self->device,arg1);
        }
        default: return -1; 
    }

}


