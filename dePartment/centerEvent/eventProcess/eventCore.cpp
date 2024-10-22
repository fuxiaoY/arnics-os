#include "eventCore.h"
#include "eventList.h"
#include "eventMessage.h"

#include "../../../rtosInterface/rtosInclude.h"
#include "../../../thirdParty/thirdPartyInclude.h"
#pragma arm section code = "._entry_event_api"
DEFINE_ARNICS_FUNC_ITEM_RANGE(arnics_event_item, EVENT_TAG, 0, 2);

// 外部消息id
time_t ID_Ts;

EVENT_STATE event_state = OnWattingOutMsg;
uint32_t EVENT_FLAG = 0; // 外部事件标志
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

void event_exec(uint32_t event_flag)
{

    // 遍历查找匹配的 event_bit
    for (size_t i = 0; i < getRegisterTableNum(); i++)
    {
        if (event_flag ==  eventBitMapping[i].event_bit)
        {
            // 找到匹配的 event_bit，执行对应的函数
            EXECUTE_FUNC_BY_NAME(arnics_event_item, eventBitMapping[i].name);
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
        Message_t msg = {0};
        eventosWantSleep = TRUE; // 现在提休眠申请
        ULOG_DEBUG("eventCenter:Waiting for Message...");
        ULOG_DEBUG("-----------------END------------------------");
        if (BlockingQueueReceive(eventosReceiveQueue, &msg) == pdTRUE)
        {
            eventosWantSleep = FALSE; // 撤销休眠申请
            ID_Ts = msg.ID_Ts;        // 记录消息ID
            // 清除分析缓存，并存入外部消息
            CLR_EVENT_FLAG_ALL(EVENT_FLAG);
            memcpy(&EVENT_FLAG, msg.buf, sizeof(uint32_t));
            // 处理接收到的消息
            ULOG_DEBUG("-----------------START----------------------");
            ULOG_DEBUG("ON_Waitting_OUT_Message::Got Message! ID=%d", ID_Ts);
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
        event_exec(EVENT_FLAG); // 此处将里面的每一项需求分析出来，并分发任务
        ULOG_DEBUG("eventCenter: analyzeSampleNeed Done!");
        osDelay(10);
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
            msg.ID_Ts = ID_Ts;
            if (xQueueSend(eventosSendQueue, &msg, 500) == pdPASS) // 阻塞500ms发送 直到尝试成功才会切换状态，保证消息能够发出
            {
                // 发送成功 一个状态结束
                ULOG_DEBUG("Message sent eventosSendQueue succeed! ID=%d", msg.ID_Ts);
                ID_Ts = 0; // 清除消息ID，代表消息已发送
                CLR_EVENT_FLAG_ALL(EVENT_FLAG);
                event_state = OnWattingOutMsg; // 进入等待消息状态
                break;
            }
            else
            {
                ULOG_DEBUG("Retry sendding!!");
                rtosThreadDelay(10);
            }
            osDelay(10);
        }
        else
        {
            ID_Ts = 0; // 清除消息ID，代表消息已发送
            CLR_EVENT_FLAG_ALL(EVENT_FLAG);
            event_state = OnWattingOutMsg; // 进入等待消息状态
            break;
        }

        break;
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

