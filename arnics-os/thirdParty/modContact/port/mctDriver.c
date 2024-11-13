#include "mctDriver.h"
#include "../common/mctProcesser.h"
#include "../../../drivers/driversInclude.h"
#include "../../../rtosInterface/entry_rtos_api.h"

//并不会阻塞整个系统的调度
void MCT_DELAY(uint32_t time)
{
    rtosThreadDelayUntil(time);
}


void mct_init(MctInstance* pInstance,uint8_t *Cmd_cache,size_t cmd_max_size, \
                                    uint8_t* Payload_cahe,size_t payload_max_size)
{
    //uart init
    dev_reg("uart1",&mct_ds);
    dev_open(&mct_ds);

    //instance init
    pInstance->cmd_cache = Cmd_cache;
    pInstance->payload_cache = Payload_cahe;
    pInstance->CMD_MAX_SIZE = cmd_max_size;
    pInstance->PAYLOAD_MAX_SIZE = payload_max_size;

    pInstance->cmd_size = 0;
    pInstance->payload_size = 0;
    initStaticFrameList(&pInstance->payload_list);
}    

void mct_data_reset(MctInstance* pInstance)
{
    memset(pInstance->cmd_cache,0,pInstance->CMD_MAX_SIZE);
    memset(pInstance->payload_cache,0,pInstance->PAYLOAD_MAX_SIZE);
    pInstance->cmd_size = 0;
    pInstance->payload_size = 0;
}

static uint32_t mct_read_continue(uint8_t *offset,size_t maxlen)
{
    return dev_read(&mct_ds,offset,maxlen);
}

int mct_write(void *file, uint16_t len)            
{
    if((NULL == file) || (0 == len))
    {
        return -1;
    }
    return dev_write(&mct_ds,file, len);
}

#define TIMEOUT_MS 100


uint32_t mct_read(uint8_t *buf, uint16_t maxlen) 
{
    uint32_t totalBytesRead = 0;
    uint32_t cnt = 0;
    const uint32_t maxCnt = TIMEOUT_MS / WAIT_SCHEDULE_TIME_MS;

    // 第一次尝试读取数据
    uint32_t bytesRead = mct_read_continue(buf + totalBytesRead, maxlen - totalBytesRead);
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
        bytesRead = mct_read_continue(buf + totalBytesRead, maxlen - totalBytesRead);
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
