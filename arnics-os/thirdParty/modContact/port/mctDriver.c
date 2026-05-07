#include "mctDriver.h"
#include "drivers/driversInclude.h"
#include "rtosInterface/entry_rtos_api.h"
#include "port/arnicsMalloc.h"
//并不会阻塞整个系统的调度
void MCT_DELAY(uint32_t time)
{
    rtosThreadDelayUntil(time);
}
void MCT_MALLOC(void **buf, uint32_t size)
{
    *buf = arnicsMalloc(size);
}
void MCT_FREE(void **buf)
{
    if(*buf != NULL)
    {
        arnicsFree(*buf);
        *buf = NULL;
    }
}
/* define ------------------------------------------------------------*/
/* command ----------------------------------------------------------*/

uint32_t command_mct_read(uint8_t *buf, uint16_t maxlen) 
{


    return 0;
}

int command_mct_write(void* file, uint16_t len)
{
    if ((NULL == file) || (0 == len))
    {
        return -1;
    }
    return 0;
}

