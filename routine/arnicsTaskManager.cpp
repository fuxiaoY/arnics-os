#include "arnicsTaskManager.h"
#include "include.h"

/* 外设初始化任务清单 */
const tTaskFunc initTaskList[] =
{
    TASK_FUNC(driver_init),
};

/*预加载*/
const tTaskFunc preloadTaskList[] =
{
    TASK_FUNC(third_init),
    TASK_FUNC(global_all_read),
    TASK_FUNC(MQTopicConfig),
    
};

/*外挂设备初始化*/
const tTaskFunc deviceTaskList[] =
{


};
// 外设初始化函数
void peripheralInit(void* argv)
{
    uint8 i;
    for (i = 0; i < eleof(initTaskList); i++)
    {
        if (initTaskList[i].func != NULL)
        {
            initTaskList[i].func();
        }
    }
}
DRIVER_INIT("driver", peripheralInit);

// 预载函数
void preLoadInit(void* argv)
{
    uint8 i;

    for (i = 0; i < eleof(preloadTaskList); i++)
    {
        if (preloadTaskList[i].func != NULL)
        {
            preloadTaskList[i].func();
        }
    }
}
SYSTEM_INIT("system", preLoadInit);

// 外挂设备初始化函数
void deviceInit(void* argv)
{
    uint8 i;
    for (i = 0; i < eleof(deviceTaskList); i++)
    {
        if (deviceTaskList[i].func != NULL)
        {
            deviceTaskList[i].func();
        }
    }
}
COMPONENT_INIT("component", deviceInit);


void driver_init(void)
{
    dev_reg("rng",&rng_ds);
    dev_open(&rng_ds);
    dev_reg("led0",&led0_ds);
    dev_open(&led0_ds);
    
    dev_reg("led1",&led1_ds);
    dev_open(&led1_ds);

    dev_reg("uart1",&debug_ds);
    dev_open(&debug_ds);
    dev_reg("iwdg",&iwdg_ds);
    test_delay_check();

    dev_reg("rtc",&rtc_ds);
    dev_open(&rtc_ds);
    rtcTimeDateTypeDef_t rtc_time;
    dev_ctl(&rtc_ds,RTC_GETDATETIME,&rtc_time);


}
void third_init(void)
{
    thirdPartyInit();
}
