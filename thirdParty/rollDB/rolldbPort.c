#include "rolldbPort.h"
#include "thirdParty/fal/inc/fal.h"
#include "drivers/driversInclude.h"
#include "rtosInterface/entry_rtos_api.h"
#include "thirdParty/softMutex/soft_mutex.h"

static soft_mutex_t rolldb_soft_mutex;   
void task_suspend(void)
{
    soft_mutex_lock(&rolldb_soft_mutex);
}
void task_resume(void)
{
    soft_mutex_unlock(&rolldb_soft_mutex);
}


static int tsdb_erase_sector(uint32_t address)
{
    const struct fal_partition *part = fal_partition_find("rolldb_tsdb");
    if (part)
    {
        if(0 >fal_partition_erase(part, address, SINGLE_BLOCK_SIZE))
        {
            return -1; // 擦除失败
        }
        return 0; // 擦除成功
    } 
    return -1; // 擦除失败

}
static int tsdb_write_data(uint32_t address, void *data, uint32_t length)
{
    const struct fal_partition *part = fal_partition_find("rolldb_tsdb");
    if (part)
    {
        uint32_t page_size = 256; // 假设 Flash 页大小为 256 字节
        uint32_t page_offset = address % page_size;
        uint32_t bytes_to_write = 0;
        uint8_t *data_ptr = (uint8_t *)data;
        while (length > 0)
        {
            // 计算当前页剩余空间
            bytes_to_write = page_size - page_offset;
            if (bytes_to_write > length)
                bytes_to_write = length;

            // 写入当前页
            if(0 > fal_partition_write(part, address, data_ptr, bytes_to_write))
            {
                return -1; // 写入失败
            }

            // 更新地址和剩余长度
            address += bytes_to_write;
            data_ptr += bytes_to_write;
            length -= bytes_to_write;

            // 重置页偏移
            page_offset = 0;
        }
        return 0;
    }
    return -1;
}


static int tsdb_read_data(uint32_t address, void *data, uint32_t length)
{
    const struct fal_partition *part = fal_partition_find("rolldb_tsdb");
    if (part)
    {
        if(0 >fal_partition_read(part, address, (uint8_t*)data, length))
        {
            return -1; // 读取失败
        }
        return 0;
    }
    return -1;
}



rollts_manager_t rollts_sys = 
{
    .flash_ops.erase_sector = tsdb_erase_sector,
    .flash_ops.write_data   = tsdb_write_data,
    .flash_ops.read_data    = tsdb_read_data,
    .flash_ops.mutex_lock   = task_suspend,
    .flash_ops.mutex_unlock = task_resume
};
// 回调函数：打印读取的日志信息
static bool print_log_callback(uint8_t *buf, uint32_t len) 
{
    if (buf == NULL || len == 0) {
        return -1; // 如果数据无效，停止读取
    }
    buf[len - 1] = '\0';
    ULOG_ALWAYS("%.*s", len, buf); // 打印日志数据
    return 0; // 返回 0 继续读取
}


void ts_init(void)
{
    soft_mutex_init(&rolldb_soft_mutex);
    rollts_init(&rollts_sys);
    log_info("roll DB (%s) initialize success",ROLLDB_VERSION);
}

void ts_clear(void)
{
    rollts_clear(&rollts_sys); // 清空数据库
}


void ts_record(uint8_t *data, uint32_t len)
{
    rollts_add(&rollts_sys, data, len); // 添加日志
}

void ts_read_all()
{
    uint8_t data_cache[1024] = {0};
    rollts_get_all(&rollts_sys, data_cache, 1024,print_log_callback);
}

void ts_read_pick(uint32_t start, uint32_t end)
{

    uint8_t data_cache[1024] = {0};
    rollts_read_pick(&rollts_sys, start, end, data_cache, 1024,print_log_callback);

    
}

void ts_read_latest(uint32_t num)
{

}

uint32_t ts_read_num(void)
{
    return rollts_get_total_record_number(&rollts_sys);

}

uint32_t ts_read_capacity(void)
{
    return rollts_capacity(&rollts_sys);
}

uint32_t ts_read_capacity_size(void)
{
    return rollts_capacity_size(&rollts_sys);
}
void ts_repair(void)
{

}
void ts_read_sysinfo(void)
{

}
