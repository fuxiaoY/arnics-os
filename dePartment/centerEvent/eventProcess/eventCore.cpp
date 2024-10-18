#include "eventCore.h"
#include "eventList.h"
DEFINE_ARNICS_FUNC_ITEM_RANGE(arnics_event_item, EVENT_TAG, 0, 2);
// 全局计数器
static unsigned long global_id_counter = 0;
// 外部消息id
time_t ID_Ts;

EVENT_STATE event_state = OnWattingOutMsg;
EVENT_BYTE_T EVENT_FLAG = {0}; // 事件标志

void event_init(void)
{
    CLR_EVENT_FLAG_ALL(&EVENT_FLAG); // 所有报表无效化
}

void event_exec(const char *name)
{
    EXECUTE_FUNC_BY_NAME(arnics_event_item,name);
}
// 定义状态列表
const static SEPARATE_STATE EventList[] = {
    SEPARATE_DEFINE_STATE(OnWattingOutMsg, onWaittingOutMessage),
    SEPARATE_DEFINE_STATE(ActionMsg, eventAction),
    SEPARATE_DEFINE_STATE(SendingRspMsg, onSendingMessage)};

// 默认的回调函数
static void SEPARATE_DEFAULT_NOT_FOUND_CALLBACK()
{
    event_state = OnWattingOutMsg;
}

// 初始化内核
SEPARATE_INIT_KERNEL(EventList, EVENT_STATE, &(event_state), SEPARATE_DEFAULT_NOT_FOUND_CALLBACK);

/*-------------------------------------------------------------------------------------*/

/*状态主函数-----------------------------------------------------------------------------*/
void Event_Process()
{
    event_init();

    // 运行内核
    SEPARATE_RUN_KERNEL(1);
}

__weak void onWaittingOutMessage()
{
}

__weak void eventAction()
{
}

__weak void onSendingMessage()
{
}
