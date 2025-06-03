#include "mctDriver.h"
#include "../../../drivers/driversInclude.h"
#include "../../../rtosInterface/entry_rtos_api.h"
#include "../../../common/usrMalloc.h"
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
    uint32_t totalBytesRead = 0;
    uint32_t cnt = 0;
    const uint32_t maxCnt = TIMEOUT_MS / WAIT_SCHEDULE_TIME_MS;

    // 第一次尝试读取数据
    uint32_t bytesRead = dev_read(&debug_ds,buf + totalBytesRead, maxlen - totalBytesRead);
    if (bytesRead > 0)
    {
        totalBytesRead += bytesRead; // 累加总共读到的字节数
    }
    else
    {
        // 第一次没有读到数据，直接退出
        return totalBytesRead;
    }

    // 进入循环，继续读取数据
    while (cnt < maxCnt)
    {
        bytesRead = dev_read(&debug_ds,buf + totalBytesRead, maxlen - totalBytesRead);
        if (bytesRead > 0)
        {
            totalBytesRead += bytesRead; // 累加总共读到的字节数
            cnt = 0; // 刷新超时计数器
        }
        else
        {
            cnt++; // 增加超时计数器
        }

        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }

    return totalBytesRead;
}

int command_mct_write(void *file, uint16_t len)            
{
    if((NULL == file) || (0 == len))
    {
        return -1;
    }
    return dev_write(&debug_ds,file, len);
}

/* define ------------------------------------------------------------*/
/* modem ----------------------------------------------------------*/

uint32_t modem_mct_read(uint8_t *buf, uint16_t maxlen) 
{
    uint32_t totalBytesRead = 0;
    uint32_t cnt = 0;
    const uint32_t maxCnt = TIMEOUT_MS / WAIT_SCHEDULE_TIME_MS;

    // 第一次尝试读取数据
    uint32_t bytesRead = dev_read(&wan_uart_ds,buf + totalBytesRead, maxlen - totalBytesRead);
    if (bytesRead > 0)
    {
        totalBytesRead += bytesRead; // 累加总共读到的字节数
    }
    else
    {
        // 第一次没有读到数据，直接退出
        return totalBytesRead;
    }

    // 进入循环，继续读取数据
    while (cnt < maxCnt)
    {
        bytesRead = dev_read(&wan_uart_ds,buf + totalBytesRead, maxlen - totalBytesRead);
        if (bytesRead > 0)
        {
            totalBytesRead += bytesRead; // 累加总共读到的字节数
            cnt = 0; // 刷新超时计数器
        }
        else
        {
            cnt++; // 增加超时计数器
        }

        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }
    if(0)
    {
        if(totalBytesRead > 0)
        {
            printf("\r\nreceive: len:%d\r\n",totalBytesRead);
            printf("%.*s \r\n", totalBytesRead, buf);
            // // 打印hex
            // printf("Hex Data: ");
            // for (uint32_t i = 0; i < totalBytesRead; i++)
            // {
            //     printf("%02X ", buf[i]);
            // }
            // printf("\r\n");
        }
    }

    return totalBytesRead;
}

int modem_mct_write(void *file, uint16_t len)            
{
    if((NULL == file) || (0 == len))
    {
        return -1;
    }
    if(0)
    {
        printf("\nsend:\r\n");
        printf("%.*s\n",len,(char *)file);
    }
    return dev_write(&wan_uart_ds,file, len);
}
