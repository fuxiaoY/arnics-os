#include "rollTs.h" // 包含日志数据库的头文件
#include <string.h> 

#define VALID_MAGIC 0xA5A5A5A5 // 定义一个有效的魔数，用于验证系统分区的有效性

static bool check_addr_in_sector(uint32_t addr, uint32_t sector_start_addr) 
{
    return (addr >= sector_start_addr && addr < (sector_start_addr + SINGLE_SECTOR_SIZE)); // 检查地址是否在扇区范围内
}


// 初始化日志数据库
bool rollts_init(rollts_sys_t *sys_handle) 
{
    if (!sys_handle) return false; // 如果句柄为空，直接返回

    memset(sys_handle, 0, sizeof(rollts_sys_t)); 
    for(uint8_t i= 0; i < SYSINFO_NUM; i++)
    {
        if(false == rolldb_flash_ops.read_data(SYSINFO_START_ADDR + i * SINGLE_SECTOR_SIZE, sys_handle, sizeof(rollts_sys_t))) // 从 Flash 中读取系统分区数据到 sys_handle 结构体中
        {
            return false;
        }

        if ((sys_handle->magic_valid != VALID_MAGIC) ||(sys_handle->sys_size != SYSINFO_SIZE )||(sys_handle->log_size != LOG_SECTOR_SIZE))
        {
            continue; // 如果系统分区无效，继续读取下一个扇区
        }
        else
        {            
            //该扇区有效,就循环读取，直到出现无效的系统分区
            rollts_sys_t sys_handle_temp;
            memcpy(&sys_handle_temp, sys_handle, sizeof(rollts_sys_t)); // 复制当前系统分区数据到临时变量
            uint32_t count = 0;
            do
            {
                memcpy(sys_handle, &sys_handle_temp, sizeof(rollts_sys_t)); 
                // 如果地址超过本扇区范围，则跳出循环
                // 操作备份区域
                if(false == check_addr_in_sector(sys_handle_temp.sys_addr_offset,SYSINFO_START_ADDR + i * SINGLE_SECTOR_SIZE))
                {
                    break;
                }
                if(false == rolldb_flash_ops.read_data(sys_handle_temp.sys_addr_offset, &sys_handle_temp, sizeof(rollts_sys_t)))
                {
                    return false;
                }
                count++;
                if(count > 100)
                {
                    sys_handle_temp.magic_valid = 0x00;
                    break;
                }
            }while(sys_handle_temp.magic_valid == VALID_MAGIC);
            break;
        }
    }

    // 检查系统分区是否有效
    if ((sys_handle->magic_valid != VALID_MAGIC) ||(sys_handle->sys_size != SYSINFO_SIZE )||(sys_handle->log_size != LOG_SECTOR_SIZE))
    { 
        log_info(" rollTs invalid! reinit...");
        // 初始化系统分区
        sys_handle->magic_valid = VALID_MAGIC; // 设置系统分区有效标志
        sys_handle->sys_info_len = sizeof(rollts_sys_t); // 设置系统分区数据长度
        sys_handle->sys_addr_current_sector = SYSINFO_START_ADDR; // 当前系统分区地址偏移
        sys_handle->sys_addr_offset = SYSINFO_START_ADDR + sizeof(rollts_sys_t);
        sys_handle->sys_size = SYSINFO_SIZE;    // 设置系统分区大小
        sys_handle->log_size = LOG_SECTOR_SIZE; // 设置日志分区大小
        sys_handle->log_data_start = LOG_SECTOR_START_ADDR; // 初始化日志起始地址
        sys_handle->log_data_end = LOG_SECTOR_START_ADDR; // 初始化日志结束地址
        sys_handle->log_num = 0; // 初始化日志数量为 0
        sys_handle->log_action_num = 0;
        sys_handle->log_current_sector = 0; // 初始化当前写入扇区为第一个扇区

        // 初始化日志分区状态
        memset(sys_handle->log_sector_status, SECTOR_EMPTY, sizeof(sys_handle->log_sector_status)); // 将所有扇区状态设置为 EMPTY
        
        // 擦除系统分区
        for(uint32_t i = 0; i < SYSINFO_NUM; i++) 
        { 
            if(false == rolldb_flash_ops.erase_sector(SYSINFO_START_ADDR + i * SINGLE_SECTOR_SIZE)) // 擦除当前扇区
            {
                return false;
            }
            else
            {
                log_info(" Sysinfo Sector erase  at : 0x%x ", (SYSINFO_START_ADDR + i * SINGLE_SECTOR_SIZE));
            }
        }
        // 擦除日志分区
        for (uint32_t i = 0; i < LOG_SECTOR_NUM; i++) 
        { 
            if(false == rolldb_flash_ops.erase_sector(LOG_SECTOR_START_ADDR + i * SINGLE_SECTOR_SIZE))
            {
                return false;
            }
            else
            {
                log_info(" Log Sector erase  at : 0x%x ", (LOG_SECTOR_START_ADDR + i * SINGLE_SECTOR_SIZE));
            }
        }
        if(false == rolldb_flash_ops.write_data(SYSINFO_START_ADDR, sys_handle, sizeof(rollts_sys_t))) // 将初始化后的系统分区数据写入 Flash
        {
            return false;
        }
    }
    log_info("sys_handle->sys_addr_current_sector:0x%x, sys_handle->sys_addr_offset:0x%x", sys_handle->sys_addr_current_sector, sys_handle->sys_addr_offset); 
    log_info("sys_handle->log_data_start:0x%x, sys_handle->log_data_end:0x%x", sys_handle->log_data_start, sys_handle->log_data_end); 
    log_info("sys_handle->log_num:%d", sys_handle->log_num); // 输出日志数量
    log_info("rollts_init succeed!"); // 输出系统分区有效标志

    return true;
}
bool rollts_deinit(rollts_sys_t *sys_handle) 
{
    // 擦除系统分区
    for(uint32_t i = 0; i < SYSINFO_NUM; i++) 
    { 
        if(false == rolldb_flash_ops.erase_sector(SYSINFO_START_ADDR + i * SINGLE_SECTOR_SIZE)) // 擦除当前扇区
        {
            return false;
        }
    }
	return true;
}

