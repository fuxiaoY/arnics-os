#include "flash.h"

#define START_ADDRESS 0x08000000

static int ef_err_port_cnt = 0;
int on_ic_read_cnt  = 0;
int on_ic_write_cnt = 0;

void feed_dog(void)
{
    //IWDG_ReloadCounter();
}
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



int bsp_flash_write(flash_t *dev, const uint8_t *buf, size_t size)
{
    HAL_StatusTypeDef status;
    size_t   i;
    uint32_t addr = START_ADDRESS + dev->offset;
    size_t mcu_write_gran = 4;
    __ALIGN_BEGIN uint32_t write_data __ALIGN_END;
    __ALIGN_BEGIN uint32_t read_data  __ALIGN_END;  

    if(addr%mcu_write_gran != 0)
    {
        ef_err_port_cnt++;
        printf("addr align error(0x%x)!\r\n", addr); 
        return -1;
    }


    HAL_FLASH_Unlock();

  /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR); 

  /* Clear PEMPTY bit set (as the code is executed from Flash which is not empty) */
  if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_PGERR) != 0)
  {
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGERR);
  }

    for (i = 0; i < size; i += mcu_write_gran, buf+=mcu_write_gran, addr += mcu_write_gran) {
        memcpy(&write_data, buf, mcu_write_gran); //The third parameter used to ensure HAL_FLASH_Program is the alignment of the first address in memory
        if (write_data != 0xFFFFFFFF)
        {
            // 编程
            status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, write_data);
        }
        read_data = *(uint32_t *)addr;
        /* You can add your code under here. */
        if (status != HAL_OK)
        {
            printf("Partition HAL_FLASH_Programe(0x%x) write error!\r\n", addr);  
            HAL_FLASH_Lock(); 
            return -1;
        }

        if (read_data != write_data) {
            HAL_FLASH_Lock(); 
            return -1;
        }
        else{
			//FLash操作可能非常耗时，如果有看门狗需要喂狗，以下代码由用户实现
           feed_dog();
        }
    }
    HAL_FLASH_Lock();

    on_ic_write_cnt++;
    return size;
}

int flash_erase(flash_t *dev,flash_ctl_t *ctl)
{

    HAL_StatusTypeDef flash_status;
    size_t erase_pages, i;
    uint32_t PAGEError = 0;

      erase_pages = ctl->size / FLASH_PAGE_SIZE;
    if (ctl->size % FLASH_PAGE_SIZE != 0) {
        erase_pages++;

    }
    
    //FLASH_EraseInitTypeDef EraseInitStruct;
    //EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    //EraseInitStruct.Banks = FLASH_BANK_1; 
    //EraseInitStruct.NbPages     = 1;  //Wipe out one sector at a time to perform one feeding of the dog to prevent timeout
    HAL_FLASH_Unlock();
   /* Erase the user Flash area
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

  /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR); 

  /* Clear PEMPTY bit set (as the code is executed from Flash which is not empty) */
  if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_PGERR) != 0)
  {
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGERR);
  }
   
    for (i = 0; i < erase_pages; i++) {
        dev->EraseInitStruct.PageAddress = (ctl->offset/FLASH_PAGE_SIZE) + i;
        flash_status = HAL_FLASHEx_Erase(&dev->EraseInitStruct, &PAGEError);
        if (flash_status != HAL_OK) {
            HAL_FLASH_Lock(); 
            printf("Partition HAL_FLASHEx_Erase Page(%d) write error!\r\n", dev->EraseInitStruct.PageAddress); 
            return -1;
        }
        else{
			//FLash operation can be very time-consuming, if there is a watchdog that needs to feed the dog, the following code is implemented by the user
            feed_dog();
        }         
    }
    HAL_FLASH_Lock(); 

    return ctl->size;
}




