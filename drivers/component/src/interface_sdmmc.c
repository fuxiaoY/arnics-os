#include "../../common/drivers_list.h"
#include "../inc/interface_sdmmc.h"
#include "../../bsp/bsp_def.h"
#include "sdmmc.h"


int sdmmc_open(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    return bsp_sdmmc_init(self->device);
}
int sdmmc_close(device_t *self)
{
    if(self->ds == 0)
    {
        return -1;
    }
    bsp_sdmmc_close(self->device);
    return 0;
}



int sdmmc_ctl(device_t *self, int cmd,va_list ap)
{
    if(self->ds == 0)
    {
        return -1;
    }  
    switch(cmd)
    {
        case SDMMC_READ:
        {
            uint32_t* p_data        = va_arg(ap, uint32_t*);
            uint32_t read_addr     = (uint32_t)va_arg(ap, int); 
            uint32_t num_of_blocks = (uint32_t)va_arg(ap, int); 
            uint32_t timeout       = (uint32_t)va_arg(ap, int);
            return bsp_sdmmc_read(self->device,p_data,read_addr,num_of_blocks,timeout);
        }
        case SDMMC_WRITE:
        {
            uint32_t* p_data        = va_arg(ap, uint32_t*);
            uint32_t write_addr    = (uint32_t)va_arg(ap, int); 
            uint32_t num_of_blocks = (uint32_t)va_arg(ap, int); 
            uint32_t timeout       = (uint32_t)va_arg(ap, int);
            return bsp_sdmmc_write(self->device,p_data,write_addr,num_of_blocks,timeout);
        }
        case SDMMC_ERASE:
        {
            uint32_t start_addr    = (uint32_t)va_arg(ap, int); 
            uint32_t end_addr      = (uint32_t)va_arg(ap, int);
            return bsp_sdmmc_erase(self->device,start_addr,end_addr);
        }
        case SDMMC_GET_CARD_INFO:
        { 
            void* p_card_info = va_arg(ap, void*);
            bsp_sdmmc_get_card_info(self->device,p_card_info);
        }
        break;
        case SDMMC_GET_CARD_STATUS:
        { 
            return bsp_sdmmc_get_card_state(self->device);  
        }

        default: return -1; 
    }
    return 0;
}

