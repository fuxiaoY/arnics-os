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
        MessageUnion messageReq;
        memcpy(&messageReq,argv,sizeof(MessageUnion));
        ULOG_INFO("led0_toggle:req a =  %d",messageReq.message_deliver.led0_toggle_msg.led0_toggle_req.a);
    }
    else
    {
        dev_ctl(&led0_ds,IO_TOGGLE,NULL);
    }

}

void led1_action(void* argv) 
{
    if(NULL != argv)
    {
        static uint32_t total_blink_count = 0;
        MessageUnion* rsp = (MessageUnion*)argv;
        ULOG_INFO("led1_action:req blink_count =  %d",rsp->message_deliver.led1_action_msg.req_blink_count);
        for(uint8_t i = rsp->message_deliver.led1_action_msg.req_blink_count; i > 0; i--)
        {
            dev_ctl(&led1_ds,IO_TOGGLE,NULL);  
            total_blink_count++;
            rtosThreadDelay(500);
        } 
        rsp->message_deliver.led1_action_msg.led1_action_rsp.total_blink_count = rsp->message_deliver.led1_action_msg.req_blink_count;
    }
    else
    {
        ULOG_INFO("led1_action:no argv");
        dev_ctl(&led1_ds,IO_TOGGLE,NULL);  
    }
}