// 清除所有日志数据
bool rollts_clear(rollts_sys_t *sys_handle) 
{
    if (!sys_handle) return false; // 如果句柄为空，直接返回
    log_debug("rollts clear start..."); 
    task_suspend();
    // 初始化系统分区
    sys_handle->magic_valid = VALID_MAGIC; // 设置系统分区有效标志
    sys_handle->sys_info_len = sizeof(rollts_sys_t); // 设置系统分区数据长度
    sys_handle->sys_addr_current_sector = SYSINFO_START_ADDR; // 当前系统分区地址偏移
    sys_handle->sys_addr_offset = SYSINFO_START_ADDR + sizeof(rollts_sys_t);
    sys_handle->sys_size = SYSINFO_SIZE;    // 设置系统分区大小
    sys_handle->log_size = LOG_SECTOR_SIZE; // 设置日志分区大小
    sys_handle->log_data_start = LOG_SECTOR_START_ADDR; // 初始化日志起始地址
    sys_handle->log_data_end = LOG_SECTOR_START_ADDR; // 初始化日志结束地址
    sys_handle->log_num = 0; // 初始化日志数量为 0
    sys_handle->log_action_num = 0;
    sys_handle->log_current_sector = 0; // 初始化当前写入扇区为第一个扇区

    // 初始化日志分区状态
    memset(sys_handle->log_sector_status, SECTOR_EMPTY, sizeof(sys_handle->log_sector_status)); // 将所有扇区状态设置为 EMPTY
    // 擦除系统分区
    for(uint32_t i = 0; i < SYSINFO_NUM; i++) 
    { 
        if(false == rolldb_flash_ops.erase_sector(SYSINFO_START_ADDR + i * SINGLE_SECTOR_SIZE)) // 擦除当前扇区
        {
            task_resume();
            return false;
        }
    }
    // 擦除日志分区
    for (uint32_t i = 0; i < LOG_SECTOR_NUM; i++) 
    { 
        if(false == rolldb_flash_ops.erase_sector(LOG_SECTOR_START_ADDR + i * SINGLE_SECTOR_SIZE))
        {
            task_resume();
            return false;
        }
    }
    if(false == rolldb_flash_ops.write_data(SYSINFO_START_ADDR, sys_handle, sizeof(rollts_sys_t))) // 将初始化后的系统分区数据写入 Flash
    {
        task_resume();
        return false;
    }
    log_debug("rollts_init succeed! magic_valid: %x", sys_handle->magic_valid); // 调试输出系统分区有效标志
    log_debug("sys_handle->log_data_start:%x, sys_handle->log_data_end:%x", sys_handle->log_data_start, sys_handle->log_data_end); 
    task_resume();
    return true;
}


