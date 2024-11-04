#include "eventCore.h"
#include "eventList.h"
#include "../../../core/coreInclude.h"
#include "../../../thirdParty/thirdPartyInclude.h"
#include "../../../rtosInterface/entry_rtos_api.h"

#pragma arm section code = "._entry_event_api"
DEFINE_ARNICS_FUNC_ITEM_RANGE(arnics_event_item, EVENT_TAG, 0, 2);


EVENT_STATE event_state = OnWattingOutMsg;
uint32_t EVENT_FLAG = 0; // 外部事件标志
Message_t mesg_cache = {0};//事件应用消息

// 定义事件位映射表
EventBitMapping eventBitMapping[EVENT_MAX_NUM];
// 用户事件
RegisterEntry registerTable[] = 
{
    REGISTER_ENTRIES
};
static uint32_t getRegisterTableNum()
{
    return (sizeof(registerTable) / sizeof(RegisterEntry));
}


#undef X

// 定义 X 宏来调用 ARNICS_REGISTER
#define X(func, priority, needRsp) \
    ARNICS_REGISTER(#func, func, EVENT_TAG, priority);
// 调用 X 宏
REGISTER_ENTRIES
#undef X
/*--------------------------------------------------------*/
// event core


void event_list_register()
{
    // 建立映射关系
    for (size_t i = 0; i < getRegisterTableNum(); ++i)
    {
        eventBitMapping[i].name = registerTable[i].name;
        eventBitMapping[i].event_bit = i;
    }
}

void event_init(void)
{
    CLR_EVENT_FLAG_ALL(EVENT_FLAG);
    event_list_register();
}

void event_exec(uint32_t event_flag,void *argv)
{

    // 遍历查找匹配的 event_bit
    for (size_t i = 0; i < getRegisterTableNum(); i++)
    {
        if (event_flag ==  eventBitMapping[i].event_bit)
        {
            // 找到匹配的 event_bit，执行对应的函数
            EXECUTE_FUNC_BY_NAME(arnics_event_item, eventBitMapping[i].name,argv);
        }
    }
}

// 获取注册表中对应 name 的索引
static int getRegisterEntryIndex(const char *name) 
{
    for (size_t i = 0; i < getRegisterTableNum(); i++) 
    {
        if (strcmp(registerTable[i].name, name) == 0) 
        {
            return i;
        }
    }
    return -1; // 未找到
}


// 用于设置事件标志 
bool set_event_flag(uint32_t *eventflag, const char *name) 
{
    int index = getRegisterEntryIndex(name);
    if (index == -1) {
        return false; // 未找到对应的 name
    }

    // 设置 eventflag 中对应位置的位
    *eventflag  = (uint32_t)index;
    return true;
}
/*-------------------------------------------------------------------------------------*/

/*任务主函数-----------------------------------------------------------------------------*/
_WEAK void onWaittingOutMessage()
{
    while (1)
    {
        memset(&mesg_cache,0,sizeof(Message_t));
        eventosWantSleep = TRUE; // 现在提休眠申请
        ULOG_DEBUG("eventCenter:Waiting for Message...");
        ULOG_DEBUG("-----------------END------------------------");
        if (true == rtosEventosGetMsg(&mesg_cache,BLOCK_DELAY))
        {
            eventosWantSleep = FALSE; // 撤销休眠申请
            // 清除分析缓存，并存入外部消息
            CLR_EVENT_FLAG_ALL(EVENT_FLAG);
            EVENT_FLAG = mesg_cache.eventflag;
            // 处理接收到的消息
            ULOG_DEBUG("-----------------START----------------------");
            ULOG_DEBUG("ON_Waitting_OUT_Message::Got Message! ID=%d", mesg_cache.ID_Ts);
            event_state = ActionMsg; // 进入消息处理状态
            break;
        }
        rtosThreadDelay(10);
    }
}

_WEAK void eventAction()
{
    while (1)
    {
        event_exec(EVENT_FLAG,mesg_cache.buf); // 此处将里面的每一项需求分析出来，并分发任务
        ULOG_DEBUG("eventCenter: analyzeSampleNeed Done!");
        rtosThreadDelay(10);
        event_state = SendingRspMsg; // 进入发送响应消息状态
        break;
    }
}

_WEAK void onResetState()
{
    Message_t msg = {0};
    bool needRsp = false;

    for (size_t i = 0; i < getRegisterTableNum(); i++)
    {
        if (EVENT_FLAG ==  eventBitMapping[i].event_bit)
        {
            if (true == registerTable[i].needRsp)
            {
                needRsp = true;
                break;
            }
        }
    }
    while (1)
    {
        if (needRsp)
        {
            if (rtosEventosSendMsg(&msg, 500)) // 阻塞500ms发送 直到尝试成功才会切换状态，保证消息能够发出
            {
                // 发送成功 一个状态结束
                ULOG_DEBUG("Message sent eventosRspQueue succeed! ID=%d", mesg_cache.ID_Ts);
                memset(&mesg_cache, 0, sizeof(Message_t));
                CLR_EVENT_FLAG_ALL(EVENT_FLAG);
                event_state = OnWattingOutMsg; // 进入等待消息状态
                break;
            }
            else
            {
                ULOG_DEBUG("Retry sendding!!");
                rtosThreadDelay(10);
            }
            rtosThreadDelay(10);
        }
        else
        {
            CLR_EVENT_FLAG_ALL(EVENT_FLAG);
            event_state = OnWattingOutMsg; // 进入等待消息状态
            break;
        }

        break;
    }
}

/*-------------------------------------------------------------------------------------*/

/*外部消息接口---------------------------------------------------------------------------*/

static unsigned long global_id_counter = 0;

/**
 * @brief  向事件中心发送消息
 * @param  无
 * @retval 
 */
uint32_t SendEventCallToEventCenter(uint32_t eventflag,void *argv,size_t len, time_t wait)
{
    // 发送消息到事件队列
    // 创建消息
    Message_t msg = {0};
    // 获取互斥信号量
    if (TakeEventosMutex(wait))
    {
        // 生成唯一ID
        msg.ID_Ts = ++global_id_counter;
        // 释放互斥信号量
        ReleaseEventosMutex();
    }
    msg.eventflag = eventflag;
    msg.length = len;
    memcpy(msg.buf, argv, len);
    // 等待时间为wait
    if (rtosDeliverMsgToEventos(&msg, wait) == true)
    {
        ULOG_DEBUG("Message:needSample sent succeed! ID=%d",msg.ID_Ts);
        return msg.ID_Ts;
    }
    else
    {
        ULOG_DEBUG("Message:needSample sent timeout failed!");
        return 0;
    }
}

/**
 * @brief  向事件中心取出消息
 * @param  无
 * @retval 
 * @attention 采用信号量保证只有一个任务可以访问该函数，会自动回填消息
 */
bool GetResponseMessageFromEventCenter(time_t ID, time_t wait)
{
    Message_t receivedMsg;
    uint32_t numMessages;
    bool receiveStatus;
    bool foundMessage = FALSE;

    // 无限循环遍历消息队列
    while (1)
    {
        numMessages = CheckEventRspMesgNum();
        for (uint32_t i = 0; i < numMessages; i++)
        {
            // 使用队列的Peek功能查看消息而不取出
            receiveStatus = PeekEventRspMesg(&receivedMsg);
            if (receiveStatus == true)
            {
                if (receivedMsg.ID_Ts == ID)
                {
                    foundMessage = TRUE;
                    break;
                }
            }
        }

        // 如果找到消息，退出循环
        if (foundMessage)
        {
            break;
        }

        // 如果没有找到，延时一段时间再重试
        rtosThreadDelay(10);
    }

    // 获取信号量
    if (TakeEventosMsgQueueMutex(wait))
    {
        // 查找并取出属于自己的消息
        for (uint32_t i = 0; i < numMessages; i++)
        {
            receiveStatus = rtosTakeMsgFromEventos( &receivedMsg, BLOCK_DELAY);
            if (receiveStatus == true)
            {
                if (receivedMsg.ID_Ts == ID)
                {
                    // 找到消息，释放信号量并返回 TRUE
                    ReleaseEventosMsgQueueMutex();
                    return TRUE;
                }
                else
                {
                    // 如果不是自己的消息，重新放回队列
                    rtosEventosSendMsg( &receivedMsg, BLOCK_DELAY);
                }
            }
            else
            {
                // 接收失败
                break; // 无需继续循环
            }
        }

        // 没有找到消息，释放信号量并返回 FALSE
        ReleaseEventosMsgQueueMutex();
        return FALSE;
    }
    else
    {
        // 如果获取信号量失败，返回 FALSE
        return FALSE;
    }
}

// 定义状态列表
const static SEPARATE_STATE EventList[] = {
    SEPARATE_DEFINE_STATE(OnWattingOutMsg, onWaittingOutMessage),
    SEPARATE_DEFINE_STATE(ActionMsg, eventAction),
    SEPARATE_DEFINE_STATE(SendingRspMsg, onResetState)};

// 默认的回调函数
static void SEPARATE_DEFAULT_NOT_FOUND_CALLBACK()
{
    event_state = OnWattingOutMsg;
}

// 初始化内核
SEPARATE_INIT_KERNEL(EventList, EVENT_STATE, &(event_state), SEPARATE_DEFAULT_NOT_FOUND_CALLBACK);

/*-------------------------------------------------------------------------------------*/

/*状态主函数-----------------------------------------------------------------------------*/
void event_process()
{
    event_init();

    // 运行内核
    SEPARATE_RUN_KERNEL(1);
}

