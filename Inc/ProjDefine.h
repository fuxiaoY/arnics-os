#ifndef _PROJ_DEFINE_H_
#define _PROJ_DEFINE_H_



// +-------------------------------------------------+
// |                   宏定义列表                     |
// +-------------------------------------------------+
// | 功能分类         | 宏定义              | 描述     |
// +-------------------------------------------------+
#define EVENT_TAG      "event"
#define MEDIA_TAG      "media"
#define BUSINESS_TAG   "business"
#define AD_TAG         "administrative"
#define GUARD_TAG      "guard"
#define CONSOLE_TAG    "console"
#define DATAPLAT_TAG   "dataplat"
#define DEVICES_TAG    "devices"
#define DRIVERS_TAG    "drivers"
// +-------------------------------------------------+
// | 框架             | _USE_FREERTOS_      | 使用 FreeRTOS 操作系统 |
// +-------------------------------------------------+
#define _USE_FREERTOS_
#define NOT_ALLOWED_SLEEP
//#define _USE_FREERTOS_MONITOR_        // 监测 FreeRTOS CPU利用率情况
#define _USE_FREERTOS_SYSTEM_MONITOR_ // 监测 FreeRTOS 系统溢出及处理

// +-------------------------------------------------+
// | 外部数据存储     | _EXT_DATAFLASH_ | 启用外部 DataFlash 存储 |
// +-------------------------------------------------+
#define _EXT_DATAFLASH_


// +-------------------------------------------------+
// | 组件             | _USE_USERASSERT_    | 启用断言机制 |
// |                  | _ULOG_ENABLED_        | 启用日志 |
// |                  | _USE_RING_          | 启用环形缓冲区 |
// +-------------------------------------------------+
#define _USE_USERASSERT_
#define _ULOG_ENABLED_
#define _USE_RING_
// +-------------------------------------------------+
// | 业务功能           | _USE_CONSOLE_       | 启用控制台功能 |
// +-------------------------------------------------+
//#define _USE_CONSOLE_

// +-------------------------------------------------+
// | 调试相关       
// |                   | _USE_UNITYTEST_     | 启用 Unity 测试框架 |
// +-------------------------------------------------+
#define _USE_UNITYTEST_

#endif // _PROJ_DEFINE_H_
