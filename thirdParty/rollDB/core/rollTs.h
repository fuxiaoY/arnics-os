/* information */
/**
  ******************************************************************************
  * @file           : rollTs.c
  * @brief          : 时序数据库实现
  * 
  * 该数据库拥有增，删（批量），查功能，并进行自动回滚。
  * 
  * 数据库原理说明:该数据库默认进行两个分区，系统分区用于管理数据库，日志分区用于存储数据
  * 系统分区 (System Info):
  * magic_valid:       用于验证系统分区的有效性。
  * sys_info_len:      系统分区数据的长度。
  * log_data_start:    日志分区中第一条日志的起始地址。
  * log_data_end:      日志分区中最后一条日志的结束地址。
  * log_num:           当前日志的数量。
  * log_action_num:    日志操作的序号。
  * current_sector:    当前写入的日志扇区索引。
  * log_sector_status: 每个日志扇区的状态（SECTOR_EMPTY, SECTOR_WRITTING, SECTOR_FULL）。
  * 日志分区 (Log Sector):
  * Log Header: 每条日志的头部信息，包含日志的有效标志、序号、长度和下一个日志的地址偏移。
  * Log Data: 日志的实际数据。
  * 
  * +-------------------+
  * |    System Sector  |
  * +-------------------+
  * |     Log Sector    |
  * +-------------------+
  * 
  * 
  * @version        : 1.0.0
  * @date           : 2025-04-10
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 ARSTUDIO.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#ifndef ROLLTS_H
#define ROLLTS_H

#ifdef __cplusplus
extern "C" {
#endif



/* Includes -----------------------------------------------------------------*/
#include "rollDef.h"

#include "../Inc/projDefine.h"      ///< 项目定义头文件
#include "../Inc/typedef.h"         ///< 类型定义头文件

/* user  --------------------------------------------------------------------*/
 // 单个日志最大长度
#define SINGLE_ROLLTS_MAX_SIZE  500 //字节
// 硬件扇区大小
#define SINGLE_SECTOR_SIZE      (4*1024) //4KB
// 日志数据库总大小
#define ROLLTS_MAX_SIZE         (6 * SINGLE_SECTOR_SIZE) // 总日志大小包括系统分区+日志分区
// 日志分区大小
#define LOG_SECTOR_SIZE         (4 * SINGLE_SECTOR_SIZE) // 日志分区大小-用于存储数据
// 系统分区大小
#define SYSINFO_SIZE            (ROLLTS_MAX_SIZE - LOG_SECTOR_SIZE)

/* typedef ------------------------------------------------------------------*/
#define ROLLDB_VERSION                    "1.0.0"      


#define SYSINFO_START_ADDR      (0)                                 // 系统分区起始地址
#define LOG_SECTOR_START_ADDR   (SYSINFO_START_ADDR + SYSINFO_SIZE) // 日志分区起始地址

#define SYSINFO_NUM             (SYSINFO_SIZE / SINGLE_SECTOR_SIZE) // 系统分区扇区数量
#define LOG_SECTOR_NUM         (LOG_SECTOR_SIZE / SINGLE_SECTOR_SIZE) // 日志分区扇区数量






// 最小擦除单位 扇区 = 4KB
// 最大编程粒度 页   = 256B
// 最小编程粒度 字   = 1B
// 最小读取粒度 字   = 1B

// 日志分区状态
typedef enum 
{
    SECTOR_EMPTY = 0X01,
    SECTOR_WRITTING,
    SECTOR_FULL
} sector_status_t;

typedef struct 
{
    bool (*erase_sector)(uint32_t address);
    bool (*write_data)(uint32_t address, void *data, uint32_t length);
    bool (*read_data)(uint32_t address, void *data, uint32_t length);
} flash_ops_t;

typedef struct 
{
    uint32_t (*get_timestamp)(void);
} time_ops_t;


// 系统分区数据句柄
typedef struct
 {
    uint32_t magic_valid;     // 系统分区有效标志
    uint32_t sys_info_len;    // 系统分区数据长度
    uint32_t sys_addr_current_sector;    // 当前系统分区扇区起始地址
    uint32_t sys_addr_offset;     // 下一个系统分区地址偏移
    uint32_t sys_size;        // 系统分区大小
    uint32_t log_size;        // 日志分区大小

    uint32_t log_data_start;  // 日志分区首条起始地址
    uint32_t log_data_end;    // 日志分区尾条结束地址
    int32_t log_num;          // 日志数量
    uint32_t log_action_num;  // 日志操作数量
    uint32_t log_current_sector;  // 当前写入的扇区 从0开始编号
    sector_status_t log_sector_status[LOG_SECTOR_SIZE/SINGLE_SECTOR_SIZE]; // 日志分区扇区状态
} rollts_sys_t;


// 单条日志数据句柄
typedef struct 
{
    uint32_t addr_offset;  // 下一个日志地址偏移
    uint32_t number;       // 日志序号
    uint32_t len;          // 当前日志总长度
    uint32_t magic_valid; // 日志有效标志
} rollts_log_t;


extern flash_ops_t rolldb_flash_ops;
extern time_ops_t rolldb_time_ops;
extern void task_suspend(void);
extern void task_resume(void);

// 日志数据接收回调
typedef bool (*rollTscb)(uint8_t *buf,uint32_t len);

/**
 * @brief 数据库初始化
 * @note 对系统分区数据进行初始化，并写入系统分区
 *  系统分区有效-跳过
 *  系统分区无效-初始化包括：日志分区扇区状态清除为 EMPTY
 *                         日志分区首尾地址初始化为0
 *                         日志数量初始化为0
 *                         日志分区全擦除
 */
extern bool rollts_init(rollts_sys_t *handle);

/**
 * @brief 数据库注销
 * @note 注销数据库，擦除系统分区
 */
extern bool rollts_deinit(rollts_sys_t *sys_handle);
/**
 * @brief 日志清除
 * @note 清除所有日志数据，包括系统分区数据（调用系统分区无效初始化） 
 *     
 */
extern bool rollts_clear(rollts_sys_t *sys_handle);

/**
 * @brief 日志追加
 * @note 追加一条日志数据 
 *     
 */
extern bool rollts_add(rollts_sys_t *sys_handle,rollts_log_t* log_handle,uint8_t *data,uint16_t length);


/**
 * @brief 日志整体读取
 * 
 */
extern bool rollts_read_all(rollts_sys_t *sys_handle,rollts_log_t* log_handle,rollTscb cb);

/**
 * @brief 日志选择读取
 * 
 */
extern bool rollts_read_pick(rollts_sys_t *sys_handle,rollts_log_t* log_handle,uint32_t start_num,uint32_t end_num,rollTscb cb);

/**
 * @brief 日志数量读取
 * 
 */
extern uint32_t rollts_num(rollts_sys_t *sys_handle);

/**
 * @brief 当前剩余容量 
 * @return 返回百分比
 */
uint32_t rollts_capacity(rollts_sys_t *sys_handle) ;

/**
 * @brief 当前剩余容量大小
 * @return 返回字节数
 */
uint32_t rollts_capacity_size(rollts_sys_t *sys_handle) ;

/**
 * @brief 日志修复
 * @return 成功返回true，失败返回false
 */
bool rollts_repair_logs(rollts_sys_t *sys_handle,rollts_log_t *log_handle);
#ifdef __cplusplus
}
#endif


#endif // ROLLTS_H

