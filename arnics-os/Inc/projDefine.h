#ifndef _PROJ_DEFINE_H_
#define _PROJ_DEFINE_H_

// +-------------------------------------------------+
// |                   平台定义                       |
// +-------------------------------------------------+
// | 跨平台环境识别宏 (请根据实际编译环境开启其一) |
// +-------------------------------------------------+

// #define PLATFORM_LINUX   1     // 启用 Linux/OS 环境 
#define PLATFORM_WIN     1     // 启用 Windows/OS 环境 
// #define PLATFORM_MCU     1     // 启用 MCU 环境 

// +-------------------------------------------------+
// |            平台宏内部校验                         |
// +-------------------------------------------------+
#ifndef PLATFORM_LINUX
#define PLATFORM_LINUX   0
#endif
#ifndef PLATFORM_WIN
#define PLATFORM_WIN     0
#endif
#ifndef PLATFORM_MCU
#define PLATFORM_MCU     0
#endif

#if (PLATFORM_LINUX + PLATFORM_WIN + PLATFORM_MCU) > 1
#error "PLATFORM_LINUX / PLATFORM_WIN / PLATFORM_MCU must be mutually exclusive (enable only one)."
#endif

/*强制开发者明确选择平台 */
#if (PLATFORM_LINUX + PLATFORM_WIN + PLATFORM_MCU) == 0
#error "No platform selected. Please define PLATFORM_LINUX, PLATFORM_WIN, or PLATFORM_MCU in projDefine.h"
#endif
// +-------------------------------------------------+
// |                   宏定义列表                     |
// +-------------------------------------------------+
// | 功能分类         | 宏定义              | 描述     |
// +-------------------------------------------------+
#define INIT_TAG       "init"
#define EVENT_TAG      "evet"
#define MEDIA_TAG      "mdia"
#define BUSINESS_TAG   "busi"
#define AD_TAG         "admi"
#define GUARD_TAG      "guad"
#define CONSOLE_TAG    "csle"
#define DATAPLAT_TAG   "dplt"
#define DEVICES_TAG    "devc"
#define DRIVERS_TAG    "driv"
// +-------------------------------------------------+
// | 框架             | _USE_FREERTOS_ / _USE_LINUX_ | 使用操作系统 |
// +-------------------------------------------------+
#if (PLATFORM_LINUX)
#define _USE_LINUX_
#elif (PLATFORM_WIN)
#define _USE_WIN_
#else
#define _USE_FREERTOS_
//#define _USE_FREERTOS_MONITOR_        // 监测 FreeRTOS CPU利用率情况
#define _USE_FREERTOS_SYSTEM_MONITOR_ // 监测 FreeRTOS 系统溢出及处理
#endif
#define NOT_ALLOWED_SLEEP

// +-------------------------------------------------+
// | 外部数据存储     | _EXT_DATAFLASH_ | 启用外部 DataFlash 存储 |
// +-------------------------------------------------+
#define _EXT_DATAFLASH_


// +-------------------------------------------------+
// | 组件             | _USE_USERASSERT_    | 启用断言机制 |
// |                  | _USE_RING_          | 启用环形缓冲区 |
// +-------------------------------------------------+
#define _USE_USERASSERT_
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
