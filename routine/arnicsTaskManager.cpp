#include "arnicsTaskManager.h"


/* 外设初始化任务清单 */
const tTaskFunc initTaskList[] =
{


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
