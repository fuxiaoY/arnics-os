#include "flash.h"
#include <string.h> // For memcpy
#include <stdio.h>

#define START_ADDRESS 0x08000000

static int ef_err_port_cnt = 0;
int on_ic_read_cnt  = 0;
int on_ic_write_cnt = 0;

/**
 * @brief 从闪存中读取数据
 * 
 * @param address 读取闪存的起始地址。
 * @param buffer 读取数据的目标缓冲区。
 * @param size 要读取的数据大小。
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
 * @brief 将数据写入闪存（分段写入），包括擦除操作。
 *
 * 此函数将数据分页写入闪存，并在写入之前执行必要的擦除操作。
 * 注意，写入地址必须对齐到闪存页的边界，否则将触发错误处理。
 *
 * @param address 写入闪存的起始地址。必须对齐到页大小边界。
 * @param data 要写入的数据缓冲区。数据长度应为非零。
 * @param size 要写入的数据大小。必须是非零值，且不超过闪存剩余空间。
 *
 * @note 地址和数据大小的约束如下：
 *       - address 必须是 FLASH_PAGE_SIZE 的倍数。
 *       - size 必须大于0且小于等于剩余闪存空间。
 *       - 如果 size 不是 FLASH_PAGE_SIZE 的倍数，最后一页可能不会完全填满。
 */
int bsp_flash_write(flash_t *dev, const uint8_t *buf, size_t size)
{
    HAL_StatusTypeDef status;

    // 确保地址对齐到页边界
    if (dev->offset % FLASH_PAGE_SIZE != 0)
    {
        // 错误处理：地址未对齐到页边界
        printf("Error: Address not aligned to page boundary.\r\n");
        return -2;
    }

    // 解锁闪存
    HAL_FLASH_Unlock();


    // 分页写入数据
    unsigned long page_start = dev->offset; // 从给定的地址开始写入
    while (size > 0)
    {
        // 计算剩余数据是否大于一页
        size_t chunk_size = (size >= FLASH_PAGE_SIZE) ? FLASH_PAGE_SIZE : size;

        // 准备写入的数据
        for (size_t i = 0; i < chunk_size; i += 8)
        {
            uint64_t value_to_write = 0;
            for (size_t j = 0; j < 8 && (i + j) < chunk_size; j++)
            {
                value_to_write |= ((uint64_t)buf[i + j]) << (j * 8);
            }

            status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, page_start + i, value_to_write);
            if (status != HAL_OK)
            {
                // 错误处理
                printf("Error: HAL_FLASH_Program.\r\n");
                HAL_FLASH_Lock();
                return -1;
            }
        }

        // 更新指针和大小
        buf += chunk_size;
        page_start += chunk_size;
        size -= chunk_size;
    }

    // 锁定闪存
    HAL_FLASH_Lock();

    return size;
}

int flash_erase(flash_t *dev,flash_ctl_t *ctl)
{

    HAL_StatusTypeDef status;
    uint32_t page_error = 0; 

    // 确保地址对齐到页边界
    if (ctl->offset % FLASH_PAGE_SIZE != 0)
    {
        // 错误处理：地址未对齐到页边界
        printf("Error: Address not aligned to page boundary.\r\n");
        return 0;
    }
    // 计算需要擦除的页数
    uint32_t  nb_pages = ctl->size / FLASH_PAGE_SIZE;// 先计算整数部分
    if (ctl->size % FLASH_PAGE_SIZE != 0) {// 如果有余数，说明数据跨越了页边界
        nb_pages++;// 需要额外擦除一个页
    }

    // 解锁闪存
    HAL_FLASH_Unlock();
    dev->EraseInitStruct.Page = (ctl->offset - START_ADDRESS) / FLASH_PAGE_SIZE;
    dev->EraseInitStruct.NbPages = nb_pages; // 设置需要擦除的页数

    // 打印EraseInitStruct的值
    /*
    printf("Erasing pages: TypeErase=%d, Banks=%d, Page=%d, NbPages=%d\r\n",
        EraseInitStruct.TypeErase, EraseInitStruct.Banks,
        EraseInitStruct.Page, EraseInitStruct.NbPages);
        */

    // 擦除指定的页
    status = HAL_FLASHEx_Erase(&dev->EraseInitStruct, &page_error);
    if (status != HAL_OK)
    {
        printf("Error: HAL_FLASHEx_Erase.\r\n");
        HAL_FLASH_Lock();
        return -1;
    }

    // 锁定闪存
    HAL_FLASH_Lock();

    return 0;
}