static void add_log_action_num(rollts_sys_t *sys_handle)
{
    sys_handle->log_action_num++;
}
// 静态函数：遍历当前扇区日志数量
static uint32_t rollts_count_logs_in_sector(uint32_t sector_start_addr) 
{
    uint32_t erase_num = 0; // 初始化被擦除日志的数量
    uint32_t sector_end_addr = sector_start_addr + SINGLE_SECTOR_SIZE; // 当前扇区结束地址
    uint32_t read_addr = sector_start_addr; // 从扇区起始地址开始读取

    // 遍历该扇区，读取日志头并检查有效性
    while (read_addr < sector_end_addr) 
    {
        rollts_log_t log; // 定义日志头结构
        memset(&log, 0, sizeof(rollts_log_t)); // 清空日志头结构
        if(false == rolldb_flash_ops.read_data(read_addr, (uint8_t *)&log, sizeof(rollts_log_t))) // 从 Flash 中读取日志头
        {
            return erase_num; // 如果读取失败，返回已计数日志数量
        }
        // 检查日志是否有效
        if (log.magic_valid != VALID_MAGIC) 
        {
            break; // 如果日志无效，停止遍历
        }
        // 有效日志，增加计数器

        erase_num++;
        read_addr = log.addr_offset; // 更新读取地址

    }
    return erase_num; // 返回扇区内有效日志的数量
}

// 检查并切换到下一个扇区
static bool rollts_switch_sector(rollts_sys_t *sys_handle) 
{

    uint32_t previous_sector = sys_handle->log_current_sector; // 保存之前扇区索引
    if(sys_handle->log_current_sector + 1 >= LOG_SECTOR_NUM)
    {
        sys_handle->log_current_sector = 0; // 如果当前扇区索引超过最大值，则重置为 0
    }
    else
    {
        sys_handle->log_current_sector++; // 切换到下一个扇区
    }
    log_debug("block jump action triggerd %d->%d", previous_sector,sys_handle->log_current_sector); 
    // 仅当被擦除的扇区是 log_data_start 所在扇区时，更新 log_data_start 先更新起始地址，再擦除
    if ((LOG_SECTOR_START_ADDR + sys_handle->log_current_sector * SINGLE_SECTOR_SIZE) == sys_handle->log_data_start) 
    {
        if(sys_handle->log_current_sector + 1 < LOG_SECTOR_NUM) //在扇区范围内
        {
            sys_handle->log_data_start = LOG_SECTOR_START_ADDR + (sys_handle->log_current_sector + 1) * SINGLE_SECTOR_SIZE; // 更新 log_data_start 为下一个扇区的起始地址
        }
        else
        {
            sys_handle->log_data_start = LOG_SECTOR_START_ADDR; // 如果当前扇区是最后一个扇区，则将 log_data_start 重置为第一个扇区的起始地址
        }
        log_debug(" new:sys_handle->log_data_start:%x", sys_handle->log_data_start); 
    }

    // 如果新切换的扇区已满（健壮性处理，不等于 EMPTY），则擦除
    if (sys_handle->log_sector_status[sys_handle->log_current_sector] != SECTOR_EMPTY) 
    {
        uint32_t sector_start_addr = LOG_SECTOR_START_ADDR + sys_handle->log_current_sector * SINGLE_SECTOR_SIZE; // 当前扇区起始地址

        // 调用独立函数计算当前扇区日志数量
        uint32_t erase_num = rollts_count_logs_in_sector(sector_start_addr);

        // 从系统分区中删除被擦除扇区的日志数量
        sys_handle->log_num -= erase_num;
        if (sys_handle->log_num < 0) 
        {
            sys_handle->log_num = 0; // 防止日志数量变为负数
        }
        log_debug("erase_num:%d, sys_handle->log_num:%d", erase_num, sys_handle->log_num);
        if(false == rolldb_flash_ops.erase_sector(sector_start_addr)) // 擦除当前扇区
        {
            return false; 
        }
        sys_handle->log_sector_status[sys_handle->log_current_sector] = SECTOR_EMPTY; // 更新扇区状态为 EMPTY
    }

    // 更新日志分区结束地址
    sys_handle->log_data_end = LOG_SECTOR_START_ADDR + sys_handle->log_current_sector * SINGLE_SECTOR_SIZE; // 更新 log_data_end 为当前扇区的起始地址
    log_debug("sys_handle->log_data_start:%x, sys_handle->log_data_end:%x", sys_handle->log_data_start, sys_handle->log_data_end); 
    return true;
}

