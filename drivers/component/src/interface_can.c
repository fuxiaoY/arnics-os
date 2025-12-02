#include "../../common/drivers_list.h"
#include "../inc/interface_can.h"
#include "../../bsp/bsp_def.h"
#include "can.h"

#ifdef _USE_RING_
#include "../../../thirdParty/ring/ring.h"
#endif
int can_open(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    bsp_can_init(self->device);
    can_t *can = (can_t *)self->device;
    ring_init(can->ring1_tx,can->tx1_buf,can->tx_buf1_size,can->ring1_rx,can->rx1_buf,can->rx_buf1_size);
    return 0;
}
int can_close(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    bsp_can_close(self->device);
    return 0;
}

int can_read(device_t *self,void *buf, size_t count)
{
    if(self->ds == 0)
    {
        return -1;
    }
    return  bsp_can_read(self->device, buf, count);
	}
   
    
int can_write(device_t *self, void *buf, size_t count)
{
    if(self->ds == 0)
    {
        return -1;
    }
    return bsp_can_write(self->device, buf, count);
}

void can_irq(device_t *self)
{
    if(self->ds == 0)
    {
        return;
    }
    bsp_can_irq(self->device);
}



