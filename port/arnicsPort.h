#ifndef _ARNICSPORT_H
#define _ARNICSPORT_H


#ifdef __cplusplus
extern "C" {
#endif

#include "Inc/projDefine.h"
#include "Inc/typedef.h"

// +-------------------------------------------------+
// | 平台             | STM32CHIP      | STM32系列 |
// +-------------------------------------------------+

// +-------------------------------------------------+
//  编程粒度
//  nor flash:   1 bit
//  stm32f2/f4:  8 bit
//  stm32f1:     32 bit
//  stm32l4:     64 bit
#ifdef MCU_STM32F1
#define STM32CHIP
#include "stm32f1xx_hal.h"
#define MCU_GRAN 32
#define _TOTAL_FLASH_SIZE_ 256 // 芯片容量 （KB）
#define MCU_START_ADDR 0x08000000


#elif defined(MCU_STM32L4)
#define STM32CHIP
#include "stm32l4xx_hal.h"
#define MCU_GRAN 64
#define _TOTAL_FLASH_SIZE_ 256 // 芯片容量 （KB）
#define MCU_START_ADDR 0x08000000

#elif defined(MCU_STM32F4)
#define STM32CHIP
#include "stm32f4xx_hal.h"
#define MCU_GRAN 8
#define MCU_START_ADDR 0x08000000
#define _TOTAL_FLASH_SIZE_ 512 // 芯片容量 （KB）

#endif


//#define _DEVELOPE_BOARD_
// +-------------------------------------------------+
// | Flash 地址配置    | BOOTLOAD_ADDR       | BootLoader 起始地址 |
// |                  | USER_APP_ADDR        | 用户应用程序起始地址 |
// |                  | USER_APP_INFO        | 信息区域起始地址     |
// |                  | USER_APP_CFG         | 运行参数区域起始地址 |
// |                  | USER_APP_STAT        | 运行状态区域起始地址 |
// |                  | USER_APP_MAXSIZE     | 用户应用程序最大尺寸 |
// +-------------------------------------------------#
#if (_TOTAL_FLASH_SIZE_ == 256)  // 针对 256KB 系列 MCU
#define BOOTLOAD_ADDR       0x08000000  // BootLoader 起始地址
#define USER_APP_ADDR       0x0800A000  // 用户应用程序起始地址
#define USER_APP_INFO       0x0803D000  // 信息区域起始地址
#define USER_APP_CFG        0x0803E000  // 运行参数区域起始地址
#define USER_APP_STAT       0x0803F800  // 运行状态区域起始地址
#define USER_APP_MAXSIZE    208896      // 用户应用程序最大尺寸
#endif



// 定义片内 Flash 地址的宏
#define CODEFLASH_SIZE_BOOTLOAD             (40*1024)    // 40KB
#define CODEFLASH_SIZE_APP                  (204*1024)   // 204KB
#define PARTITION_SIZE_TEST                 (4*1024)     // 4KB
#define PARTITION_SIZE_RFCFG                (6*1024)     // 6KB
#define PARTITION_SIZE_RTSTAT               (2*1024)     // 2KB



#define CODEFLASH_IMAGE_ADDR_BOOTLOAD_OFFSET               0x00
// APP固件地址
#define CODEFLASH_IMAGE_ADDR_APP_OFFSET                    (CODEFLASH_SIZE_BOOTLOAD)
// 测试分区
#define CODEFLASH_IMAGE_ADDR_TEST_OFFSET                   (CODEFLASH_IMAGE_ADDR_APP_OFFSET  + CODEFLASH_SIZE_APP)
// RF配置分区
#define CODEFLASH_IMAGE_ADDR_RFCFG_OFFSET                  (CODEFLASH_IMAGE_ADDR_TEST_OFFSET  + PARTITION_SIZE_TEST)
// 实时状态分区
#define CODEFLASH_IMAGE_ADDR_RTSTAT                        (CODEFLASH_IMAGE_ADDR_RFCFG_OFFSET  + PARTITION_SIZE_RFCFG)


// 定义片外 Flash 地址的宏
#define DATAFLASH_SIZE_IDENTIFY                    (4*1024)   // 4 KB
#define DATAFLASH_SIZE_DB                          (40*1024)  // 40KB
#define DATAFLASH_SIZE_LOG                         (400*1024) // 400KB

//定义片外 Flash 地址名称的宏
#define PARTITION_NAME_ONCHIP_BOOTLOADER      "bootloader"
#define PARTITION_NAME_ONCHIP_APP             "app"
#define PARTITION_NAME_ONCHIP_TEST            "test"
#define PARTITION_NAME_ONCHIP_RFCFG           "rfcfg"
#define PARTITION_NAME_ONCHIP_RTSTAT          "rtstat"

#define PARTITION_NAME_FW_IDENTIFIER_BOOT_A   "fw_identifer_boot_a"
#define PARTITION_NAME_FW_BOOT_A              "fw_boot_a"
#define PARTITION_NAME_FW_IDENTIFER_BOOT_B    "fw_identifer_boot_b"
#define PARTITION_NAME_FW_BOOT_B              "fw_boot_b"

#define PARTITION_NAME_FW_IDENTIFER_APP_A     "fw_identifer_app_a"
#define PARTITION_NAME_FW_APP_A               "fw_app_a"
#define PARTITION_NAME_FW_IDENTIFER_APP_B     "fw_identifer_app_b"
#define PARTITION_NAME_FW_APP_B               "fw_app_b"

#define PARTITION_NAME_ARNICS_SYS_PARA           "arnicsPara"
#define PARTITION_NAME_SYSTEM_CFG_PARA           "systemCfg"
#define PARTITION_NAME_SYSTEM_STATUS_PARA        "systemStatus"

#define DATA_FALSH_SIZE_ARNICS_SYS_PARA           (4*1024)
#define DATA_FALSH_SIZE_SYSTEM_CFG_PARA           (8*1024)
#define DATA_FALSH_SIZE_SYSTEM_STATUS_PARA        (8*1024)

#define DATAFLASH_DATAPLAT_ADDR   (0x000000)
#define DATAFLASH_ARNICS_SYS_PARA_START_ADDR DATAFLASH_DATAPLAT_ADDR   
#define DATAFLASH_ARNICS_SYS_PARA_END_ADDR (DATAFLASH_ARNICS_SYS_PARA_START_ADDR + DATA_FALSH_SIZE_ARNICS_SYS_PARA) 

#define DATAFLASH_SYS_CFG_PARA_START_ADDR DATAFLASH_ARNICS_SYS_PARA_END_ADDR   
#define DATAFLASH_SYS_CFG_PARA_END_ADDR (DATAFLASH_SYS_CFG_PARA_START_ADDR + DATA_FALSH_SIZE_SYSTEM_CFG_PARA) 

#define DATAFLASH_SYS_STATUS_PARA_START_ADDR DATAFLASH_SYS_CFG_PARA_END_ADDR   
#define DATAFLASH_SYS_STATUS_PARA_END_ADDR (DATAFLASH_SYS_STATUS_PARA_START_ADDR + DATA_FALSH_SIZE_SYSTEM_STATUS_PARA) 

// BootLoader固件地址A, 起始地址(含IDENTIFY): , 数据部分起始地址:
#define DATAFLASH_IMAGE_ADDR_BOOTLOAD_A_IDENTIFY    0x19000 //100 KB
#define DATAFLASH_IMAGE_ADDR_BOOTLOAD_A             (DATAFLASH_IMAGE_ADDR_BOOTLOAD_A_IDENTIFY + DATAFLASH_SIZE_IDENTIFY)

// BootLoader固件地址B, 起始地址(含IDENTIFY): , 数据部分起始地址:
#define DATAFLASH_IMAGE_ADDR_BOOTLOAD_B_IDENTIFY    (DATAFLASH_IMAGE_ADDR_BOOTLOAD_A + CODEFLASH_SIZE_BOOTLOAD)
#define DATAFLASH_IMAGE_ADDR_BOOTLOAD_B             (DATAFLASH_IMAGE_ADDR_BOOTLOAD_B_IDENTIFY + DATAFLASH_SIZE_IDENTIFY)

// APP固件地址A, 起始地址(含IDENTIFY): , 数据部分起始地址
#define DATAFLASH_IMAGE_ADDR_APP_A_IDENTIFY         (DATAFLASH_IMAGE_ADDR_BOOTLOAD_B + CODEFLASH_SIZE_BOOTLOAD)
#define DATAFLASH_IMAGE_ADDR_APP_A                  (DATAFLASH_IMAGE_ADDR_APP_A_IDENTIFY + DATAFLASH_SIZE_IDENTIFY)

// APP固件地址B, 起始地址(含IDENTIFY): , 数据部分起始地址
#define DATAFLASH_IMAGE_ADDR_APP_B_IDENTIFY         (DATAFLASH_IMAGE_ADDR_APP_A + CODEFLASH_SIZE_APP)
#define DATAFLASH_IMAGE_ADDR_APP_B                  (DATAFLASH_IMAGE_ADDR_APP_B_IDENTIFY + DATAFLASH_SIZE_IDENTIFY)
#define DATAFLASH_IMAGE_ADDR_APP_B_END              (DATAFLASH_IMAGE_ADDR_APP_B + CODEFLASH_SIZE_APP)
// 日志存储地址, 起始地址(含IDENTIFY, 此处假设无单独IDENTIFY部分): , 大小: 50 KB
#define DATAFLASH_DB_ADDR                           0x07E000
#define DATAFLASH_LOG_ADDR                          0x088000
#define DATAFLASH_LOG_END_ADDR                      (DATAFLASH_LOG_ADDR + DATAFLASH_SIZE_LOG)
// w25Q64最大数据闪存地址 8MB, 终结地址: 0x7FFFFF
#define DATAFLASH_MAX_ADDR                          0x7FFFFF
//#endif





#ifdef __cplusplus
}
#endif
#endif
