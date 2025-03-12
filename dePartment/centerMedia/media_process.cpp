#include "media_process.h"
_SECTION( "._entry_media_api")





/*-------------------------------------------------------------------------------------*/

/*状态主函数-----------------------------------------------------------------------------*/


void media_process()
{
    while(1)
    {
        rtosThreadDelay(1000);
    }
}

/*-------------------------------------------------------------------------------------*/

/*外部消息接口---------------------------------------------------------------------------*/
static uint32_t global_media_id_counter = 0;
/**
 * @brief 从消息队列中读取消息
 * @param msg 指向存储读取消息的缓冲区
 * @param wait 等待信号量的时间
 * @retval 如果成功读取消息返回 TRUE，否则返回 FALSE
 */
bool readMediaReq(MediaMessage_t *msg, time_t wait)
{
    // 检查队列是否为空
    if (CheckMediaRspMesgNum() == 0) {
        return false;
    }

    // 获取信号量
    if (TakeMediaMutex(wait) == true)
    {
        // 尝试从队列中读取消息
        if (rtosMediaGetMsg(msg, 0) == true)
        {
            ReleaseMediaMsgQueueMutex(); // 释放信号量
            return TRUE;
        }
        ReleaseMediaMsgQueueMutex(); // 释放信号量
    }

    return FALSE;
}

/**
 * @brief  向媒体接收消息队列中写入消息，如果队列已满则丢弃最旧的消息
 * @param  无
 * @retval
 * @attention
 */
void SendMsgToMediaCenter(MediaMessage_t *message)
{
    // 获取互斥信号量，确保同一时间只有一个线程可以操作消息队列
    if (TakeMediaMutex(BLOCK_DELAY) == true)
    {
        // 检查消息队列是否已满
        if (CheckMediaQueueSpacesAvailable() == 0)
        {
            // 如果队列已满，则先接收一条消息
            MediaMessage_t oldMessage;
            rtosMediaGetMsg(&oldMessage, 0);
        }
        // 生成唯一ID
        message->ID_Ts = ++global_media_id_counter;
        // 发送新的消息到队列中
        rtosDeliverMsgToMedia(message, BLOCK_DELAY);

        // 释放互斥信号量
        ReleaseMediaMsgQueueMutex();
    }
}
