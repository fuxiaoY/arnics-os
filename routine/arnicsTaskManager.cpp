#include "arnicsTaskManager.h"
#include "../drivers/driversInclude.h"

void init(void)
{
    dev_reg("led0",&led0_ds);
    dev_reg("led1",&led1_ds);
    dev_reg("uart1",&debug_ds);
    dev_open(&led0_ds);
    dev_open(&led1_ds);
    dev_open(&debug_ds);
}

/* 外设初始化任务清单 */
const tTaskFunc initTaskList[] =
{
    TASK_FUNC(init),
};

/*运行时配置参数、运行时状态记录预加载*/
const tTaskFunc preloadTaskList[] =
{


};

/*外挂设备初始化*/
const tTaskFunc deviceTaskList[] =
{


};
// 外设初始化函数
void peripheralInit(void)
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

// 配置参数、运行时状态记录预载函数
void preLoadInit(void)
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
void deviceInit(void)
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
