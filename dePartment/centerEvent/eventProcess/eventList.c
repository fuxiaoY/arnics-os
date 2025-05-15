#include "eventList.h"
#include "eventCore.h"
#include "../../../drivers/driversInclude.h"
#include "../../../thirdParty/thirdPartyInclude.h"
#include "../../../rtosInterface/entry_rtos_api.h"
_SECTION("._entry_event_api")

extern Message_t mesg_cache;//事件应用消息


// 定义函数

void led0_toggle(void* argv) 
{

    if(NULL != argv)
    {
        
    }
    else
    {
        EVET_START
        // 切换led0_ds的IO状态
        dev_ctl(&led0_ds,IO_TOGGLE,NULL);
        EVET_DELAY(1000);
        EVET_END
    }

}

void led1_action(void* argv) 
{
    if(NULL != argv)
    {
        static uint32_t total_blink_count = 0;
        MessageUnion* rsp = (MessageUnion*)argv;
        for(uint8_t i = rsp->message_deliver.led1_action_msg.req_blink_count; i > 0; i--)
        {
            dev_ctl(&led1_ds,IO_TOGGLE,NULL);  
            total_blink_count++;
            rtosThreadDelay(500);
        } 
    }
    else
    {
        dev_ctl(&led1_ds,IO_TOGGLE,NULL);  
    }
}




