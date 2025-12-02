#include "flash.h"
#include <string.h> // For memcpy
#include <stdio.h>

#define START_ADDRESS 0x08000000

// STM32F407VGT6 Flash sectors address
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Sector 11, 128 Kbytes */

static int ef_err_port_cnt = 0;
int on_ic_read_cnt  = 0;
int on_ic_write_cnt = 0;

/**
 * @brief 获取地址对应的扇区号
 * 
 * @param address Flash地址
 * @return uint32_t 扇区号
 */
static uint32_t get_sector(uint32_t address)
{
    if (address < ADDR_FLASH_SECTOR_1) {
        return FLASH_SECTOR_0;
    } else if (address < ADDR_FLASH_SECTOR_2) {
        return FLASH_SECTOR_1;
    } else if (address < ADDR_FLASH_SECTOR_3) {
        return FLASH_SECTOR_2;
    } else if (address < ADDR_FLASH_SECTOR_4) {
        return FLASH_SECTOR_3;
    } else if (address < ADDR_FLASH_SECTOR_5) {
        return FLASH_SECTOR_4;
    } else if (address < ADDR_FLASH_SECTOR_6) {
        return FLASH_SECTOR_5;
    } else if (address < ADDR_FLASH_SECTOR_7) {
        return FLASH_SECTOR_6;
    } else {
        return FLASH_SECTOR_8;
    }
}

/**
 * @brief 从闪存中读取数据
 * 
 * @param dev flash设备结构体指针
 * @param buf 读取数据的目标缓冲区
 * @param size 要读取的数据大小
 * @return int 读取的字节数
 */
int bsp_flash_read(flash_t *dev, uint8_t *buf, size_t size)
{
    size_t i;
    uint32_t addr = START_ADDRESS + dev->offset;
    if( addr%4 != 0)
    {
        ef_err_port_cnt++;
    }
    for (i = 0; i < size; i++, addr++, buf++)
    {
        *buf = *(uint8_t *) addr;
    }

    return size;
}


/**
 * @brief 将数据写入闪存（分段写入），包括擦除操作
 *
 * 此函数将数据分页写入闪存，并在写入之前执行必要的擦除操作
 *
 * @param dev flash设备结构体指针
 * @param buf 要写入的数据缓冲区
 * @param size 要写入的数据大小
 * @return int 写入的字节数，负数表示失败
 */
int bsp_flash_write(flash_t *dev, const uint8_t *buf, size_t size)
{
    uint32_t address = START_ADDRESS + dev->offset;


    // 解锁Flash
    HAL_FLASH_Unlock();
    
    // 写入数据 (按字对齐)
    for (size_t i = 0; i < size; i += sizeof(uint32_t)) 
    {
        uint32_t data;
        if (i + sizeof(uint32_t) <= size) 
        {
            // 完整的4字节
            data = *((uint32_t*)(buf + i));
        } 
        else 
        {
            // 处理不足4字节的情况
            data = 0xFFFFFFFF;
            memcpy(&data, buf + i, size - i);
        }
        
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address + i, data) != HAL_OK) {
            HAL_FLASH_Lock();
            return -1;
        }
    }
    
    // 上锁Flash
    HAL_FLASH_Lock();
    
    return (int)size;
}

/**
 * @brief 擦除指定地址的扇区
 * 
 * @param dev flash设备结构体指针
 * @param ctl flash控制结构体指针，包含offset和size信息
 * @return int 0表示成功，负数表示失败
 */
int flash_erase(flash_t *dev, flash_ctl_t *ctl)
{
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SectorError = 0;
    uint32_t start_address = START_ADDRESS + ctl->offset;
    uint32_t end_address = start_address + ctl->size - 1;
    
    // 检查地址对齐
    if (start_address % 4 != 0) {
        return -1;
    }
    
    // 解锁Flash
    HAL_FLASH_Unlock();
    
    // 擦除扇区
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = get_sector(start_address);
    EraseInitStruct.NbSectors = get_sector(end_address) - get_sector(start_address) + 1;
    
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
        HAL_FLASH_Lock();
        return -1;
    }
    
    // 上锁Flash
    HAL_FLASH_Lock();
    
    return 0;
}