// 写入日志头
static bool rollts_write_log_header(rollts_sys_t *sys_handle, rollts_log_t *log_handle,uint16_t length) 
{
    log_handle->magic_valid = VALID_MAGIC; // 设置日志有效标志
    add_log_action_num(sys_handle);
    log_handle->number = sys_handle->log_action_num; // 设置日志操作序号
    log_handle->len = length + sizeof(rollts_log_t); // 设置日志长度
    log_handle->addr_offset = sys_handle->log_data_end + log_handle->len; // 设置日志地址偏移
    if(false == rolldb_flash_ops.write_data(sys_handle->log_data_end, (uint8_t *)log_handle, sizeof(rollts_log_t))) // 写入日志头到 Flash
    {
        return false; // 写入失败
    }
    return true; // 写入成功
}

// 写入日志数据
static bool rollts_write_log_data(rollts_sys_t *sys_handle, uint8_t *data, uint16_t length) 
{
    if(false == rolldb_flash_ops.write_data(sys_handle->log_data_end, data, length)) // 写入日志数据到 Flash
    {
        return false; // 写入失败
    }
    return true; // 写入成功
}

// 更新系统分区数据
// 定义一个静态函数 rollts_update_sysinfo，用于更新系统信息
// 参数 sys_handle 是一个指向 rollts_sys_t 类型的指针，表示系统信息的句柄
static bool rollts_update_sysinfo(rollts_sys_t *sys_handle) 
{

    uint32_t addr_end_offset = sys_handle->sys_addr_offset + sizeof(rollts_sys_t); // 计算当前系统分区地址偏移
    if(check_addr_in_sector(addr_end_offset, sys_handle->sys_addr_current_sector))
    {
        uint32_t addr_offset = sys_handle->sys_addr_offset;
        sys_handle->sys_addr_offset += sizeof(rollts_sys_t); // 更新下一个系统分区地址偏移
        //偏移后地址在扇区内 可以直接写入
        if(false == rolldb_flash_ops.write_data(addr_offset, sys_handle, sizeof(rollts_sys_t))) // 将系统分区数据写入 Flash
        {
            return false; // 写入失败
        }
    }
    else
    {
        //下一个扇区
        // 擦除当前系统分区所在的 Flash 扇区
        if(false == rolldb_flash_ops.erase_sector(sys_handle->sys_addr_current_sector)) 
        {
            return false; // 擦除失败
        }
        sys_handle->sys_addr_current_sector += SINGLE_SECTOR_SIZE;    //更新
        if(sys_handle->sys_addr_current_sector >= SYSINFO_START_ADDR + SYSINFO_NUM * SINGLE_SECTOR_SIZE)
        {
            sys_handle->sys_addr_current_sector = SYSINFO_START_ADDR; // 如果当前系统分区地址超过最大值，则重置为起始地址
        }
        sys_handle->sys_addr_offset = sys_handle->sys_addr_current_sector + sizeof(rollts_sys_t);  // 下一个系统分区地址偏移
        // 将系统分区数据写入下一个 Flash
        if(false == rolldb_flash_ops.write_data(sys_handle->sys_addr_current_sector, sys_handle, sizeof(rollts_sys_t))) 
        {
            return false; // 写入失败
        }
    }
    return true; // 写入成功
}
static bool log_goto_next_sector(rollts_sys_t *sys_handle)
{
    return ((0 == sys_handle->log_data_end % SINGLE_SECTOR_SIZE) && (((sys_handle->log_data_end - LOG_SECTOR_START_ADDR)/SINGLE_SECTOR_SIZE)!= sys_handle->log_current_sector));
}
// 追加日志
bool rollts_add(rollts_sys_t *sys_handle, rollts_log_t *log_handle, uint8_t *data, uint16_t length) {
    if (!sys_handle || !log_handle || !data || length == 0) return false; // 如果参数无效，直接返回
    if (VALID_MAGIC != sys_handle->magic_valid) return false; // 如果系统分区无效，直接返回 
    if(length > ROLLTS_MAX_SIZE)
    {
        return false; // 如果日志长度超过最大限制，直接返回
    }
    task_suspend();
    // 检查当前扇区是否有足够空间 或偏移地址已经跨页到另一个扇区
    uint32_t sector_offset = sys_handle->log_data_end % SINGLE_SECTOR_SIZE; // 当前扇区的写入偏移量

    if ((sector_offset + sizeof(rollts_log_t) + length > SINGLE_SECTOR_SIZE) || (log_goto_next_sector(sys_handle) == true))
    { // 如果当前扇区空间不足
        sys_handle->log_sector_status[sys_handle->log_current_sector] = SECTOR_FULL; // 当前扇区写满，标记为 FULL
        if(false == rollts_switch_sector(sys_handle)) // 切换到下一个扇区
        {
            return false;
        }
    }

    // 写入日志头
    if(false == rollts_write_log_header(sys_handle, log_handle, length)) 
    {
        task_resume();
        return false; // 写入失败
    }
    sys_handle->log_data_end += sizeof(rollts_log_t); 
    // 写入日志数据
    if(false == rollts_write_log_data(sys_handle, data, length))
    {
        task_resume();
        return false; // 写入失败
    }
    sys_handle->log_data_end += length; 
    // 更新当前扇区状态为 WRITING
    sys_handle->log_sector_status[sys_handle->log_current_sector] = SECTOR_WRITTING; 
    sys_handle->log_num++; 
    log_debug("rollts_add log_num:%d, sys_handle->log_data_start:%x, sys_handle->log_data_end:%x log->number:%d", sys_handle->log_num, sys_handle->log_data_start, sys_handle->log_data_end,log_handle->number);
    // 更新系统分区数据
    if(false == rollts_update_sysinfo(sys_handle))
    {
        task_resume();
        return false; // 更新失败
    }
    task_resume();
    return true;
}

