#include "business_process.h"

_SECTION( "._entry_business_api")
wisdomStackPart_t business_state = WORKSTAT_INIT;


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
SEPARATE_INIT_KERNEL(BusinessList, wisdomStackPart_t, &(business_state), SEPARATE_DEFAULT_NOT_FOUND_CALLBACK);

void business_process(void)
{
    // 运行内核 
    SEPARATE_RUN_KERNEL(1);
}



void business_init()
{
    uint32_t eventflag = 0;
    set_event_flag(&eventflag,"led_action");
    while(1)
    {
        MessageUnion test = {0};
        test.message_deliver.test_req.a = 1;
        test.message_deliver.test_req.b = 2;
        uint32_t id =SendEventCallToEventCenter(eventflag,&test,sizeof(MessageUnion),BLOCK_DELAY);
        GetResponseMessageFromEventCenter(id,BLOCK_DELAY,&test);
        //ULOG_INFO("%d,%d",test.message_deliver.test_rsp.a,test.message_deliver.test_rsp.b);
        
        rtosThreadDelay(1000);
    }

}
