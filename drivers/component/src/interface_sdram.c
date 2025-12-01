
#include "../../common/drivers_list.h"
#include "../inc/interface_sdram.h"
#include "../../bsp/bsp_def.h"
#include "fmc.h"


int sdram_open(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    return bsp_sdram_init(self->device);
}

int sdram_close(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    return bsp_sdram_close(self->device);
}