static uint8_t data[SINGLE_ROLLTS_MAX_SIZE] = {0}; // 定义日志数据缓冲区
// 批量读取日志
bool rollts_read_all(rollts_sys_t *sys_handle, rollts_log_t *log_handle, rollTscb cb) 
{
    if (!sys_handle || !log_handle || !cb) return false; // 如果参数无效，直接返回
    if (VALID_MAGIC != sys_handle->magic_valid) return false; // 如果系统分区无效，直接返回 
    task_suspend();
    uint32_t log_count = 0; // 初始化日志计数器
    uint32_t read_addr = sys_handle->log_data_start; // 从日志起始地址开始读取
    while (read_addr != sys_handle->log_data_end) 
    { // 遍历所有日志
        rollts_log_t log; // 定义日志头结构
        memset(&log, 0, sizeof(rollts_log_t)); // 清空日志头结构

        if(false == rolldb_flash_ops.read_data(read_addr, (uint8_t *)&log, sizeof(rollts_log_t))) // 从 Flash 中读取日志头
        {
            task_resume();
            return false;
        }
        // 检查日志是否有效
        if (log.magic_valid != VALID_MAGIC) 
        {
            log_debug("invalid log block, try next sector!"); // 调试输出日志无效
            // 跳转到下一个扇区的起始地址
            uint32_t current_sector = (read_addr - LOG_SECTOR_START_ADDR) / SINGLE_SECTOR_SIZE;
            if (current_sector + 1 < LOG_SECTOR_NUM) 
            {
                read_addr = LOG_SECTOR_START_ADDR + (current_sector + 1) * SINGLE_SECTOR_SIZE;
            } 
            else 
            {
                // 如果已经是最后一个扇区，回到第一个扇区
                read_addr = LOG_SECTOR_START_ADDR;
            }
            continue; // 继续下一次循环

        }
        read_addr += sizeof(rollts_log_t); // 更新读取地址
        memset(data, 0, SINGLE_ROLLTS_MAX_SIZE);
        if(false == rolldb_flash_ops.read_data(read_addr, data, (log.len-sizeof(rollts_log_t)))) // 从 Flash 中读取日志数据
        {
            task_resume();
            return false;
        }
        read_addr = log.addr_offset; // 更新读取地址
        log_count++; // 增加日志计数器
        // 调用回调函数处理日志数据
        if (!cb(data, (log.len-sizeof(rollts_log_t)))) break; // 如果回调函数返回 false，则停止读取
        // 如果日志计数器超过总日志数量，停止读取
        if (log_count >= sys_handle->log_num) break;
    }
    log_debug("rollts num is :%d!", sys_handle->log_num); 
    task_resume();
    return true;
}

