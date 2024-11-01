#include "business_process.h"
#include "../../core/coreInclude.h"
#include "../../thirdParty/thirdPartyInclude.h"
#include "../../rtosInterface/entry_rtos_api.h"
#include "../../dataPlat/entry_dataPlat_api.h"

#include "../centerEvent/entry_event_api.h"
#pragma arm section code = "._entry_business_api"
ENUM_WORKSTATE business_state = WORKSTAT_INIT;


/*-------------------------------------------------------------------------------------*/

/*状态主函数-----------------------------------------------------------------------------*/

// 定义状态列表 
const static SEPARATE_STATE BusinessList[] = {
    SEPARATE_DEFINE_STATE(WORKSTAT_INIT, business_init)
};

// 默认的回调函数 
static void SEPARATE_DEFAULT_NOT_FOUND_CALLBACK() {
    business_state = WORKSTAT_INIT;
}
// 初始化内核
SEPARATE_INIT_KERNEL(BusinessList, ENUM_WORKSTATE, &(business_state), SEPARATE_DEFAULT_NOT_FOUND_CALLBACK);

void business_process(void)
{
    // 运行内核 
    SEPARATE_RUN_KERNEL(1);
}



void business_init()
{
    uint32_t eventcall = 0;
    set_event_flag(&eventcall,"led_action");
    while(1)
    {
        SendEventCallToEventCenter(&eventcall,BLOCK_DELAY);
        rtosThreadDelay(1000);
    }

}
