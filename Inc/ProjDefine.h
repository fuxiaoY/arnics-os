#ifndef _PROJ_DEFINE_H_
#define _PROJ_DEFINE_H_



// +-------------------------------------------------+
// |                   宏定义列表                     |
// +-------------------------------------------------+
// | 功能分类         | 宏定义              | 描述     |
// +-------------------------------------------------+
#define EVENT_TAG "event"
#define MEDIA_TAG "media"
#define BUSINESS_TAG "business"
#define AD_TAG "administrative"
// +-------------------------------------------------+
// | 框架             | _USE_FREERTOS_      | 使用 FreeRTOS 操作系统 |
// +-------------------------------------------------+
#define _USE_FREERTOS_
//#define _USE_FREERTOS_MONITOR_        // 监测 FreeRTOS CPU利用率情况
//#define _USE_FREERTOS_SYSTEM_MONITOR_ // 监测 FreeRTOS 系统溢出及处理
// +-------------------------------------------------+
// | 平台             | STM32CHIP      | STM32系列 |
// +-------------------------------------------------+
#define STM32CHIP

// +-------------------------------------------------+
// | 外部数据存储     | _EXT_DATAFLASH_W25Q64_ | 启用外部 DataFlash 存储 |
// +-------------------------------------------------+
#define _EXT_DATAFLASH_W25Q64_


// +-------------------------------------------------+
// | 组件             | _USE_USERASSERT_    | 启用断言机制 |
// |                  | ULOG_ENABLED        | 启用日志系统 |
// |                  | _USE_RING_          | 启用环形缓冲区 |
// +-------------------------------------------------+
#define _USE_USERASSERT_
#define ULOG_ENABLED
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

// +-------------------------------------------------+
// | 测试模块启用      | _USE_UNITYTEST_BATTERY_ | 启用电池测试模块 |
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
// |                  | USER_APP_ADDR        | 用户应用程序起始地址 |
// |                  | USER_APP_REIREC      | 测试区域起始地址 |
// |                  | USER_APP_CFG         | 运行参数区域起始地址 |
// |                  | USER_APP_STAT        | 运行状态区域起始地址 |
// |                  | USER_APP_MAXSIZE    | 用户应用程序最大尺寸 |
// |                  | REI_REC_MAXSIZE     | 测试区域最大尺寸 |
// |                  | CFG_MAXSIZE       | 运行参数区域最大尺寸 |
// |                  | STAT_MAXSIZE      | 运行状态区域最大尺寸 |
// +-------------------------------------------------#
#if (_TOTAL_FLASH_SIZE_ == 256)  // 针对 256KB 系列 MCU
#define BOOTLOAD_ADDR       0x08000000  // BootLoader 起始地址
#define USER_APP_ADDR       0x0800A000  // 用户应用程序起始地址
#define USER_APP_REIREC     0x0803D000  // 测试区域起始地址
#define USER_APP_CFG        0x0803E000  // 运行参数区域起始地址
#define USER_APP_STAT       0x0803F800  // 运行状态区域起始地址
#define USER_APP_MAXSIZE    208896      // 用户应用程序最大尺寸
#define REI_REC_MAXSIZE     4096        // 测试区域最大尺寸
#define CFG_MAXSIZE         6144        // 运行参数区域最大尺寸
#define STAT_MAXSIZE        2048        // 运行状态区域最大尺寸
#endif


// 定义片外 Flash 地址的宏
#define DATAFLASH_SIZE_IDENTIFY             0x1000   // 4 KB
#define DATAFLASH_SIZE_BOOTLOAD             0xA000   // 40 KB
#define DATAFLASH_SIZE_APP                  0x33000  // 204 KB

#define DATAFLASH_SIZE_DB                   0xA000  // 40KB
#define DATAFLASH_SIZE_LOG                  0x64000  // 400KB


#define PARTITION_NAME_ONCHIP_BOOTLOADER   "bootloader"
#define PARTITION_NAME_ONCHIP_APP          "app"
#define PARTITION_NAME_ONCHIP_TEST         "test"
#define PARTITION_NAME_ONCHIP_RFCFG        "rfcfg"
#define PARTITION_NAME_ONCHIP_RTSTAT       "rtstat"