// 按范围读取日志
bool rollts_read_pick(rollts_sys_t *sys_handle, rollts_log_t *log_handle, uint32_t start_num, uint32_t end_num, rollTscb cb) {
    if (!sys_handle || !log_handle || !cb || start_num > end_num || end_num > sys_handle->log_num) return false; // 如果参数无效，直接返回
    if (VALID_MAGIC != sys_handle->magic_valid) return false; // 如果系统分区无效，直接返回 
    task_suspend();
    uint32_t read_addr = sys_handle->log_data_start; // 从日志起始地址开始读取
    uint32_t log_count = 0; // 初始化日志计数器
    //计算真实的log_num范围
    uint32_t valid_num = 0;
    uint32_t read_num = 0;
    while (read_addr != sys_handle->log_data_end) 
    { // 遍历所有日志
        rollts_log_t log; // 定义日志头结构
        memset(&log, 0, sizeof(rollts_log_t)); // 清空日志头结构
        if(false == rolldb_flash_ops.read_data(read_addr, (uint8_t *)&log, sizeof(rollts_log_t))) // 从 Flash 中读取日志头
        {
            task_resume();
            return false;
        }
        // 检查日志是否有效
        if (log.magic_valid != VALID_MAGIC) 
        {
            log_debug("invalid log block, try next sector!"); // 调试输出日志无效
            // 跳转到下一个扇区的起始地址
            uint32_t current_sector = (read_addr - LOG_SECTOR_START_ADDR) / SINGLE_SECTOR_SIZE;
            if (current_sector + 1 < LOG_SECTOR_NUM) 
            {
                read_addr = LOG_SECTOR_START_ADDR + (current_sector + 1) * SINGLE_SECTOR_SIZE;
            } 
            else 
            {
                // 如果已经是最后一个扇区，回到第一个扇区
                read_addr = LOG_SECTOR_START_ADDR;
            }
            continue; // 继续下一次循环

        }
        else
        {
            valid_num++;
        }
        if(valid_num >= start_num && valid_num <= end_num)
        {
            memset(data, 0, SINGLE_ROLLTS_MAX_SIZE);
            if(false == rolldb_flash_ops.read_data(read_addr + sizeof(rollts_log_t), data, (log.len-sizeof(rollts_log_t)))) // 从 Flash 中读取日志数据
            {
                task_resume();
                return false;
            }
            // 调用回调函数处理日志数据
            if (!cb(data, (log.len-sizeof(rollts_log_t)))) break; // 如果回调函数返回 false，则停止读取
            read_num++;
        }
        read_addr = log.addr_offset; // 更新读取地址
        log_count++; // 增加日志计数器

        // 如果读取量达到，停止读取
        if (read_num >= end_num - start_num + 1) break;
        // 如果日志计数器超过总日志数量，停止读取
        if (log_count >= sys_handle->log_num) break;
    }
    task_resume();
    return true;
}

