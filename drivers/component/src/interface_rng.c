#include "../inc/interface_rng.h"
#include "../../bsp/bsp_def.h"
#include "rng.h"

int rng_open(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    bsp_rng_init(self->device);
    return 0;
}

int rng_close(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    bsp_rng_deinit(self->device);
    return 0;
}

int rng_read(device_t *self, void *buf, size_t size)
{
    if(self->ds == 0)
    {
        return -1;
    }
    return bsp_rng_read(self->device, buf, size);
}
