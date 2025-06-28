#include "dataStore.h"
#include "thirdParty/thirdPartyInclude.h"
#include "thirdParty/AlgorithmLib/crc16.h"
#include "dataPlat/dataManager/dataManagerList.h"

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
bool data_save(const char *part_name,size_t offset,uint8_t *buf ,size_t len)
{
    int ret = 0;
	struct fal_partition fal_part, *P_fal_part;
	P_fal_part = &fal_part;
	P_fal_part = (struct fal_partition *)fal_partition_find(part_name);
	if(P_fal_part == NULL)
	{
		printf("fal partition error\r\n");
		return false;
	}
	ret = fal_partition_erase(P_fal_part,offset,len);
    if(-1 == ret)
    {
        return false;
    }
	Delay_ms(20);
	ret = fal_partition_write(P_fal_part,offset, buf, len);
    if(-1 == ret)
    {
        return false;
    }
	Delay_ms(20);
    return true;
}

bool data_load(const char *part_name,size_t offset,uint8_t *buf ,size_t len)
{
    int ret = 0;
	struct fal_partition fal_part, *P_fal_part;
	P_fal_part = &fal_part;
	P_fal_part = (struct fal_partition *)fal_partition_find(part_name);
	if(P_fal_part == NULL)
	{
		printf("fal partition error\r\n");
		return false;
	}
    ret = fal_partition_read(P_fal_part,offset, buf, len);
    if(-1 == ret)
    {
        return false;
    }
	Delay_ms(20);
    return true;
}





