#include "mctDriver.h"
#include "../common/mctProcesser.h"
#include "../../../drivers/driversInclude.h"
#include "../../../rtosInterface/entry_rtos_api.h"
#define TIMEOUT_MS 100
//并不会阻塞整个系统的调度
void MCT_DELAY(uint32_t time)
{
    rtosThreadDelayUntil(time);
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