// 当前日志数
uint32_t rollts_num(rollts_sys_t *sys_handle) 
{
    if (!sys_handle) return 0; // 如果句柄为空，直接返回 0
    if (VALID_MAGIC != sys_handle->magic_valid) return 0; // 如果系统分区无效，直接返回 
    return sys_handle->log_num; // 返回日志数量
}

// 当前剩余容量 返回百分比
uint32_t rollts_capacity(rollts_sys_t *sys_handle) 
{
    if (!sys_handle) return 0; // 如果句柄为空，直接返回 0
    if (VALID_MAGIC != sys_handle->magic_valid) return 0; // 如果系统分区无效，直接返回 
    // 通过查询sys_handle 结构体中的 log_sector_status，计算当前容量
    uint32_t capacity = 0;
    for (int i = 0; i < LOG_SECTOR_NUM; i++) 
    {
        if (sys_handle->log_sector_status[i] == SECTOR_EMPTY) 
        {
            capacity++; // 如果当前扇区为空，则计数
        }   
    }
    return (capacity * 100) / LOG_SECTOR_NUM; // 返回当前容量百分比
}

// 当前剩余容量大小 返回字节数
uint32_t rollts_capacity_size(rollts_sys_t *sys_handle) 
{
    if (!sys_handle) return 0; // 如果句柄为空，直接返回 0
    if (VALID_MAGIC != sys_handle->magic_valid) return 0; // 如果系统分区无效，直接返回 
    // 通过查询sys_handle 结构体中的 log_sector_status，计算当前容量
    uint32_t capacity = 0;
    for (int i = 0; i < LOG_SECTOR_NUM; i++) 
    {
        if (sys_handle->log_sector_status[i] == SECTOR_EMPTY) 
        {
            capacity += SINGLE_SECTOR_SIZE; // 如果当前扇区为空，则计数
        }   
    }
    return capacity; // 返回当前容量大小
}