#define PARTITION_NAME_FW_IDENTIFIER_BOOT_A   "fw_identifer_boot_a"
#define PARTITION_NAME_FW_BOOT_A              "fw_boot_a"
#define PARTITION_NAME_FW_IDENTIFER_BOOT_B    "fw_identifer_boot_b"
#define PARTITION_NAME_FW_BOOT_B              "fw_boot_b"

#define PARTITION_NAME_FW_IDENTIFER_APP_A     "fw_identifer_app_a"
#define PARTITION_NAME_FW_APP_A               "fw_app_a"
#define PARTITION_NAME_FW_IDENTIFER_APP_B     "fw_identifer_app_b"
#define PARTITION_NAME_FW_APP_B               "fw_app_b"
/*
BootLoader A Identify Address: 0x000000
BootLoader A Address: 0x001000
BootLoader B Identify Address: 0x00B000
BootLoader B Address: 0x00C000
APP A Identify Address: 0x016000
APP A Address: 0x017000
APP B Identify Address: 0x04A000
APP B Address: 0x04B000
DB Address: 0x07E000
Log Address: 0x088000
Max DataFlash Address: 0x7FFFFF
*/

// BootLoader固件地址A, 起始地址(含IDENTIFY): , 数据部分起始地址: , 大小: 40 KB
#define DATAFLASH_IMAGE_ADDR_BOOTLOAD_A_IDENTIFY    0x00000
#define DATAFLASH_IMAGE_ADDR_BOOTLOAD_A             (DATAFLASH_IMAGE_ADDR_BOOTLOAD_A_IDENTIFY + DATAFLASH_SIZE_IDENTIFY)

// BootLoader固件地址B, 起始地址(含IDENTIFY): , 数据部分起始地址: , 大小: 40 KB
#define DATAFLASH_IMAGE_ADDR_BOOTLOAD_B_IDENTIFY    (DATAFLASH_IMAGE_ADDR_BOOTLOAD_A + DATAFLASH_SIZE_BOOTLOAD)
#define DATAFLASH_IMAGE_ADDR_BOOTLOAD_B             (DATAFLASH_IMAGE_ADDR_BOOTLOAD_B_IDENTIFY + DATAFLASH_SIZE_IDENTIFY)

// APP固件地址A, 起始地址(含IDENTIFY): , 数据部分起始地址: , 大小: 120 KB
#define DATAFLASH_IMAGE_ADDR_APP_A_IDENTIFY         (DATAFLASH_IMAGE_ADDR_BOOTLOAD_B + DATAFLASH_SIZE_BOOTLOAD)
#define DATAFLASH_IMAGE_ADDR_APP_A                  (DATAFLASH_IMAGE_ADDR_APP_A_IDENTIFY + DATAFLASH_SIZE_IDENTIFY)

// APP固件地址B, 起始地址(含IDENTIFY): , 数据部分起始地址: , 大小: 120 KB
#define DATAFLASH_IMAGE_ADDR_APP_B_IDENTIFY         (DATAFLASH_IMAGE_ADDR_APP_A + DATAFLASH_SIZE_APP)
#define DATAFLASH_IMAGE_ADDR_APP_B                  (DATAFLASH_IMAGE_ADDR_APP_B_IDENTIFY + DATAFLASH_SIZE_IDENTIFY)

// 日志存储地址, 起始地址(含IDENTIFY, 此处假设无单独IDENTIFY部分): , 大小: 50 KB
#define DATAFLASH_DB_ADDR                           0x07E000
#define DATAFLASH_LOG_ADDR                          0x088000
#define DATAFLASH_LOG_END_ADDR                      (DATAFLASH_LOG_ADDR + DATAFLASH_SIZE_LOG)
// w25Q64最大数据闪存地址, 起始地址: 0x7FFFFF
#define DATAFLASH_MAX_ADDR                          0x7FFFFF
//#endif


#endif // _PROJ_DEFINE_H_
