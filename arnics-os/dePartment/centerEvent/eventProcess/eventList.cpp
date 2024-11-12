#include "eventList.h"
#include "eventCore.h"
#include "../../../drivers/driversInclude.h"
#include "../../../thirdParty/thirdPartyInclude.h"
_SECTION("._entry_event_api")

extern Message_t mesg_cache;//事件应用消息


// 定义函数
void led_action(void* argv) 
{
    static int a = 0;
    a++;
    // 函数实现
    ULOG_INFO("led_action\r\n");
    dev_ctl(&led0_ds,IO_TOGGLE,NULL);
    dev_ctl(&led1_ds,IO_TOGGLE,NULL);
    if(a == 4)
    {
        GPIO_PinState new_pin_state = GPIO_PIN_SET;
        uint32_t new_pin_mode = GPIO_MODE_OUTPUT_OD;

        // 使用宏
        DEV_PARAMS_SET(&led1_ds,
            {"PinState", &new_pin_state},
            {"GPIO_Mode", &new_pin_mode}
        );

        dev_open(&led1_ds);
    }
    MessageUnion messageReq = {0};
    memcpy(&messageReq,argv,sizeof(MessageUnion));
    MessageUnion* rsp = (MessageUnion*)argv;

    rsp->message_deliver.test_rsp.a = messageReq.message_deliver.test_req.a;
    rsp->message_deliver.test_rsp.b = messageReq.message_deliver.test_req.b;
}

void battery_check(void* argv) 
{
    // 函数实现
}






