#include "eventCore.h"
#include "eventList.h"
#include "../../../core/coreInclude.h"
#include "../../../thirdParty/thirdPartyInclude.h"
#include "../../../rtosInterface/entry_rtos_api.h"


DEFINE_ARNICS_FUNC_ITEM_RANGE(arnics_event_item, EVENT_TAG, 0, 3);
_SECTION("._entry_event_api")

static EVENT_STATE event_state = OnWattingOutMsg;
static uint32_t EVENT_FLAG = 0; // 外部事件标志
static uint32_t MSG_FLAG = 0; // 外部消息标志
static message_t mesg_cache = {0};//事件应用消息


#undef X
#define X(func, priority, needRsp) + 1
#define COUNT_TOTAL_ENTRIES (0 REGISTER_ENTRIES)
// 定义事件位映射表
static EventBitMapping eventBitMapping[COUNT_TOTAL_ENTRIES];
static uint32_t getRegisterTableNum()
{
    return (COUNT_TOTAL_ENTRIES);
}
#undef X
// 定义 X 宏
#define X(func, priority, needRsp) \
    {#func, func, priority, needRsp},

// 用户事件
static RegisterEntry registerTable[] = 
{
    REGISTER_ENTRIES
};


#undef X
// 定义 X 宏来调用 ARNICS_REGISTER
#define X(func, priority, needRsp) \
    ARNICS_REGISTER(#func, func, EVENT_TAG, priority);
// 调用 X 宏
REGISTER_ENTRIES

#undef X
#define X(func, priority, needRsp) + 1



static uint32_t events[COUNT_TOTAL_ENTRIES];
static uint32_t event_num = 0;
// 计算组合数 C(n, k)
uint32_t combination(uint32_t n, uint32_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;

    // 优化计算：C(n, k) = C(n, n-k)
    if (k > n / 2) 
    {
        k = n - k;
    }

    uint32_t result = 1;
    for (uint32_t i = 1; i <= k; ++i) 
    {
        result = result * (n - k + i) / i;
    }
    return result;
}
// 根据已有ID和新增事件，生成新的组合ID
uint32_t update_combination_id(uint32_t event_num, uint32_t current_id)
{
    // 参数校验
    if (event_num == 0 || event_num > COUNT_TOTAL_ENTRIES) {
        return current_id; // 无效事件编号
    }

    // 如果是空组合，直接返回事件编号作为ID
    if (current_id == 0) {
        return event_num;
    }

    // 最大支持组合数量检查
    if (current_id > ((1 << COUNT_TOTAL_ENTRIES) - 1)) {
        return current_id; // 超出支持范围
    }

    // 存储当前组合中的事件
    uint32_t current_events[COUNT_TOTAL_ENTRIES] = {0};
    memset(current_events, 0, sizeof(current_events));
    uint32_t current_count = 0;

    // 解码当前ID对应的事件组合
    uint32_t id = current_id;

    // 判断当前ID是否在单事件范围内
    if (id <= COUNT_TOTAL_ENTRIES) 
    {
        current_events[0] = id;
        current_count = 1;
    } else 
    {
        // 否则使用逆向解码函数还原事件组合
        uint32_t k = 1;
        while (combination(COUNT_TOTAL_ENTRIES, k) < id) {
            id -= combination(COUNT_TOTAL_ENTRIES, k++);
        }

        uint32_t last = 0;
        for (uint32_t i = 1; i <= k; ++i) {
            uint32_t v = last + 1;
            while (combination(COUNT_TOTAL_ENTRIES - v, k - i) < id) {
                id -= combination(COUNT_TOTAL_ENTRIES - v++, k - i);
            }
            current_events[current_count++] = v;
            last = v;
        }
    }

    // 检查新事件是否已存在
    for (uint32_t i = 0; i < current_count; ++i) 
    {
        if (current_events[i] == event_num) 
        {
            return current_id; // 已存在，返回原ID
        }
    }

    // 创建新组合
    uint32_t new_events[COUNT_TOTAL_ENTRIES];
    // 添加越界检查
    if (current_count >= COUNT_TOTAL_ENTRIES) 
    {
        return current_id; // 组合已满，避免越界访问
    }
    for (size_t i = 0; i < current_count; ++i) 
    {
        new_events[i] = current_events[i];
    }
    new_events[current_count++] = event_num;

    // 排序
    for (uint32_t i = 0; i < current_count; ++i) 
    {
        for (uint32_t j = i + 1; j < current_count; ++j) 
        {
            if (new_events[j] < new_events[i]) 
            {
                uint32_t temp = new_events[i];
                new_events[i] = new_events[j];
                new_events[j] = temp;
            }
        }
    }

    // 计算新组合的ID
    uint32_t new_id = 0;

    // 加上前面所有组合长度小于 current_count 的组合数量
    for (uint32_t k = 1; k < current_count; ++k) 
    {
        new_id += combination(COUNT_TOTAL_ENTRIES, k);
    }

    // 对于当前长度的组合，找出它是第几个字典序
    uint32_t prev = 0;
    for (uint32_t i = 0; i < current_count; ++i) 
    {
        uint32_t current = new_events[i];

        for (uint32_t v = prev + 1; v < current; ++v) 
        {
            new_id += combination(COUNT_TOTAL_ENTRIES - v, current_count - i - 1);
        }

        // 如果找到目标事件
        if (current == event_num && i == current_count - 1) 
        {
            break;
        }

        prev = current;
    }

    // 增加1以表示这是该位置的新组合
    return new_id + 1;
}

// 将ID转换为事件组合数组
void id_to_event_combination(uint32_t id, uint32_t* output, uint32_t* count)
{
    if (id == 0 || id > ((1 << COUNT_TOTAL_ENTRIES) - 1) || output == NULL || count == NULL) {
        *count = 0;
        return;
    }

    *count = 0;
    uint8_t k = 1;
    
    // 找出组合长度
    while (combination(COUNT_TOTAL_ENTRIES, k) < id) {
        id -= combination(COUNT_TOTAL_ENTRIES, k++);
    }

    uint8_t last = 0;
    for (uint8_t i = 1; i <= k; ++i) {
        uint8_t v = last + 1;
        while (combination(COUNT_TOTAL_ENTRIES - v, k - i) < id) {
            id -= combination(COUNT_TOTAL_ENTRIES - v++, k - i);
        }
        output[(*count)++] = v;
        last = v;
    }
}
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
    CLR_EVENT_FLAG_ALL(MSG_FLAG);
    event_list_register();
}

void event_exec(uint32_t event_flag,uint32_t msg_flag,void *argv)
{
    uint32_t msgs[COUNT_TOTAL_ENTRIES];
    uint32_t msg_num = 0;
    memset(events,0,sizeof(events));
    memset(msgs,0,sizeof(msgs));
    event_num = 0;
    msg_num = 0;
    id_to_event_combination(event_flag, events, &event_num);
    id_to_event_combination(msg_flag, msgs, &msg_num);   
    bool ismsg = false;
    //外部员工执行
    for (size_t i = 0; i < event_num; i++)
    {
        // 遍历查找匹配的 event_bit
        for (size_t j = 0; j < getRegisterTableNum(); j++)
        {
            if (events[i] ==  eventBitMapping[j].event_bit)
            {
                ismsg = false;
                for (size_t k = 0; k < msg_num; k++)
                {
                    if (msgs[k] ==  eventBitMapping[j].event_bit)
                    {
                        // 有消息传递
                        ismsg = true;
                        break;
                    }
                }
                if(ismsg)
                {
                    // 找到匹配的 event_bit，执行对应的函数
                    EXECUTE_FUNC_BY_NAME_AT_LEVEL(arnics_event_item, eventBitMapping[j].name,EVENT_EXTERNAL_EMPLOY,argv);
                }
                else
                {
                    // 找到匹配的 event_bit，执行对应的函数
                    EXECUTE_FUNC_BY_NAME_AT_LEVEL(arnics_event_item, eventBitMapping[j].name,EVENT_EXTERNAL_EMPLOY,NULL);
                }
            }
        }
    }
    //内部员工执行
    for (size_t i = 0; i < event_num; i++)
    {
        // 遍历查找匹配的 event_bit
        for (size_t j = 0; j < getRegisterTableNum(); j++)
        {
            if (events[i] ==  eventBitMapping[j].event_bit)
            {
                ismsg = false;
                for (size_t k = 0; k < msg_num; k++)
                {
                    if (msgs[k] ==  eventBitMapping[j].event_bit)
                    {
                        // 有消息传递
                        ismsg = true;
                        break;
                    }
                }
                if(ismsg)
                {
                    // 找到匹配的 event_bit，执行对应的函数
                    EXECUTE_FUNC_BY_NAME_AT_LEVEL(arnics_event_item, eventBitMapping[j].name,EVENT_INTERNAL_EMPLOY,argv);
                }
                else
                {
                    // 找到匹配的 event_bit，执行对应的函数
                    EXECUTE_FUNC_BY_NAME_AT_LEVEL(arnics_event_item, eventBitMapping[j].name,EVENT_INTERNAL_EMPLOY,NULL);
                }
            }
        }
    }
            
}

void event_internal_exec()
{
    // 遍历查找匹配的 event_bit
    for (size_t i = 0; i < getRegisterTableNum(); i++)
    {
        EXECUTE_FUNC_BY_NAME_AT_LEVEL(arnics_event_item, eventBitMapping[i].name,EVENT_INTERNAL_EMPLOY,NULL);
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
bool add_event_flag(eventFlag_t *eventflag, const char *name,bool ismsg) 
{
    int index = getRegisterEntryIndex(name);
    if (index == -1) {
        return false; // 未找到对应的 name
    }

    // 设置 eventflag 中对应位置的位
    eventflag->event_flag = update_combination_id((uint32_t)index, eventflag->event_flag);
    if(ismsg)
    {
        eventflag->msg_flag = update_combination_id((uint32_t)index, eventflag->msg_flag);
    }
    return true;
}

// 用于设置事件标志 
bool set_event_flag(eventFlag_t *eventflag, const char *name,bool ismsg) 
{
    int index = getRegisterEntryIndex(name);
    if (index == -1) {
        return false; // 未找到对应的 name
    }
    CLR_EVENT_FLAG_ALL(eventflag->event_flag);
    CLR_EVENT_FLAG_ALL(eventflag->msg_flag);
    // 设置 eventflag 中对应位置的位
    eventflag->event_flag = update_combination_id((uint32_t)index, eventflag->event_flag);
    if(ismsg)
    {
        eventflag->msg_flag = update_combination_id((uint32_t)index, eventflag->msg_flag);
    }
    return true;
}
/*-------------------------------------------------------------------------------------*/

/*任务主函数-----------------------------------------------------------------------------*/
_WEAK void onWaittingOutMessage()
{
    while (1)
    {
        memset(&mesg_cache,0,sizeof(message_t));
        eventos_want_sleep = TRUE; // 现在提休眠申请
        ULOG_DEBUG("eventCenter:Waiting for Message...");
        ULOG_DEBUG("-----------------END------------------------");
        if (true == rtosEventosGetMsg(&mesg_cache,100))
        {
            eventos_want_sleep = FALSE; // 撤销休眠申请
            // 清除分析缓存，并存入外部消息
            CLR_EVENT_FLAG_ALL(EVENT_FLAG);
            CLR_EVENT_FLAG_ALL(MSG_FLAG);
            EVENT_FLAG = mesg_cache.eventflag;
            MSG_FLAG = mesg_cache.msgflag;
            // 处理接收到的消息
            ULOG_DEBUG("-----------------START----------------------");
            ULOG_DEBUG("ON_Waitting_OUT_Message::Got Message! ID=%d", mesg_cache.ID_Ts);
            event_state = ActionMsg; // 进入消息处理状态
            break;
        }
        else
        {
            event_internal_exec();
        }
        rtosThreadDelay(100);
    }
}

_WEAK void eventAction()
{
    while (1)
    {
        event_exec(EVENT_FLAG,MSG_FLAG,mesg_cache.buf);          // 此处将里面的每一项需求分析出来，并分发任务
        ULOG_DEBUG("eventCenter: analyzeSampleNeed Done!");
        rtosThreadDelay(100);
        event_state = SendingRspMsg; // 进入发送响应消息状态
        break;
    }
}

_WEAK void onResetState()
{
    bool needRsp = false;
    for(size_t j = 0; j < event_num; j++)
    {
        uint32_t single_event = events[j];
        for (size_t i = 0; i < getRegisterTableNum(); i++)
        {
            if (single_event ==  eventBitMapping[i].event_bit)
            {
                if (true == registerTable[i].needRsp)
                {
                    needRsp = true;
                    break;
                }
            }
        }
        if(needRsp)
        {
            break;
        }
    }
    CLR_EVENT_FLAG_ALL(EVENT_FLAG);
    CLR_EVENT_FLAG_ALL(MSG_FLAG);
    while (1)
    {
        if (needRsp)
        {
            if (rtosEventosSendMsg(&mesg_cache, 500)) // 阻塞500ms发送 直到尝试成功才会切换状态，保证消息能够发出
            {
                // 发送成功 一个状态结束
                ULOG_DEBUG("Message sent eventosRspQueue succeed! ID=%d", mesg_cache.ID_Ts);
                memset(&mesg_cache, 0, sizeof(message_t));

                event_state = OnWattingOutMsg; // 进入等待消息状态
                break;
            }
            else
            {
                ULOG_DEBUG("Retry sendding!!");
                event_internal_exec();
                rtosThreadDelay(100);
            }
            rtosThreadDelay(100);
        }
        else
        {
            event_state = OnWattingOutMsg; // 进入等待消息状态
            break;
        }
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
uint32_t SendEventCallToEventCenter(eventFlag_t eventflag, time_t wait)
{
    // 发送消息到事件队列
    // 创建消息
    message_t msg = {0};
    memset(&msg, 0, sizeof(msg));
    // 获取互斥信号量
    if (TakeEventosMutex(wait))
    {
        // 生成唯一ID
        msg.ID_Ts = ++global_id_counter;
        // 释放互斥信号量
        ReleaseEventosMutex();
    }
    msg.eventflag = eventflag.event_flag;
    msg.msgflag = eventflag.msg_flag;
    if(!IS_EVENT_FLAG_CLR(msg.msgflag))
    {
        memcpy(msg.buf, &eventflag.msg, sizeof(messageUnion_u));
        msg.length = sizeof(messageUnion_u);
    }

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
bool GetResponseMessageFromEventCenter(time_t ID, time_t wait,void *argv)
{
    message_t receivedMsg;
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
                    //消息拷贝
                    memcpy(argv,receivedMsg.buf,receivedMsg.length);
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

