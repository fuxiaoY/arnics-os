#include "dataStore.h"
#include "port/portInclude.h"
#include "thirdParty/AlgorithmLib/crc16.h"
#include "dataPlat/dataManager/dataManagerList.h"
#if PLATFORM_MCU
#pragma arm section code = "._entry_dataplat_api"
#endif

bool value_pack_with_crc16(const store_operation_t* op,uint8_t* src_buf,size_t src_len) 
{
    // 值传递
    memcpy((uint8_t*)op->data_struct_ptr + op->data_offset,src_buf,src_len);
    // CRC回填
    uint16_t* crc_ptr = (uint16_t*)((uint8_t*)op->data_struct_ptr + op->crc_offset);
    *crc_ptr = DL_CRC16((uint8_t*)op->data_struct_ptr + op->data_offset, op->data_len);
    return true;
}
bool value_load_with_crc16(const store_operation_t* op,uint8_t* dest_buf,size_t dest_len) 
{
    uint16_t* crc_ptr = (uint16_t*)((uint8_t*)op->data_struct_ptr + op->crc_offset);
    if(*crc_ptr != DL_CRC16((uint8_t*)op->data_struct_ptr + op->data_offset, op->data_len))
    {
        printf("CRC auth failed\r\n");
        return false;
    }
    // 值拷贝
    memcpy(dest_buf,(uint8_t*)op->data_struct_ptr + op->data_offset,dest_len);
    return true;
}


// ---------------------------------------------------------
// 统一的对外 API 接口 (业务层调用)
// ---------------------------------------------------------
bool data_save(const char *part_name,size_t offset,uint8_t *buf ,size_t len)
{
    if (!internal_plat_erase(part_name, offset, len)) 
    {
        return false;
    }
#if PLATFORM_MCU
	Delay_ms(20);
#endif
    if (!internal_plat_write(part_name, offset, buf, len)) 
    {
        return false;
    }
#if PLATFORM_MCU
	Delay_ms(20);
#endif
    return true;
}

bool data_load(const char *part_name,size_t offset,uint8_t *buf ,size_t len)
{
    if (!internal_plat_read(part_name, offset, buf, len)) 
    {
        return false;
    }
#if PLATFORM_MCU
	Delay_ms(20);
#endif
    return true;
}





