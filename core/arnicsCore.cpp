#include "arnicsCore.h"

ArnicsCoreData arnics_core_data;

static void nop_process(void) {}


const ArnicsInitItem arnics_init_item_start _SECTION("arnics.init.0") = 
{     
    "", nop_process
};
const ArnicsInitItem arnics_init_item_end   _SECTION("arnics.init.5") = 
{       
    "", nop_process
};

static void init_items(void)
{
    const pArnicsInitItem it = &arnics_init_item_start;
    while (it < &arnics_init_item_end) 
    {
        it++->init();
    }   
}

void arnics_systick_handler()
{
    arnics_core_data.arnics_systick++;
}
void arnics_addTick(uint32_t addTime)
{
  arnics_core_data.arnics_systick += addTime;
}


void arnics_core_init()
{
    init_items();
}

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


