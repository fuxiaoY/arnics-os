#include "rolldbPort.h"
#include "../fal/inc/fal.h"
#include "../../drivers/driversInclude.h"
#include "../../rtosInterface/entry_rtos_api.h"
static rollts_sys_t rollts_sys;
static rollts_log_t rollts_log;
static bool ts_init_flag = false;

void task_suspend(void)
{
#ifdef RTOS_MUTEX_ENABLE
    rtosTaskSuspendAll();
#endif
}
void task_resume(void)
{
#ifdef RTOS_MUTEX_ENABLE
    rtosTaskResumeAll();
#endif
}


static bool tsdb_erase_sector(uint32_t address)
{
    const struct fal_partition *part = fal_partition_find("rolldb_tsdb");
    if (part)
    {
        if(0 >fal_partition_erase(part, address, SINGLE_SECTOR_SIZE))
        {
            return false; // 擦除失败
        }
        return true; // 擦除成功
    } 
    return false; // 擦除失败

}
static bool tsdb_write_data(uint32_t address, void *data, uint32_t length)
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
                return false; // 写入失败
            }

            // 更新地址和剩余长度
            address += bytes_to_write;
            data_ptr += bytes_to_write;
            length -= bytes_to_write;

            // 重置页偏移
            page_offset = 0;
        }
        return true;
    }
    return false;
}


static bool tsdb_read_data(uint32_t address, void *data, uint32_t length)
{
    const struct fal_partition *part = fal_partition_find("rolldb_tsdb");
    if (part)
    {
        if(0 >fal_partition_read(part, address, (uint8_t*)data, length))
        {
            return false; // 读取失败
        }
        return true;
    }
    return false;
}

flash_ops_t rolldb_flash_ops = 
{
    .erase_sector = tsdb_erase_sector,
    .write_data = tsdb_write_data,
    .read_data = tsdb_read_data
};


// 回调函数：打印读取的日志信息
static bool print_log_callback(uint8_t *buf, uint32_t len) 
{
    if (buf == NULL || len == 0) {
        return false; // 如果数据无效，停止读取
    }

    log_info("%.*s", len, buf); // 打印日志数据
    return true; // 返回 true 继续读取
}


void ts_init(void)
{
    rollts_init(&rollts_sys);
    log_info("roll DB (%s) initialize success",ROLLDB_VERSION);
    ts_init_flag = true;
}
void ts_deinit(void)
{
    rollts_deinit(&rollts_sys); // 注销数据库
    log_info("roll DB %s deinitialize success",ROLLDB_VERSION);
    ts_init_flag = false;
}
void ts_clear(void)
{
    if(true == ts_init_flag)
    {
        rollts_clear(&rollts_sys); // 清空数据库
    }
}

void ts_record(uint8_t *data, uint32_t len)
{
    if(true == ts_init_flag)
    {
        rollts_add(&rollts_sys, &rollts_log, data, len); // 添加日志
    }
}

void ts_read_all()
{
    if(true == ts_init_flag)
    {
        rollts_read_all(&rollts_sys, &rollts_log, print_log_callback); // 读取所有日志并打印
    }
}

void ts_read_pick(uint32_t start, uint32_t end)
{
    if(true == ts_init_flag)
    {
        rollts_read_pick(&rollts_sys, &rollts_log, start, end, print_log_callback); // 读取指定范围的日志并打印
    }
}

uint32_t ts_read_num(void)
{
    if(true == ts_init_flag)
    {
        return rollts_num(&rollts_sys);
    }
    return 0;
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
    if(true == ts_init_flag)
    {
        rollts_repair_logs(&rollts_sys,&rollts_log); // 修复数据库
    }
}
void ts_read_sysinfo(void)
{
    ULOG_ALWAYS("sys info->magic_valid: 0x%x",rollts_sys.magic_valid); 
    ULOG_ALWAYS("sys info->sys_info_len: 0x%x",rollts_sys.sys_info_len); 
    ULOG_ALWAYS("sys info->sys_addr_current_sector: 0x%x",rollts_sys.sys_addr_current_sector); 
    ULOG_ALWAYS("sys info->sys_addr_offset: 0x%x",rollts_sys.sys_addr_offset); 
    ULOG_ALWAYS("sys info->sys_size: %d",rollts_sys.sys_size);
    ULOG_ALWAYS("sys info->log_size: %d",rollts_sys.log_size);
    
    ULOG_ALWAYS("sys info->log_data_start: 0x%x",rollts_sys.log_data_start);
    ULOG_ALWAYS("sys info->log_data_end: 0x%x",rollts_sys.log_data_end);
    ULOG_ALWAYS("sys info->log_num: %d",rollts_sys.log_num);
    ULOG_ALWAYS("sys info->log_action_num: %d",rollts_sys.log_action_num);
    ULOG_ALWAYS("sys info->log_current_sector: %d",rollts_sys.log_current_sector);
}

#include "../unity/api_unity_def.h"

void rolldbSetUp(void) 
{
    rollts_clear(&rollts_sys);
    rollts_init(&rollts_sys); // 初始化数据库
}
void rolldbTearDown(void) 
{
    // rollts_deinit(&rollts_sys); // 注销数据库
}


void testRollDB(void) 
{
    uint8_t data[SINGLE_ROLLTS_MAX_SIZE] = {0}; // 初始化 data 数组为全 0
    uint32_t count = 0;

    // 测试日志追加
    sprintf((char *)data, "Hello, this is a test message this is a test message. this is a test message.. %d", count); 
    rollts_add(&rollts_sys, &rollts_log, data, strlen((char *)data)); 

    TEST_ASSERT_EQUAL_UINT32(1, rollts_sys.log_num); // 验证日志数量是否为 1

    uint32_t log_num = rollts_num(&rollts_sys);
    printf("Log number: %d\n", log_num); // 打印日志数量
    uint32_t capacity = rollts_capacity(&rollts_sys);
    printf("Capacity: %d%%\n", capacity); // 打印日志容量
    uint32_t  capacity_size = rollts_capacity_size(&rollts_sys); 
    printf("Capacity size: %d\n", capacity_size); // 打印日志容量大小

    // 测试日志读取并打印
    printf("Reading all logs:\n");
    rollts_read_all(&rollts_sys, &rollts_log, print_log_callback);

    // 测试日志追加多条
    for (count = 1; count <= 200; count++) 
    {
        //printf("Added log: %d  next_addr :%x\n", count, rollts_sys.log_data_end); // 打印添加的日志
        sprintf((char *)data, "Hello, this is a test message this is a test message. this is a test message.. %d", count);
        rollts_add(&rollts_sys, &rollts_log, data, strlen((char *)data));

    }

    // 测试日志读取并打印
    printf("Reading all logs:\n");
    rollts_read_all(&rollts_sys, &rollts_log, print_log_callback);

    // 测试选择性日志读取并打印
    printf("Reading logs from 157 to 159:\n");
    rollts_read_pick(&rollts_sys, &rollts_log, 157, 159, print_log_callback);

}
