/**
  ******************************************************************************
  * @file           : rollTs.c
  * @brief          : 时序数据库实现
  * 
  * RollDB是一个专为嵌入式系统设计的轻量级数据库，具有以下特点：
  * - 自动回滚机制：当存储空间不足时自动清理旧日志
  * - 高效的Flash存储管理：按扇区管理日志，优化写入和擦除操作
  * - 灵活的日志读取方式：支持批量读取和按范围读取
  * - 容量监控：提供剩余容量查询功能
  * 
  * 数据库存储结构分为两部分：
  * 1. 系统分区：存储数据库元信息(如日志数量、当前写入扇区等)
  * 2. 日志分区：存储实际的日志数据，每条日志包含日志头和日志体
  * 
  * 回滚机制通过循环使用日志扇区实现，当存储空间不足时会自动擦除最早
  * 的日志扇区以腾出空间给新日志。
  *
  * +-------------------+
  * |    System Sector  | -> 固定 1 block
  * +-------------------+
  * |     Log Sector    |
  * +-------------------+
  * 
  * @version        : 1.0.1
  * @date           : 2025-12-10
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
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>   
#include <string.h>
#include "rollDef.h"
/*---------------------------------------------------------------------------*/
/********************
 * 配置项 数据库 
 *******************/

// 数据库总大小 -字节数(需与最小擦除单元对齐)
#define ROLLTS_MAX_SIZE         (100  * MIN_ERASE_UNIT_SIZE) 
/*---------------------------------------------------------------------------*/
/*******************
 * 配置项 物理存储单元 
 *******************/

// 最小擦除粒度 作为单block大小-字节数
#define MIN_ERASE_UNIT_SIZE     (4 * 1024)
#define SINGLE_BLOCK_SIZE        (MIN_ERASE_UNIT_SIZE)

// 最小编程粒度 -字节数
#define MIN_WRITE_UNIT_SIZE     (1)
/*---------------------------------------------------------------------------*/
/*******************
 * 自动配置
 *******************/

// 数据库BLOCK数量
#define ROLLTS_MAX_BLOCK_NUM   (ROLLTS_MAX_SIZE/MIN_ERASE_UNIT_SIZE)
/* typedef-------------------------------------------------------------------*/
#define ROLLDB_VERSION         "1.0.1"

/**
 * 系统分区结构体
 */ 
typedef struct
{
    uint32_t                    magic_valid;

    uint32_t           data_start_block_num;              // 日志分区起始块编号
    uint32_t             data_end_block_num;              // 日志分区结束块编号
    uint32_t                       log_size;              // 日志分区大小
    // 数据库定义备份
    uint32_t                rollts_max_size;               
    uint32_t              single_block_size;
    uint32_t            min_write_unit_size;
    uint32_t           rollts_max_block_num;
    uint32_t                data_start_addr;
    uint32_t                  data_end_addr;
} rollts_sys_t;
#define SYSINFO_SIZE     sizeof(rollts_sys_t)
/**
 * 块区信息头
 * 
 * 位索引:   7   6    5    4    3    2    1    0
 * 内容:   is_head | reserved[5:0]
 */

#define SET_HEAD(status)          status.is_head      = 0  // 00
#define SET_BACKUP(status)        status.is_head      = 1  // 01
#define SET_NOT_HEAD(status)      status.is_head      = 3  // 11


#define IS_HEAD(status)         (status.is_head      == 0) // 00
#define IS_BACKUP(status)       (status.is_head      == 1) // 01
#define IS_NOT_HEAD(status)     (status.is_head      == 3) // 11

typedef struct 
{
    uint32_t                 last_data_addr;            // 0xFFFFFFFF:存储区未满 num:最后一个数据地址
    int32_t                        data_num;            // -1:未写满，不更新      num: 数据条数
    uint32_t                    magic_valid;

    union 
    {
        struct
        {
            uint8_t         block_status :6;             // default：111
            uint8_t              is_head :2;             // backup:01 head:00 not head:11
        };
        uint8_t                status;
    };

} block_info_t;
/**
 * 数据结构体
 */
typedef struct 
{
    uint32_t                    magic_valid;
    // 双向链表
    uint32_t                       pre_addr;            // 上一个日志地址
    uint32_t                       cur_addr;            // 当前日志地址
    uint32_t                      next_addr;            // 下一个日志地址
    uint32_t                    payload_len;            // payload
} rollts_data_t;


/* function-------------------------------------------------------------------*/
typedef struct 
{
    int                            (*erase_sector)(uint32_t address);
    int (*write_data)(uint32_t address, void *data, uint32_t length);
    int  (*read_data)(uint32_t address, void *data, uint32_t length);
#ifdef RTOS_MUTEX_ENABLE
    void                                         (*mutex_lock)(void);
    void                                       (*mutex_unlock)(void);
#endif
} flash_ops_t;

typedef struct 
{
    uint32_t                                  (*get_timestamp)(void);
} time_ops_t;

/**
 * 数据库管理单元
 */
typedef struct 
{
    uint32_t                              pre_addr;     // 用于数据库数据写入block地址
    uint32_t                             head_addr;     // 数据库起始地址
    uint32_t                      head_backup_addr;     // 数据库备份起始地址
} rollts_memtab_t;



typedef struct 
{ 
    uint32_t                        is_init;
    rollts_sys_t                   sys_info;           // 系统分区信息
    rollts_memtab_t                 mem_tab;           // 目录结构-工作区
    rollts_data_t               rollts_data;           // block 数据结构
    int32_t              cur_block_data_num;           // 当前块数据条数
    uint32_t           last_valid_data_addr;           // 最后一个有效数据地址
    bool                 current_block_full;

    flash_ops_t                  flash_ops;
} rollts_manager_t;

typedef struct
{
  uint32_t addr;
  uint32_t fack_end_addr;
  uint32_t pre_data_addr;
} find_the_last_position_t;

// 日志数据接收回调
typedef bool (*rollTscb)(uint8_t *buf,uint32_t len);

/**
 * @func: 数据库初始化
 */
extern int rollts_init(rollts_manager_t *rollts_manager);

/**
 * @func: 清除所有日志数据
 */
extern bool rollts_clear(rollts_manager_t *rollts_manager);

/**
 * @func: 数据库添加数据
 */
extern bool rollts_add(rollts_manager_t *rollts_manager, uint8_t *data, uint32_t payload_len);

/**
 * @brief 日志整体读取
 * 
 */
extern bool rollts_get_all(rollts_manager_t *rollts_manager, 
                           uint8_t *data, uint32_t max_payload_len,rollTscb cb);

/**
 * @brief 日志条数读取
 */
extern int32_t rollts_get_total_record_number(rollts_manager_t *rollts_manager);

/**
 * @brief 日志选择读取
 * 
 */
extern bool rollts_read_pick(rollts_manager_t *rollts_manager,
                             uint32_t start_num, uint32_t end_num,
                             uint8_t *data, uint32_t max_payload_len,rollTscb cb);

/**
 * @brief 日志剩余容量 百分比
 */
extern uint8_t rollts_capacity(rollts_manager_t *rollts_manager);

/**
 * @brief 日志总大小
 */
extern uint32_t rollts_capacity_size(rollts_manager_t *rollts_manager);

#ifdef __cplusplus
}
#endif


#endif // ROLLTS_H
