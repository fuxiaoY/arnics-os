#ifndef _PROJ_DEFINE_H_
#define _PROJ_DEFINE_H_

// +-------------------------------------------------+
// |                   宏定义列表                     |
// +-------------------------------------------------+
// | 功能分类         | 宏定义              | 描述               |
// +-------------------------------------------------+
// | 串口调试         | _STDIO_REDEFINE_   | 使用串口1做调试输出并启用printf重定向到串口1 |
// +-------------------------------------------------+
//#define _STDIO_REDEFINE_

// +-------------------------------------------------+
// | 外部数据存储     | _EXT_DATAFLASH_W25Q64_ | 启用外部 DataFlash 存储 |
// +-------------------------------------------------+
#define _EXT_DATAFLASH_W25Q64_

// +-------------------------------------------------+
// | 框架             | _USE_FREERTOS_      | 使用 FreeRTOS 操作系统 |
// +-------------------------------------------------+
//#define _USE_FREERTOS_

// +-------------------------------------------------+
// | 组件             | _USE_USERASSERT_    | 启用断言机制 |
// |                  | ULOG_ENABLED        | 启用日志系统 |
// |                  | _USE_RING_          | 启用环形缓冲区 |
// +-------------------------------------------------+
#define _USE_USERASSERT_
#define ULOG_ENABLED
//#define _USE_RING_
// +-------------------------------------------------+
// | 业务功能           | _USE_CONSOLE_       | 启用控制台功能 |
// +-------------------------------------------------+
#define _USE_CONSOLE_

// +-------------------------------------------------+
// | 调试相关           | NOT_ALLOWED_SLEEP   | 禁止设备进入休眠模式 |
// |                  | _USE_UNITYTEST_     | 启用 Unity 测试框架 |
// +-------------------------------------------------+

#define _USE_UNITYTEST_

// +-------------------------------------------------+
// | 测试模块启用       | _USE_UNITYTEST_BATTERY_ | 启用电池测试模块 |
// |                  | _USE_UNITYTEST_POSTURE_ | 启用姿态测试模块 |
// |                  | _USE_UNITYTEST_SENSORIR_ | 启用红外传感器测试模块 |
// |                  | _USE_UNITYTEST_RFID_     | 启用 RFID 测试模块 |
// |                  | _DEVELOPE_BOARD_    | 开发板配置 |
// +-------------------------------------------------+
//#define _FUNCTION_TEST_COMPILE_

#ifdef _FUNCTION_TEST_COMPILE_
#define _USE_UNITYTEST_BATTERY_
#define _USE_UNITYTEST_POSTURE_
#define _USE_UNITYTEST_SENSORIR_
#define _USE_UNITYTEST_RFID_
#endif

//#define _DEVELOPE_BOARD_
// +-------------------------------------------------+
// | Flash 地址配置     | BOOTLOAD_ADDR       | BootLoader 起始地址 |
// |                  | USER_APP_ADDR_A     | 用户应用程序起始地址 |
// |                  | USER_APP_REIREC     | 测试区域起始地址 |
// |                  | USER_APP_RTCFG      | 运行参数区域起始地址 |
// |                  | USER_APP_RTSTAT     | 运行状态区域起始地址 |
// |                  | USER_APP_MAXSIZE    | 用户应用程序最大尺寸 |
// |                  | REI_REC_MAXSIZE     | 测试区域最大尺寸 |
// |                  | RTCFG_MAXSIZE       | 运行参数区域最大尺寸 |
// |                  | RTSTAT_MAXSIZE      | 运行状态区域最大尺寸 |
// +-------------------------------------------------#
#if (_TOTAL_FLASH_SIZE_ == 256)  // 针对 256KB 系列 MCU
#define BOOTLOAD_ADDR       0x08000000  // BootLoader 起始地址
#define USER_APP_ADDR_A     0x0800A000  // 用户应用程序起始地址
#define USER_APP_REIREC     0x0803D000  // 测试区域起始地址
#define USER_APP_RTCFG      0x0803E000  // 运行参数区域起始地址
#define USER_APP_RTSTAT     0x0803F800  // 运行状态区域起始地址
#define USER_APP_MAXSIZE    208896      // 用户应用程序最大尺寸
#define REI_REC_MAXSIZE     4096        // 测试区域最大尺寸
#define RTCFG_MAXSIZE       6144        // 运行参数区域最大尺寸
#define RTSTAT_MAXSIZE      2048        // 运行状态区域最大尺寸
#endif

#endif // _PROJ_DEFINE_H_
