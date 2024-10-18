/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-17     armink       the first version
 */

#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_
#include "ProjDefine.h"
#include "upgradeManagerDef.h"
#include "fal_def.h"
//定义使用SFUD
#define FAL_USING_SFUD_PORT
//定义TABLE分区
#define FAL_PART_HAS_TABLE_CFG
//定义外部flash名称
#define NOR_FLASH_DEV_NAME             "w25qxx"

/* ===================== Flash device Configuration ========================= */
extern const struct fal_flash_dev onchip_flash;
extern struct fal_flash_dev nor_flash0;

/* flash device table */
#define FAL_FLASH_DEV_TABLE                                          \
{                                                                    \
    &onchip_flash,                                           \
    &nor_flash0,                                                     \
}
/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG
/* partition table */
/*
 *               数据库名
 * magic_word   |name[FAL_DEV_NAME_MAX]   |flash_name[FAL_DEV_NAME_MAX]   |long offset  |len  |reserved
 */
#define FAL_PART_TABLE                                                                \
{                                                                                     \
    {FAL_PART_MAGIC_WORD,  PARTITION_NAME_ONCHIP_BOOTLOADER,   "flash_onchip",         0,   40*1024, 0},  \
    {FAL_PART_MAGIC_WORD,  PARTITION_NAME_ONCHIP_APP,          "flash_onchip",   40*1024,  204*1024, 0},  \
    {FAL_PART_MAGIC_WORD,  PARTITION_NAME_ONCHIP_TEST,         "flash_onchip",   244*1024,  4*1024, 0},   \
    {FAL_PART_MAGIC_WORD,  PARTITION_NAME_ONCHIP_RFCFG,        "flash_onchip",   248*1024,  6*1024, 0},  \
    {FAL_PART_MAGIC_WORD,  PARTITION_NAME_ONCHIP_RTSTAT,       "flash_onchip",   254*1024,  2*1024, 0}, \
    {FAL_PART_MAGIC_WORD, PARTITION_NAME_FW_IDENTIFIER_BOOT_A, NOR_FLASH_DEV_NAME,          DATAFLASH_IMAGE_ADDR_BOOTLOAD_A_IDENTIFY, DATAFLASH_SIZE_IDENTIFY, 0}, \
    {FAL_PART_MAGIC_WORD, PARTITION_NAME_FW_BOOT_A,            NOR_FLASH_DEV_NAME,          DATAFLASH_IMAGE_ADDR_BOOTLOAD_A, DATAFLASH_SIZE_BOOTLOAD, 0}, \
    {FAL_PART_MAGIC_WORD, PARTITION_NAME_FW_IDENTIFER_BOOT_B,  NOR_FLASH_DEV_NAME,          DATAFLASH_IMAGE_ADDR_BOOTLOAD_B_IDENTIFY, DATAFLASH_SIZE_IDENTIFY, 0}, \
    {FAL_PART_MAGIC_WORD, PARTITION_NAME_FW_BOOT_B,            NOR_FLASH_DEV_NAME,          DATAFLASH_IMAGE_ADDR_BOOTLOAD_B, DATAFLASH_SIZE_BOOTLOAD, 0}, \
    {FAL_PART_MAGIC_WORD, PARTITION_NAME_FW_IDENTIFER_APP_A,   NOR_FLASH_DEV_NAME,          DATAFLASH_IMAGE_ADDR_APP_A_IDENTIFY, DATAFLASH_SIZE_IDENTIFY, 0}, \
    {FAL_PART_MAGIC_WORD, PARTITION_NAME_FW_APP_A,             NOR_FLASH_DEV_NAME,          DATAFLASH_IMAGE_ADDR_APP_A, DATAFLASH_SIZE_APP, 0}, \
    {FAL_PART_MAGIC_WORD, PARTITION_NAME_FW_IDENTIFER_APP_B,   NOR_FLASH_DEV_NAME,          DATAFLASH_IMAGE_ADDR_APP_B_IDENTIFY, DATAFLASH_SIZE_IDENTIFY, 0}, \
    {FAL_PART_MAGIC_WORD, PARTITION_NAME_FW_APP_B,             NOR_FLASH_DEV_NAME,          DATAFLASH_IMAGE_ADDR_APP_B, DATAFLASH_SIZE_APP, 0}, \
    {FAL_PART_MAGIC_WORD,  "flashDB_KVDB", NOR_FLASH_DEV_NAME,                    DATAFLASH_DB_ADDR, DATAFLASH_SIZE_DB, 0}, \
    {FAL_PART_MAGIC_WORD,  "flashDB_TSDB", NOR_FLASH_DEV_NAME,                    DATAFLASH_LOG_ADDR, DATAFLASH_SIZE_LOG, 0}, \
}

#endif /* FAL_PART_HAS_TABLE_CFG */

#endif /* _FAL_CFG_H_ */
