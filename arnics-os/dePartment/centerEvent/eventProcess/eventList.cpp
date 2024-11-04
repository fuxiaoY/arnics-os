#include "eventList.h"
#include "eventCore.h"
#include "../../../drivers/driversInclude.h"
#pragma arm section code="._entry_event_api"

extern Message_t mesg_cache;//事件应用消息



// 定义函数
void led_action(void* argv) 
{
    static int a = 0;
    a++;
    // 函数实现
    printf("led_action\r\n");
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


}

void battery_check(void* argv) 
{
    // 函数实现
}






