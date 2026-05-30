#include "eventCore.h"
#include "eventList.h"
#include "core/coreInclude.h"
#include "port/portInclude.h"
#include "thirdParty/thirdPartyInclude.h"
#include "rtosInterface/entry_rtos_api.h"


#if PLATFORM_MCU
_SECTION("._entry_event_api")
#endif
static EVENT_STATE event_state = OnWattingOutMsg;
static eventBits_t EVENT_FLAG  = {0}; // 外部事件标志
static eventBits_t MSG_FLAG    = {0}; // 外部消息标志
static message_t   mesg_cache  = {0}; // 事件应用消息


#undef X
#define X(func, employ_kind, needRsp) + 1
#define COUNT_TOTAL_ENTRIES (0 REGISTER_ENTRIES)
// 定义事件位映射表
static EventBitMapping eventBitMapping[COUNT_TOTAL_ENTRIES];
static uint32_t getRegisterTableNum()
{
    return (COUNT_TOTAL_ENTRIES);
}
#undef X
// 定义 X 宏
#define X(func, employ_kind, needRsp) \
    {#func, func, employ_kind, offsetof(messge_deliver_t, func##_msg), (sizeof(func##_msg_t)), needRsp},

// 用户事件
static RegisterEntry registerTable[] = 
{
    REGISTER_ENTRIES
};



static void execute_event_func(const char* name, int employ_kind, void* argv)
{
    for (size_t i = 0; i < getRegisterTableNum(); i++) 
    {
        if (registerTable[i].employ_kind == employ_kind && 
            strcmp(registerTable[i].name, name) == 0) 
        {
            if (registerTable[i].func != NULL) 
            {
                registerTable[i].func(argv);
            }
            break;
        }
    }
}

#undef X
#define X(func, employ_kind, needRsp) + 1



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
    if (event_num == 0 || event_num > COUNT_TOTAL_ENTRIES) 
    {
        return current_id; // 无效事件编号
    }

    // 如果是空组合，直接返回事件编号作为ID
    if (current_id == 0) 
    {
        return event_num;
    }

    if (COUNT_TOTAL_ENTRIES > 32U) 
    {
        return current_id;
    }
    const uint64_t max_id = (1ULL << COUNT_TOTAL_ENTRIES) - 1ULL;
    if ((uint64_t)current_id > max_id) 
    {
        return current_id;
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
    } 
    else 
    {
        // 否则使用逆向解码函数还原事件组合
        uint32_t k = 1;
        while (combination(COUNT_TOTAL_ENTRIES, k) < id) 
        {
            id -= combination(COUNT_TOTAL_ENTRIES, k++);
        }

        uint32_t last = 0;
        for (uint32_t i = 1; i <= k; ++i) 
        {
            uint32_t v = last + 1;
            while (combination(COUNT_TOTAL_ENTRIES - v, k - i) < id) 
            {
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
    if (output == NULL || count == NULL) 
    {
        return;
    }
    *count = 0;
    if (id == 0 || COUNT_TOTAL_ENTRIES > 32U) 
    {
        return;
    }
    const uint64_t max_id = (1ULL << COUNT_TOTAL_ENTRIES) - 1ULL;
    if ((uint64_t)id > max_id) 
    {
        return;
    }

    uint8_t k = 1;
    
    // 找出组合长度
    while (combination(COUNT_TOTAL_ENTRIES, k) < id) 
    {
        id -= combination(COUNT_TOTAL_ENTRIES, k++);
    }

    uint8_t last = 0;
    for (uint8_t i = 1; i <= k; ++i) 
    {
        uint8_t v = last + 1;
        while (combination(COUNT_TOTAL_ENTRIES - v, k - i) < id) 
        {
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
        eventBitMapping[i].employ_kind = registerTable[i].employ_kind;
        eventBitMapping[i].event_bit = i;
        eventBitMapping[i].msg_struct_offset = registerTable[i].msg_struct_offset;
        eventBitMapping[i].msg_struct_size = registerTable[i].msg_struct_size;
        eventBitMapping[i].needRsp = registerTable[i].needRsp;
    }
}

void event_init(void)
{
    CLR_EVENT_FLAG_ALL(EVENT_FLAG);
    CLR_EVENT_FLAG_ALL(MSG_FLAG);
    USER_ASSERT(COUNT_TOTAL_ENTRIES >= 1);
    USER_ASSERT(COUNT_TOTAL_ENTRIES <= EVENT_MAX_EMPLOYEES);
    event_list_register();
}

/*-------------------------------------------------------------------------------------*/

/*雇佣员工-----------------------------------------------------------------------------*/
typedef struct 
{
    uint8_t j;
    messge_deliver_t data;
}hird_employ_t;

typedef struct {
    const char* name;
    rtosPriority_e rtos_priority;
    size_t stack_size;
} hird_employ_task_t;
#undef Y
#define Y(name,rtos_priority,stack_size) \
    {#name, rtos_priority,stack_size},
hird_employ_task_t stack_ref[] =
{
    HIRD_EMPLOY_STATIC_REFERENCE
};
#undef Y

static volatile uint32_t hird_task_id = 0;
void hird_employ(void *argument)
{
    hird_employ_t *param = (hird_employ_t *)argument;
    uint8_t j = param->j;
    hird_task_id++;
    execute_event_func(eventBitMapping[j].name, EVENT_HIRED_EMPLOY, (uint8_t*)&param->data + eventBitMapping[j].msg_struct_offset);
    if(false == eventBitMapping[j].needRsp)
    {
        arnicsFree(param); // 不需要回复则直接释放内存
    }
    eventBitMapping[j].is_running = false;
    rtosTaskSelfDelete();
}
void hird_employ_no_argv(void *argument)
{
    hird_employ_t *param = (hird_employ_t *)argument;
    uint8_t j = param->j;
    if(false == eventBitMapping[j].needRsp)
    {
        arnicsFree(param); // 不需要回复则直接释放内存
    }
    hird_task_id++;
    execute_event_func(eventBitMapping[j].name, EVENT_HIRED_EMPLOY, NULL);
    eventBitMapping[j].is_running = false;
    rtosTaskSelfDelete();
}
void event_hird_employ_task(uint8_t j,void *argv)
{
    uint32_t current_hird_task_id = hird_task_id;
    eventBitMapping[j].something_wrong = false;
    if((EVENT_HIRED_EMPLOY == eventBitMapping[j].employ_kind))
    {
        if(true == eventBitMapping[j].is_running)
        {
            UFLOG_ERR("event:hird_employ task is running! %s",eventBitMapping[j].name);
            eventBitMapping[j].something_wrong = true;
            return;
        }
        hird_employ_t *hird_employ_data = (hird_employ_t *)arnicsMalloc(sizeof(hird_employ_t));
        eventBitMapping[j].task_argv = hird_employ_data;
        if (hird_employ_data == NULL) 
        {
            UFLOG_ERR("event:Memory allocation failed! %s",eventBitMapping[j].name);
            eventBitMapping[j].something_wrong = true;
            return;
        }
        memset(hird_employ_data, 0, sizeof(*hird_employ_data));
        hird_employ_data->j = j;
        size_t stack_size = 256;
        rtosPriority_e rtos_priority = rtosPriorityNormal;
        for(size_t i = 0; i < sizeof(stack_ref)/sizeof(stack_ref[0]); i++)
        {
            if(0 == strcmp(stack_ref[i].name, eventBitMapping[j].name))
            {
                // 用户指定堆空间
                stack_size = stack_ref[i].stack_size;
                rtos_priority = stack_ref[i].rtos_priority;
                break;  
            }
        }

        eventBitMapping[j].is_running = true;
        if(NULL != argv)
        {
            memcpy(&hird_employ_data->data,argv,sizeof(messge_deliver_t));
            rtosTaskCreate((char*)eventBitMapping[j].name, rtos_priority, (void* )hird_employ, stack_size, hird_employ_data);
        }
        else
        {
            rtosTaskCreate((char*)eventBitMapping[j].name, rtos_priority, (void* )hird_employ_no_argv, stack_size, hird_employ_data);
        }

        uint32_t i_count = 0;
        do
        {
            rtosThreadDelay(100);
            i_count++;
            if(i_count > 20)
            {
                UFLOG_ERR("%s task create out of time!",eventBitMapping[j].name);
                eventBitMapping[j].something_wrong = true;
                eventBitMapping[j].is_running      = false;
                return;
            }
        } while(current_hird_task_id == hird_task_id);
        //任务创建成功，由任务负责释放内存
    }
}
void hird_employ_message_handle(size_t mapping_id,messge_deliver_t *event_mesg_cache)
{
    // 等待雇佣工执行完成
    do
    {
        rtosThreadDelay(10);
    }while(true == eventBitMapping[mapping_id].is_running);

    memcpy((uint8_t*)event_mesg_cache + eventBitMapping[mapping_id].msg_struct_offset, \
    (uint8_t*)eventBitMapping[mapping_id].task_argv +offsetof(hird_employ_t,data) + eventBitMapping[mapping_id].msg_struct_offset,eventBitMapping[mapping_id].msg_struct_size);
    if(eventBitMapping[mapping_id].something_wrong)
    {
        event_mesg_cache->NULL_msg.illegal_msg = true;
    }
    else
    {
        event_mesg_cache->NULL_msg.illegal_msg = false;
    }
    arnicsFree(eventBitMapping[mapping_id].task_argv);
}
/*-------------------------------------------------------------------------------------*/
static bool eventBitsTestIndex(const eventBits_t* bits, uint32_t index)
{
    if (index >= EVENT_MAX_EMPLOYEES)
    {
        return false;
    }
    return (bits->words[index / EVENT_FLAG_WORD_BITS] & (1UL << (index % EVENT_FLAG_WORD_BITS))) != 0U;
}

static void eventBitsToIndices(const eventBits_t* bits, uint32_t* output, uint32_t* count)
{
    *count = 0;
    for (uint32_t i = 0; i < getRegisterTableNum(); i++)
    {
        if (eventBitsTestIndex(bits, i))
        {
            output[(*count)++] = i;
        }
    }
}

void event_exec(const eventBits_t* event_flag, const eventBits_t* msg_flag, messge_deliver_t* argv)
{
    memset(events, 0, sizeof(events));
    event_num = 0;
    eventBitsToIndices(event_flag, events, &event_num);

    for (size_t i = 0; i < event_num; i++)
    {
        const uint32_t mapping_id = events[i];
        if (eventBitsTestIndex(msg_flag, mapping_id))
        {
            event_hird_employ_task(mapping_id, argv);
        }
        else
        {
            event_hird_employ_task(mapping_id, NULL);
        }
    }

    for (size_t i = 0; i < event_num; i++)
    {
        const uint32_t mapping_id = events[i];
        if (eventBitsTestIndex(msg_flag, mapping_id))
        {
            execute_event_func(eventBitMapping[mapping_id].name, EVENT_EXTERNAL_EMPLOY, ((uint8_t*)argv + eventBitMapping[mapping_id].msg_struct_offset));
        }
        else
        {
            execute_event_func(eventBitMapping[mapping_id].name, EVENT_EXTERNAL_EMPLOY, NULL);
        }
    }

    for (size_t i = 0; i < event_num; i++)
    {
        const uint32_t mapping_id = events[i];
        if (eventBitsTestIndex(msg_flag, mapping_id))
        {
            execute_event_func(eventBitMapping[mapping_id].name, EVENT_INTERNAL_EMPLOY, ((uint8_t*)argv + eventBitMapping[mapping_id].msg_struct_offset));
        }
        else
        {
            execute_event_func(eventBitMapping[mapping_id].name, EVENT_INTERNAL_EMPLOY, NULL);
        }
    }
}

void event_internal_exec()
{
    // 遍历查找匹配的 event_bit
    for (size_t i = 0; i < getRegisterTableNum(); i++)
    {
        execute_event_func(eventBitMapping[i].name, EVENT_INTERNAL_EMPLOY, NULL);
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

static bool eventBitsSetIndex(eventBits_t* bits, uint32_t index)
{
    if (index >= EVENT_MAX_EMPLOYEES)
    {
        return false;
    }
    bits->words[index / EVENT_FLAG_WORD_BITS] |= (1UL << (index % EVENT_FLAG_WORD_BITS));
    return true;
}

bool add_event_flag(eventFlag_t *eventflag, const char *name,bool ismsg) 
{
    int index = getRegisterEntryIndex(name);
    if (index == -1) 
    {
        return false; // 未找到对应的 name
    }

    if (!eventBitsSetIndex(&eventflag->event_flag, (uint32_t)index))
    {
        return false;
    }
    if (ismsg)
    {
        if (!eventBitsSetIndex(&eventflag->msg_flag, (uint32_t)index))
        {
            return false;
        }
    }
    return true;
}

// 用于设置事件标志 
bool set_event_flag(eventFlag_t *eventflag, const char *name,bool ismsg) 
{
    int index = getRegisterEntryIndex(name);
    if (index == -1) 
    {
        return false; // 未找到对应的 name
    }
    CLR_EVENT_FLAG_ALL(eventflag->event_flag);
    CLR_EVENT_FLAG_ALL(eventflag->msg_flag);
    if (!eventBitsSetIndex(&eventflag->event_flag, (uint32_t)index))
    {
        return false;
    }
    if (ismsg)
    {
        if (!eventBitsSetIndex(&eventflag->msg_flag, (uint32_t)index))
        {
            return false;
        }
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
        // TODO:eventos_want_sleep = TRUE; // 现在提休眠申请
        UFLOG_DBG("eventCenter:Waiting for Message...");
        UFLOG_DBG("-----------------END------------------------");
        if (true == rtosEventosGetMsg(&mesg_cache,100))
        {
            // TODO:eventos_want_sleep = FALSE; // 撤销休眠申请
            // 清除分析缓存，并存入外部消息
            CLR_EVENT_FLAG_ALL(EVENT_FLAG);
            CLR_EVENT_FLAG_ALL(MSG_FLAG);
            EVENT_FLAG = mesg_cache.eventflag;
            MSG_FLAG = mesg_cache.msgflag;
            // 处理接收到的消息
            UFLOG_DBG("-----------------START----------------------");
            UFLOG_DBG("ON_Waitting_OUT_Message::Got Message! ID=%d", mesg_cache.ID_Ts);
            event_state = ActionMsg; // 进入消息处理状态
            break;
        }
        else
        {
            event_internal_exec();
        }
        rtosThreadDelay(10);
    }
}

_WEAK void eventAction()
{
    while (1)
    {
        event_exec(&EVENT_FLAG, &MSG_FLAG, &mesg_cache.message_union.message_deliver);          // 此处将里面的每一项需求分析出来，并分发任务
        UFLOG_DBG("eventCenter: analyzeSampleNeed Done!");
        rtosThreadDelay(10);
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
                if (true == eventBitMapping[i].needRsp)
                {
                    needRsp = true; //必定需要回复，直接置位
                    if(EVENT_HIRED_EMPLOY == eventBitMapping[i].employ_kind)
                    {
                        // 是雇佣工 等待雇佣工的消息
                        hird_employ_message_handle(i,&mesg_cache.message_union.message_deliver);
                    }


                }
            }
        }
    }
    CLR_EVENT_FLAG_ALL(EVENT_FLAG);
    CLR_EVENT_FLAG_ALL(MSG_FLAG);
    while (1)
    {
        if (needRsp)
        {
            bool send_ok = false;
            if (TakeEventosMsgQueueMutex(0))
            {
                send_ok = rtosEventosSendMsg(&mesg_cache, 0);
                ReleaseEventosMsgQueueMutex();
            }
            if (send_ok) // 发送成功才会切换状态，保证消息能够发出
            {
                // 发送成功 一个状态结束
                UFLOG_DBG("Message sent eventosRspQueue succeed! ID=%d", mesg_cache.ID_Ts);
                memset(&mesg_cache, 0, sizeof(message_t));

                event_state = OnWattingOutMsg; // 进入等待消息状态
                break;
            }
            else
            {
                UFLOG_DBG("Retry sendding!!");
                event_internal_exec();
                rtosThreadDelay(10);
            }
            rtosThreadDelay(10);
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
        memcpy(&msg.message_union, &eventflag.msg, sizeof(messageUnion_u));
    }

    // 等待时间为wait
    if (rtosDeliverMsgToEventos(&msg, wait) == true)
    {
        UFLOG_DBG("Message:needSample sent succeed! ID=%d",msg.ID_Ts);
        return msg.ID_Ts;
    }
    else
    {
        UFLOG_DBG("Message:needSample sent timeout failed!");
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
    if (argv == NULL) 
    {
        return FALSE;
    }

    const uint32_t start_tick = arnics_getTick();
    while (1)
    {
        uint32_t remaining_wait = 0;
        if ((uint32_t)wait == 0U)
        {
            remaining_wait = 0U;
        }
        else if ((uint32_t)wait == (uint32_t)BLOCK_DELAY)
        {
            remaining_wait = (uint32_t)BLOCK_DELAY;
        }
        else
        {
            const uint32_t elapsed = arnics_getTick() - start_tick;
            if (elapsed >= (uint32_t)wait)
            {
                return FALSE;
            }
            remaining_wait = (uint32_t)wait - elapsed;
        }

        if (!TakeEventosMsgQueueMutex(remaining_wait))
        {
            if ((uint32_t)wait == (uint32_t)BLOCK_DELAY)
            {
                continue;
            }
            return FALSE;
        }

        const uint32_t numMessages = CheckEventRspMesgNum();
        message_t receivedMsg;
        bool foundMessage = FALSE;

        for (uint32_t i = 0; i < numMessages; i++)
        {
            if (rtosTakeMsgFromEventos(&receivedMsg, 0) == true)
            {
                if (receivedMsg.ID_Ts == ID)
                {
                    memcpy(argv, &receivedMsg.message_union, sizeof(messageUnion_u));
                    foundMessage = TRUE;
                    break;
                }
                rtosEventosSendMsg(&receivedMsg, 0);
            }
            else
            {
                break;
            }
        }

        ReleaseEventosMsgQueueMutex();
        if (foundMessage)
        {
            return TRUE;
        }

        if ((uint32_t)wait == 0U)
        {
            return FALSE;
        }

        uint32_t delay_ms = 10U;
        if ((uint32_t)wait != (uint32_t)BLOCK_DELAY)
        {
            const uint32_t elapsed = arnics_getTick() - start_tick;
            const uint32_t remaining = (elapsed >= (uint32_t)wait) ? 0U : ((uint32_t)wait - elapsed);
            if (remaining == 0U)
            {
                return FALSE;
            }
            if (remaining < delay_ms)
            {
                delay_ms = remaining;
            }
        }
        rtosThreadDelay(delay_ms);
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

