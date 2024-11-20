/*
 * Copyright (c) 2022, Spider, <spider@spider.vc>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include "inc/fal.h"
#include "fal_cfg.h"
#include "../../Inc/ProjDefine.h"
#include "../../drivers/driversInclude.h"
#include "../../port/portInclude.h"  //delay.h
static int init(void)
{
    dev_reg("mcu_flash",&mcuflash_ds);
    return 1;
}





static int read(long offset, uint8_t *buf, size_t size)
{

    DEV_PARAMS_SET(&mcuflash_ds,
        {"offset", &offset}
    );
    return dev_read(&mcuflash_ds,buf,size);
}


static int write(long offset, const uint8_t *buf, size_t size)
{
    DEV_PARAMS_SET(&mcuflash_ds,
        {"offset", &offset}
    );
    return dev_write(&mcuflash_ds,(void *)buf,size);
}

static int erase(long offset, size_t size)
{
    flash_ctl_t flash_erase;
    flash_erase.offset = offset;
    flash_erase.size = size;
    return dev_ctl(&mcuflash_ds,FLASH_EREASE,&flash_erase);
}


/*
  "flash_onchip" : Flash 设备的名字。
  0x08000000: 对 Flash 操作的起始地址。
  1024*1024：Flash 的总大小（1MB）。
  128*1024：Flash 块/扇区大小（因为 STM32F2 各块大小不均匀，所以擦除粒度为最大块的大小：128K）。
  {init, read, write, erase} ：Flash 的操作函数。 如果没有 init 初始化过程，第一个操作函数位置可以置空。
  8 : 设置写粒度，单位 bit， 0 表示未生效（默认值为 0 ），该成员是 fal 版本大于 0.4.0 的新增成员。各个 flash 写入粒度不尽相同，可通过该成员进行设置，以下列举几种常见 Flash 写粒度：
  nor flash:  1 bit
  stm32f2/f4: 8 bit
  stm32f1:    32 bit
  stm32l4:    64 bit
 */

//定义 内部flash 设备
const struct fal_flash_dev onchip_flash =
{
    .name       = MCU_FLASH_DEV_NAME,
    .addr       = MCU_START_ADDR,
    .len        = _TOTAL_FLASH_SIZE_*1024,
    .blk_size   = FLASH_PAGE_SIZE,
    .ops        = {init, read, write, erase},
    .write_gran = MCU_GRAN
};