// 添加日志修复功能
bool rollts_repair_logs(rollts_sys_t *sys_handle,rollts_log_t *log_handle)
{
    log_info("rollts_repair_logs start...");
    if (!sys_handle || !log_handle) return false; // 如果参数无效，直接返回
    task_suspend();
    rollts_log_t log; // 定义日志头结构
    // 修复结果
    uint32_t repair_log_num_record = 0;
    uint32_t repair_log_start_addr = LOG_SECTOR_START_ADDR;
    uint32_t repair_log_end_addr = LOG_SECTOR_START_ADDR;
    uint32_t repair_current_sector = 0;

    uint32_t loop_addr = LOG_SECTOR_START_ADDR; // 循环地址
    uint32_t current_sector = 0; // 当前扇区索引
    uint32_t count_number = 0;
    uint8_t get_first_valid = 0;
    do
    {
        if(false == rolldb_flash_ops.read_data(loop_addr, (uint8_t *)&log, sizeof(rollts_log_t))) // 从 Flash 中读取日志头
        {
            task_resume();
            return false;
        }

        // 检查日志是否有效
        if (VALID_MAGIC == log.magic_valid) 
        {
            if(0 == get_first_valid)
            {
                count_number = log.number; // 记录第一个有效日志的数量
                get_first_valid = 1;
            }
            else
            {
                if(1 == get_first_valid)
                {
                    if(log.number - count_number != 1)
                    {
                        //检测到日志号断点，更新日志起始结束地址
                        repair_log_start_addr = loop_addr; // 更新修复日志起始地址
                        repair_current_sector = current_sector; // 当前系统分区地址偏移
                        get_first_valid = 2;
                        log_info("log number is not continuous, repair log start addr is :0x%x!", repair_log_start_addr); // 输出日志起始地址
                    }
                    else
                    {
                        // 检测到日志号连续，更新日志结束地址
                        repair_log_end_addr = log.addr_offset; // 更新修复日志结束地址
                        count_number = log.number; 

                    }

                }
            }

            if(log.addr_offset / SINGLE_SECTOR_SIZE - SYSINFO_SIZE   != current_sector)
            {
                //地址偏移在当前扇区内
                sys_handle->log_sector_status[current_sector] = SECTOR_WRITTING; 
            }
            else
            {
                //地址偏移出当前扇区
                sys_handle->log_sector_status[current_sector] = SECTOR_FULL; 
            }

            repair_log_num_record++; // 记录修复日志数量
            loop_addr = log.addr_offset; // 更新读取地址

        }   
        else
        {
            if(0xFFFFFFFF == log.magic_valid)
            {
                if(0 == loop_addr%SINGLE_SECTOR_SIZE)// 如果地址在扇区头部
                {
                    sys_handle->log_sector_status[current_sector] = SECTOR_EMPTY; 
                    if(current_sector > 0)
                    {
                        sys_handle->log_sector_status[current_sector - 1] = SECTOR_FULL; 
                    }

                }
                else
                {
                    sys_handle->log_sector_status[current_sector] = SECTOR_WRITTING; 
                }

            }
            else
            {
                sys_handle->log_sector_status[current_sector] = SECTOR_FULL; // 如果日志头不为全F，则标记当前扇区为满
            }
            // 跳转到下一个扇区的起始地址
            current_sector = (loop_addr - LOG_SECTOR_START_ADDR) / SINGLE_SECTOR_SIZE;
            log_debug("invalid log block, try next sector! 0x%x",current_sector); // 调试输出日志无效
            if (current_sector + 1 < LOG_SECTOR_NUM) 
            {
                loop_addr = LOG_SECTOR_START_ADDR + (current_sector + 1) * SINGLE_SECTOR_SIZE;
            } 
            else 
            {
                log_debug("last sector! break."); // 调试输出最后一个扇区
                break; // 如果已经是最后一个扇区，退出循环
            }
        }

    }while(loop_addr <= (LOG_SECTOR_START_ADDR + LOG_SECTOR_NUM * SINGLE_SECTOR_SIZE - sizeof(rollts_log_t))); // 遍历所有日志

    // 擦除系统分区
    for(uint32_t i = 0; i < SYSINFO_NUM; i++) 
    { 
        if(false == rolldb_flash_ops.erase_sector(SYSINFO_START_ADDR + i * SINGLE_SECTOR_SIZE)) // 擦除当前扇区
        {
            return false;
        }
        else
        {
            log_info(" Sysinfo Sector erase  at : 0x%x ", (SYSINFO_START_ADDR + i * SINGLE_SECTOR_SIZE));
        }
    }
    sys_handle->magic_valid = VALID_MAGIC; // 设置系统分区有效标志
    sys_handle->sys_info_len = sizeof(rollts_sys_t); // 设置系统分区数据长度
    sys_handle->sys_addr_current_sector = SYSINFO_START_ADDR; // 当前系统分区地址偏移
    sys_handle->sys_addr_offset = SYSINFO_START_ADDR + sizeof(rollts_sys_t);
    sys_handle->sys_size = SYSINFO_SIZE;    // 设置系统分区大小
    sys_handle->log_size = LOG_SECTOR_SIZE; // 设置日志分区大小

    sys_handle->log_data_start = repair_log_start_addr; // 初始化日志起始地址
    sys_handle->log_data_end = repair_log_end_addr; // 初始化日志结束地址
    sys_handle->log_num = repair_log_num_record; // 初始化日志数量
    sys_handle->log_current_sector = repair_current_sector; // 初始化当前写入扇区为第一个扇区
    sys_handle->log_action_num = count_number;
    log_info("repair log num is :%d!", repair_log_num_record); // 输出修复日志数量
    log_info("repair log start addr is :0x%x!", repair_log_start_addr); // 输出修复日志起始地址
    log_info("repair log end addr is :0x%x!", repair_log_end_addr); // 输出修复日志结束地址
    log_info("log_action_num:%d", sys_handle->log_action_num); // 输出日志数量
    if(false == rolldb_flash_ops.write_data(SYSINFO_START_ADDR, sys_handle, sizeof(rollts_sys_t))) // 将初始化后的系统分区数据写入 Flash
    {
        return false;
    }
    task_resume();
    return true;
}
