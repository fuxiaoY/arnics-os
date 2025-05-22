#include "ad_process.h"
_SECTION( "._entry_ad_api")
administrativePart_t ad_state = AD_SLEEP;
/*-------------------------------------------------------------------------------------*/

/*状态主函数-----------------------------------------------------------------------------*/
/**
 * @brief 行政管理主处理函数
 */
extern void sleep_process();
// 定义状态列表 
const static SEPARATE_STATE adList[] = {
    SEPARATE_DEFINE_STATE(AD_SLEEP, sleep_process),
};

// 默认的回调函数 
static void SEPARATE_DEFAULT_NOT_FOUND_CALLBACK() {
    ad_state = AD_SLEEP;
}
// 初始化内核
SEPARATE_INIT_KERNEL(adList, administrativePart_t, &ad_state, SEPARATE_DEFAULT_NOT_FOUND_CALLBACK);


void ad_process()
{          
    SEPARATE_RUN_KERNEL(1);
}

/*-------------------------------------------------------------------------------------*/

/*外部消息接口---------------------------------------------------------------------------*/
static uint32_t global_ad_id_counter = 0;
/**
 * @brief 从消息队列中读取消息
 * @param msg 指向存储读取消息的缓冲区
 * @param wait 等待信号量的时间
 * @retval 如果成功读取消息返回 TRUE，否则返回 FALSE
 */
bool readAdReq(adMessage_t *msg, time_t wait)
{
    // 检查队列是否为空
    if (CheckAdReqMesgNum() == 0) {
        return false;
    }

    // 获取信号量
    if (TakeAdMsgQueueMutex(wait) == true)
    {
        // 尝试从队列中读取消息
        if (rtosAdGetMsg(msg, 0) == true)
        {
            ReleaseAdMsgQueueMutex(); // 释放信号量
            return true;
        }
        ReleaseAdMsgQueueMutex(); // 释放信号量
    }

    return false;
}

/**
 * @brief  向行政中心消息队列中写入消息，如果队列已满则丢弃最旧的消息
 * @param  无
 * @retval
 * @attention
 */
void SendMsgToAdCenter(adMessage_t *message)
{
    // 获取互斥信号量，确保同一时间只有一个线程可以操作消息队列
    if (TakeAdMsgQueueMutex(BLOCK_DELAY) == true)
    {
        // 检查消息队列是否已满
        if (CheckAdQueueSpacesAvailable() == 0)
        {
            // 如果队列已满，则先接收一条消息
            adMessage_t oldMessage = {0};
            rtosAdGetMsg(&oldMessage, 0);
        }
        // 生成唯一ID
        message->ID_Ts = ++global_ad_id_counter;
        // 发送新的消息到队列中
        rtosDeliverMsgToAd(message, BLOCK_DELAY);

        // 释放互斥信号量
        ReleaseAdMsgQueueMutex();
    }
}



