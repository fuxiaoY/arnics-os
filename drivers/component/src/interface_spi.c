#include "../inc/interface_spi.h"
#include "../../common/drivers_common.h"
#include "../../bsp/bsp_def.h"
#include "spi.h"


int spi_open(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    bsp_spi_init(self->device);
  
    return 0;
}
int spi_close(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    bsp_spi_close(self->device);
    return 0;
}

int spi_read(device_t *self,void *buf, size_t count)
{
    if(self->ds == 0)
    {
        return -1;
    }

    return bsp_spi_read(self->device, buf, count);
}

int spi_write(device_t *self, void *buf, size_t count)
{
    if(self->ds == 0)
    {
        return -1;
    }

    return bsp_spi_write(self->device, buf, count);

}
