#include "rollTs.h" 

#define MAGIC_VALID       0x20251204 // 定义一个有效的魔数，用于验证系统分区的有效性
#define MAGIC_DATA_VALID  0x20251205
uint32_t crc_simple(uint8_t *data, size_t len) 
{
    uint32_t checksum = 0x07;  
    for (size_t i = 0; i < len; i++) 
    {
        checksum += data[i];
    }
    return ~checksum;  
}

/* function-------------------------------------------------------------------*/

static void rollts_manager_print(rollts_manager_t *rollts_manager)
{
    log_debug("magic_valid          : 0x%x", rollts_manager->sys_info.magic_valid);
    log_debug("data_start_block_num : 0x%x", rollts_manager->sys_info.data_start_block_num);
    log_debug("data_end_block_num   : 0x%x", rollts_manager->sys_info.data_end_block_num);
    log_debug("log_size             : 0x%x", rollts_manager->sys_info.log_size);
    log_debug("rollts_max_size      : 0x%x", rollts_manager->sys_info.rollts_max_size);
    log_debug("single_block_size    : 0x%x", rollts_manager->sys_info.single_block_size);
    log_debug("min_write_unit_size  : 0x%x", rollts_manager->sys_info.min_write_unit_size);
    log_debug("rollts_max_block_num : 0x%x", rollts_manager->sys_info.rollts_max_block_num);
    log_debug("data_start_addr      : 0x%x", rollts_manager->sys_info.data_start_addr);
    log_debug("data_end_addr        : 0x%x", rollts_manager->sys_info.data_end_addr);
}

/**
 * @func: 检查数据库大小配置是否与最小擦除单元对齐
 *        检查最小大小是否无法完成roll
 */
static bool check_if_rollts_size_aligned(void)
{
    if(ROLLTS_MAX_SIZE % SINGLE_BLOCK_SIZE != 0)
    {
        return false;
    }
    if(ROLLTS_MAX_SIZE/SINGLE_BLOCK_SIZE < 5)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/**
 * @func: 检查系统分区是否能正常分配，
 *        并初始化系统分区信息
 */
static bool check_if_sys_aligned(rollts_manager_t *rollts_manager)
{
    bool ret = false;
    if(0 == rollts_manager->flash_ops.read_data(0, &rollts_manager->sys_info, SYSINFO_SIZE))
    {
        //读取成功后，检查magic是否有效
        if(MAGIC_VALID != rollts_manager->sys_info.magic_valid)
        {
            ret = false;
        }
        if(    rollts_manager->sys_info.data_start_block_num      != 1
            || rollts_manager->sys_info.data_end_block_num        != ROLLTS_MAX_BLOCK_NUM - 1
            || rollts_manager->sys_info.log_size                  != ROLLTS_MAX_BLOCK_NUM * SINGLE_BLOCK_SIZE
            || rollts_manager->sys_info.rollts_max_size           != ROLLTS_MAX_SIZE
            || rollts_manager->sys_info.single_block_size         != SINGLE_BLOCK_SIZE
            || rollts_manager->sys_info.min_write_unit_size       != MIN_WRITE_UNIT_SIZE
            || rollts_manager->sys_info.rollts_max_block_num      != ROLLTS_MAX_BLOCK_NUM)
        {
            ret = false;
        }
        else
        {
            rollts_manager->sys_info.data_start_addr  = rollts_manager->sys_info.data_start_block_num * rollts_manager->sys_info.single_block_size;
            rollts_manager->sys_info.data_end_addr    = (rollts_manager->sys_info.data_end_block_num)   * rollts_manager->sys_info.single_block_size;
            ret = true;
        }
    }

    return ret;
}

/**
 * @func: 初始化数据库管理单元
 */
static void rollts_manager_init(rollts_manager_t *rollts_manager)
{
    rollts_manager->sys_info.magic_valid               = MAGIC_VALID;
    // 数据分区在系统分区后1 block
    rollts_manager->sys_info.data_start_block_num      = 1;        
    rollts_manager->sys_info.data_end_block_num        = ROLLTS_MAX_BLOCK_NUM - 1;  
    rollts_manager->sys_info.log_size                  = ROLLTS_MAX_BLOCK_NUM * SINGLE_BLOCK_SIZE;
    rollts_manager->sys_info.rollts_max_size           = ROLLTS_MAX_SIZE; 
    rollts_manager->sys_info.single_block_size         = SINGLE_BLOCK_SIZE;
    rollts_manager->sys_info.min_write_unit_size       = MIN_WRITE_UNIT_SIZE;
    rollts_manager->sys_info.rollts_max_block_num      = ROLLTS_MAX_BLOCK_NUM;
    rollts_manager->sys_info.data_start_addr           = rollts_manager->sys_info.data_start_block_num * rollts_manager->sys_info.single_block_size;
    rollts_manager->sys_info.data_end_addr             = (rollts_manager->sys_info.data_end_block_num)  * rollts_manager->sys_info.single_block_size;
}

/**
 * @func: 格式化 数据区head
 */
static int head_block_force_format(rollts_manager_t *rollts_manager)
{
    log_debug("format head block...");
    block_info_t block_info;
    memset(&block_info, 0xFF, sizeof(block_info_t));
    block_info.magic_valid = MAGIC_VALID;
    block_info.data_num    = -1;
    // 清除数据分区
    uint32_t rollts_max_data_block_num = rollts_manager->sys_info.rollts_max_block_num - 1;
    for (uint32_t i = 0; i < rollts_max_data_block_num; i++) 
    { 
        if(0 != rollts_manager->flash_ops.erase_sector(rollts_manager->sys_info.data_start_addr  + \
                                                      rollts_manager->sys_info.single_block_size * i))
        {
            return -1;
        }
        else
        {
            if(1 == i)
            {
                // 写入数据分区起始地址
                SET_HEAD(block_info);
            }
            else if(2 == i)
            {
                SET_BACKUP(block_info);
            }
            else
            {
                SET_NOT_HEAD(block_info);
            }
            // 初始化当前数据块
            if(0 != rollts_manager->flash_ops.write_data(rollts_manager->sys_info.data_start_addr  + \
                                                     rollts_manager->sys_info.single_block_size * i,
                                                     &block_info, sizeof(block_info_t)))
            {
                return -1;
            }  
            else                                       
            {
                log_info(" Log Sector erase and init  at : 0x%x ", (rollts_manager->sys_info.data_start_addr  + \
                                                rollts_manager->sys_info.single_block_size * i));
            }

        }
    }
    rollts_manager->mem_tab.pre_addr         = rollts_manager->sys_info.data_start_addr;
    rollts_manager->mem_tab.head_addr        = rollts_manager->mem_tab.pre_addr + rollts_manager->sys_info.single_block_size;
    rollts_manager->mem_tab.head_backup_addr = rollts_manager->mem_tab.head_addr + rollts_manager->sys_info.single_block_size;
    return 0;
}

/**
 * @func: 初始化所有分区信息
 *        
 */
static int rollts_format(rollts_manager_t *rollts_manager)
{
    // 对物理地址进行初始化
    if(0 != head_block_force_format(rollts_manager))
    {
        log_info(" Log Sector erase failed! ");
        return -1;
    }
    // 清除系统分区
    if(0 != rollts_manager->flash_ops.erase_sector(0))
    {
        return -1;
    }
    else
    {
        if(0 != rollts_manager->flash_ops.write_data(0,&rollts_manager->sys_info, sizeof(rollts_sys_t)))
        {
            return -1;
        }  
        else                                       
        {
            log_info(" Sys Sector erase and init  at : 0x%x ", 0);
        }

    }
    
    log_info("rollts_format succeed!");                        
    return 0;
}

/* function-------------------------------------------------------------------*/
/**
 * @func: 提取上一个block
 */
static uint32_t get_pre_block(rollts_manager_t *rollts_manager, uint32_t mem_addr)
{
    // 提取上一个block的内存地址
    mem_addr = mem_addr - rollts_manager->sys_info.single_block_size;

    // 可以在这里添加对mem_addr边界的检查，防止越界访问
    if (mem_addr < rollts_manager->sys_info.data_start_addr) 
    {
        // 处理边界情况，例如回到循环缓冲区的末尾
        mem_addr = rollts_manager->sys_info.data_end_addr;
    }
    return mem_addr;
}

/**
 * @func: 提取下一个block
 */
static uint32_t get_next_block(rollts_manager_t *rollts_manager, uint32_t mem_addr)
{
    // 提取下一个block的内存地址
    mem_addr = mem_addr + rollts_manager->sys_info.single_block_size;

    // 添加对mem_addr边界的检查，防止越界访问
    if (mem_addr > rollts_manager->sys_info.data_end_addr) 
    {
        // 处理边界情况，例如回到循环缓冲区的起始位置
        mem_addr = rollts_manager->sys_info.data_start_addr;
    }
    
    return mem_addr;
}

/**
 * @func: 获取最旧的数据块
 *
 */
static uint32_t get_oldest_block(rollts_manager_t *rollts_manager)
{
    return get_next_block(rollts_manager, rollts_manager->mem_tab.head_backup_addr);
}

/**
 * @func: 格式化head block
 */
static bool head_block_format(rollts_manager_t *rollts_manager)
{
    log_debug("head_block_format start...");
    uint32_t pre_addr  = 0;
    uint32_t next_addr = 0;
    if(rollts_manager->mem_tab.head_addr == rollts_manager->sys_info.data_start_addr) //head在起始块
    {
        pre_addr  = rollts_manager->sys_info.data_end_addr;
        next_addr = rollts_manager->sys_info.data_start_addr + rollts_manager->sys_info.single_block_size;
    }
    else if(rollts_manager->mem_tab.head_addr == rollts_manager->sys_info.data_end_addr) //head在结束块
    {
        pre_addr  = rollts_manager->sys_info.data_end_addr - rollts_manager->sys_info.single_block_size;
        next_addr = rollts_manager->sys_info.data_start_addr;
    }
    else
    {
        pre_addr  = rollts_manager->mem_tab.head_addr - rollts_manager->sys_info.single_block_size;
        next_addr = rollts_manager->mem_tab.head_addr + rollts_manager->sys_info.single_block_size;
    }
    block_info_t pre_block_info;
    block_info_t next_block_info;
    memset(&pre_block_info , 0xFF, sizeof(block_info_t));
    memset(&next_block_info, 0xFF, sizeof(block_info_t));

    rollts_manager->flash_ops.read_data(pre_addr,  &pre_block_info, sizeof(block_info_t));
    rollts_manager->flash_ops.read_data(next_addr,&next_block_info, sizeof(block_info_t));
    if(!IS_NOT_HEAD(pre_block_info))
    {
        memset(&pre_block_info , 0xFF, sizeof(block_info_t));
        pre_block_info.magic_valid = MAGIC_VALID;
        pre_block_info.data_num    = -1;
        if(0 != rollts_manager->flash_ops.erase_sector(pre_addr))
        {
            return false;
        }
        if(0 != rollts_manager->flash_ops.write_data(pre_addr, &pre_block_info, sizeof(block_info_t)))
        {
            return false;
        }
    }
    if(!IS_BACKUP(next_block_info))
    {
        memset(&next_block_info , 0xFF, sizeof(block_info_t));
        pre_block_info.magic_valid = MAGIC_VALID;
        pre_block_info.data_num    = -1;
        SET_BACKUP(next_block_info);
        if(rollts_manager->flash_ops.erase_sector(next_addr))
        {
            return false;
        }
        if(rollts_manager->flash_ops.write_data(next_addr, &next_block_info, sizeof(block_info_t)))
        {
            return false;
        }
    }
    rollts_manager->mem_tab.head_backup_addr = next_addr;
    rollts_manager->mem_tab.pre_addr         = pre_addr;
    return true;
}

/**
 * @func: 扫描起始数据块
 */
static int scan_head_block(rollts_manager_t *rollts_manager)
{
    int num = 0;
    bool find_in_first                 = false;
    bool find_in_last                  = false;
    uint32_t find_head_block_addr     = 0;

    block_info_t block_info;
    uint32_t rollts_max_data_block_num = rollts_manager->sys_info.rollts_max_block_num - 1;
    for (uint32_t i = 0; i < rollts_max_data_block_num; i++) 
    { 
        block_info.magic_valid = 0;
        rollts_manager->flash_ops.read_data(rollts_manager->sys_info.data_start_addr  + \
                                            rollts_manager->sys_info.single_block_size * i,
                                            &block_info, sizeof(block_info_t));
        log_debug("----------------------------------------");                                
        log_debug("block_info.last_data_addr     : 0x%x", block_info.last_data_addr);
        log_debug("block_info.data_num           : %d", block_info.data_num);
        log_debug("block_info.magic_valid        : 0x%x", block_info.magic_valid);    
        log_debug("block_info.is_head            : %d", block_info.is_head);
        log_debug("----------------------------------------");     
        if(MAGIC_VALID == block_info.magic_valid)
        {
            // 查询是否是启动块
            if(IS_HEAD(block_info))
            {
                num++;
                rollts_manager->mem_tab.head_addr =rollts_manager->sys_info.data_start_addr  + \
                                                   rollts_manager->sys_info.single_block_size * i;
                if(i == 0)
                {
                    find_in_first = true;
                    find_head_block_addr = rollts_manager->mem_tab.head_addr;
                }                             
                if(i == rollts_max_data_block_num)
                {
                    find_in_last = true;
                }
            }
        }
        else
        {
            continue;
        }
    }
    // 启动块出现在一头一尾
    if(find_in_last && find_in_first)
    {
        rollts_manager->mem_tab.head_addr = find_head_block_addr;
    }
    return num;

}

/**
 * @func: 初始化内存表
 */
static int rollts_mem_tab_init(rollts_manager_t *rollts_manager)
{
    log_debug(" rollts_mem_tab_init start...");
    // 扫描 head块
    if(scan_head_block(rollts_manager) > 0)
    {
        log_debug("scan_head_block_addr : 0x%x",rollts_manager->mem_tab.head_addr);
        if(head_block_format(rollts_manager))
        {
            log_debug("rollts_manager->mem_tab.pre_addr        : 0x%x",rollts_manager->mem_tab.pre_addr);
            log_debug("rollts_manager->mem_tab.head_addr       : 0x%x",rollts_manager->mem_tab.head_addr);
            log_debug("rollts_manager->mem_tab.head_backup_addr: 0x%x",rollts_manager->mem_tab.head_backup_addr);
        }
        else
        {
            log_error(" head_block_format failed!");
            return -1;
        }
        return 0;
    }
    else
    {
        log_alt(" scan_head_block_addr not found!");
        if(0 == head_block_force_format(rollts_manager))
        {
            return 0;
        }
        rollts_manager->is_init = 0;
        return -1;
    }
}

/**
 * @func: 初始化数据块结构体
 * 
 */
static void data_block_loop(rollts_manager_t *rollts_manager)
{
    memset(&rollts_manager->rollts_data,0x00,sizeof(rollts_data_t));

    uint32_t start_addr  = rollts_manager->mem_tab.pre_addr + sizeof(block_info_t);
    uint32_t end_addr    = rollts_manager->mem_tab.pre_addr + rollts_manager->sys_info.single_block_size - 1;

    bool is_block_info_valid = true;
    // 首先查询block_info是否已经写入了end_addr 和 data_num
    block_info_t current_block_info;
    rollts_manager->flash_ops.read_data(rollts_manager->mem_tab.pre_addr, &current_block_info, sizeof(block_info_t));
    if((0xFFFFFFFF != current_block_info.last_data_addr)
     &&(-1         != current_block_info.data_num))
    {
        rollts_manager->current_block_full = true;
        // 当前数据块已经写入了数据
        if((0xFFFFFFFF == current_block_info.last_data_addr)
                ||(-1  == current_block_info.data_num))
        {
            // 完整性检查不通过，需要进行日志修复
            log_alt("rollts_data_block_loop: block_info integrity check failed");
            log_alt("repair...");
            is_block_info_valid = false;
        }
        else
        {
            // 完整性检查通过，进行数据块数据初始化
            rollts_manager->flash_ops.read_data(current_block_info.last_data_addr, 
                                                     &rollts_manager->rollts_data, sizeof(rollts_data_t));
            rollts_manager->cur_block_data_num = current_block_info.data_num;
            return;
        }
    }
    else
    {
        rollts_manager->current_block_full = false;
    }
    rollts_data_t tmp_rollts_data;
    memset(&tmp_rollts_data,0x00,sizeof(rollts_data_t));
    // 遍历数据块
    rollts_manager->cur_block_data_num = 0;

    rollts_manager->flash_ops.read_data(start_addr, &tmp_rollts_data, sizeof(rollts_data_t));
    if(MAGIC_DATA_VALID != tmp_rollts_data.magic_valid)
    {
        // 第一数据区为空
        log_debug(" tmp_rollts_data find first empty");
        // 从头开始写数据
        rollts_manager->rollts_data.magic_valid = MAGIC_DATA_VALID;

        rollts_manager->rollts_data.pre_addr  = 0;
        rollts_manager->rollts_data.cur_addr  = start_addr;
    }
    else
    {
        rollts_manager->rollts_data.magic_valid = MAGIC_DATA_VALID;
        rollts_manager->rollts_data.pre_addr    = tmp_rollts_data.cur_addr;
        rollts_manager->rollts_data.cur_addr    = tmp_rollts_data.next_addr;

        start_addr  = tmp_rollts_data.next_addr;
        while(start_addr <= end_addr) //写入时保证有效数据的next不超限制
        {
            rollts_manager->flash_ops.read_data(start_addr, &tmp_rollts_data, sizeof(rollts_data_t));
            
            if(MAGIC_DATA_VALID == tmp_rollts_data.magic_valid)
            {
                // 当前block数据条数增加
                rollts_manager->cur_block_data_num++;
                start_addr  = tmp_rollts_data.next_addr;
                // 保证rollts_data 始终为当前可写空位
                rollts_manager->rollts_data.magic_valid = MAGIC_DATA_VALID;
                rollts_manager->rollts_data.pre_addr    = tmp_rollts_data.cur_addr;
                rollts_manager->rollts_data.cur_addr    = tmp_rollts_data.next_addr;
            }
            else
            {
                // 当前数据头为空
                log_debug(" tmp_rollts_data find empty");
                break;
            }

        }

    }

    if(false  == is_block_info_valid)
    {
        // 修复blcok info
        // 完整性检查失败，进行数据块数据初始化
        current_block_info.last_data_addr = start_addr;
        current_block_info.data_num       = rollts_manager->cur_block_data_num;

        log_debug("writting last_data_addr... 0x%x",current_block_info.last_data_addr);
        rollts_manager->flash_ops.write_data(rollts_manager->mem_tab.pre_addr + offsetof(block_info_t, last_data_addr),
                                            &current_block_info.last_data_addr, sizeof(uint32_t));
        
        log_debug("writting data_num... %d",current_block_info.data_num);
        rollts_manager->flash_ops.write_data(rollts_manager->mem_tab.pre_addr + offsetof(block_info_t, data_num),
                                            &current_block_info.data_num, sizeof(uint32_t));                                       

    }
}

/* function-------------------------------------------------------------------*/
/**
 * @func: head日志块迁移
 */
static int head_block_move(rollts_manager_t *rollts_manager)
{
    log_debug("(pre)memtab:pre_addr        :0x%x",rollts_manager->mem_tab.pre_addr);
    log_debug("(pre)memtab:head_addr       :0x%x",rollts_manager->mem_tab.head_addr);
    log_debug("(pre)memtab:head_backup_addr:0x%x",rollts_manager->mem_tab.head_backup_addr);
    block_info_t block_info;
    memset(&block_info,0xFF,sizeof(block_info_t));
    block_info.magic_valid = MAGIC_VALID;
    block_info.data_num    = -1;
    // 1. 将head_back 置为head
    SET_HEAD(block_info);
    rollts_manager->flash_ops.write_data(rollts_manager->mem_tab.head_backup_addr
                                              ,&block_info, sizeof(block_info_t));
    // 2. 将之前head置为数据区
    SET_NOT_HEAD(block_info);
    rollts_manager->flash_ops.erase_sector(rollts_manager->mem_tab.head_addr);
    rollts_manager->flash_ops.write_data(rollts_manager->mem_tab.head_addr
                                              ,&block_info, sizeof(block_info_t));                               
    // 3.更新rollts_manager->mem_tab
    log_debug("mem_tab fresh...");
    uint32_t pre_addr  = 0;
    uint32_t cur_addr  = 0;
    uint32_t next_addr = 0;
    pre_addr           = rollts_manager->mem_tab.head_addr;
    cur_addr           = rollts_manager->mem_tab.head_backup_addr;
    next_addr          = rollts_manager->mem_tab.head_backup_addr + rollts_manager->sys_info.single_block_size;

    if(next_addr > rollts_manager->sys_info.data_end_addr)
    {
        next_addr = rollts_manager->sys_info.data_start_addr;
    }
    
    rollts_manager->mem_tab.pre_addr         = pre_addr;
    rollts_manager->mem_tab.head_addr        = cur_addr;
    rollts_manager->mem_tab.head_backup_addr = next_addr;

    // 4.将之前head_back后1 block置为head_back
    SET_BACKUP(block_info);
    rollts_manager->flash_ops.erase_sector(rollts_manager->mem_tab.head_backup_addr);
    rollts_manager->flash_ops.write_data(rollts_manager->mem_tab.head_backup_addr 
                                              ,&block_info, sizeof(block_info_t)); 
    rollts_manager->current_block_full = false;
    // 打印 memtab信息
    log_debug("memtab:pre_addr        :0x%x",rollts_manager->mem_tab.pre_addr);
    log_debug("memtab:head_addr       :0x%x",rollts_manager->mem_tab.head_addr);
    log_debug("memtab:head_backup_addr:0x%x",rollts_manager->mem_tab.head_backup_addr);

    // 5.从头开始写块数据
    rollts_manager->rollts_data.magic_valid = MAGIC_DATA_VALID;
    rollts_manager->rollts_data.pre_addr  = 0;
    rollts_manager->rollts_data.cur_addr  = rollts_manager->mem_tab.pre_addr + sizeof(block_info_t);

    // test
    // {

    //     block_info_t block_info_test;  

    //     memset(&block_info_test,0x00,sizeof(block_info_t));     
    //     rollts_manager->flash_ops.read_data(rollts_manager->mem_tab.pre_addr
    //                                             ,&block_info_test, sizeof(block_info_t));
    //     log_debug("pre block_info_test:is_head         :0x%x",block_info_test.is_head);
    //     log_debug("pre block_info_test:head_addr       :0x%x",block_info_test.magic_valid);                                       

    //     memset(&block_info_test,0x00,sizeof(block_info_t));                                        
    //     rollts_manager->flash_ops.read_data(rollts_manager->mem_tab.head_addr
    //                                             ,&block_info_test, sizeof(block_info_t));
    //     log_debug("head block_info_test:is_head         :0x%x",block_info_test.is_head);
    //     log_debug("head block_info_test:head_addr       :0x%x",block_info_test.magic_valid);

    //     memset(&block_info_test,0x00,sizeof(block_info_t));                                        
    //     rollts_manager->flash_ops.read_data(rollts_manager->mem_tab.head_backup_addr
    //                                             ,&block_info_test, sizeof(block_info_t));   
    //     log_debug("head back block_info_test:is_head         :0x%x",block_info_test.is_head);
    //     log_debug("head back block_info_test:head_addr       :0x%x",block_info_test.magic_valid);
    // }
    return 0;
}

/**
 * @func: 查找最后一个日志位置,并计算是否需要切换日志块
 */
static bool find_the_last_position_and_calc(rollts_manager_t *rollts_manager,
                                                uint32_t    frame_len,
                                                uint32_t  payload_len)
{
    // 当前块是否已经封顶
    if(rollts_manager->current_block_full)
    {
        log_debug("current block is full,need to move to next block");
        return false;
    }

    // 不可超过当前block最后一位
    if((rollts_manager->rollts_data.cur_addr % rollts_manager->sys_info.single_block_size + frame_len \
         < rollts_manager->sys_info.single_block_size ))
    {
        // 不需要切换日志块
        rollts_manager->rollts_data.next_addr     = rollts_manager->rollts_data.cur_addr + frame_len;
        rollts_manager->rollts_data.payload_len   = payload_len;
        // 当前block数据条数增加
        rollts_manager->cur_block_data_num++;

        log_debug("rollts_data.magic_valid:0x%x",rollts_manager->rollts_data.magic_valid);
        log_debug("rollts_data.pre_addr   :0x%x",rollts_manager->rollts_data.pre_addr);
        log_debug("rollts_data.cur_addr   :0x%x",rollts_manager->rollts_data.cur_addr);
        log_debug("rollts_data.next_addr  :0x%x",rollts_manager->rollts_data.next_addr);
        log_debug("rollts_data.payload_len:%d"  ,rollts_manager->rollts_data.payload_len);
        return true;
    }
    else
    {
        //需要切换日志块
        log_debug("no space in current block,need switch to next block");

        // 进行当前block封顶
        rollts_manager->last_valid_data_addr = rollts_manager->rollts_data.pre_addr; // 更新最后有效数据块
        //空间不足时,对pre block记录最后数据地址 
        uint32_t last_data_addr = 0x0a; //随机值观察是否被覆写
        rollts_manager->flash_ops.read_data(rollts_manager->mem_tab.pre_addr + offsetof(block_info_t, last_data_addr),
                                               &last_data_addr, sizeof(uint32_t));
        //空间不足时，对pre block记录日志数量   
        int32_t data_num = 0x06; //随机值观察是否被覆写
        rollts_manager->flash_ops.read_data(rollts_manager->mem_tab.pre_addr + offsetof(block_info_t, data_num),
                                               &data_num, sizeof(int32_t));  

        if(0xFFFFFFFF == last_data_addr) //无数据
        {
            log_debug("writting last_data_addr... 0x%x",rollts_manager->last_valid_data_addr);
            rollts_manager->flash_ops.write_data(rollts_manager->mem_tab.pre_addr + offsetof(block_info_t, last_data_addr),
                                                &rollts_manager->last_valid_data_addr, sizeof(uint32_t));
        }
        else
        {
            log_alt("last_data_addr is not 0xFFFFFFFF,you need to check it");
        }
        if(-1 == data_num)  
        {
            log_debug("writting data_num... %d",rollts_manager->cur_block_data_num);
            rollts_manager->flash_ops.write_data(rollts_manager->mem_tab.pre_addr + offsetof(block_info_t, data_num),
                                                &rollts_manager->cur_block_data_num, sizeof(uint32_t));                                       
        }  
        else
        {
            log_alt("data_num is not -1,you need to check it");
        }
        // test
        // { 
        //     block_info_t pre_block_info;
        //     memset(&pre_block_info , 0xFF, sizeof(block_info_t));
        //     rollts_manager->flash_ops.read_data(rollts_manager->mem_tab.pre_addr,  &pre_block_info, sizeof(block_info_t));
        //     log_debug("writting pre_block_info:last_data_addr :0x%x",pre_block_info.last_data_addr);
        //     log_debug("writting pre_block_info:data_num       :0x%x",pre_block_info.data_num);
        // }
        return false;
    }
}
/* function-------------------------------------------------------------------*/
/**
 * @func: 添加数据
 */
bool rollts_add(rollts_manager_t *rollts_manager, uint8_t *data, uint32_t payload_len)
{
    if(MAGIC_VALID != rollts_manager->is_init)
    {
        return false;
    }
#ifdef RTOS_MUTEX_ENABLE
    rollts_manager->flash_ops.mutex_lock();
#endif
    // 数据帧总长计算
    uint32_t data_frame_len = sizeof(rollts_data_t) + payload_len;
    // 检查数据长度是否超过单个块数据上限大小 冗余4字节
    if(data_frame_len >= rollts_manager->sys_info.single_block_size - sizeof(block_info_t) - 4)
    {
        log_alt(" rollts_add: (data_frame_len :%d, you need to split data less than %d",
                    data_frame_len,rollts_manager->sys_info.single_block_size - sizeof(block_info_t) - 4);
#ifdef RTOS_MUTEX_ENABLE
        rollts_manager->flash_ops.mutex_unlock();
#endif
        return false;
    }
    // 查找当前最后日志位置
    if(false == find_the_last_position_and_calc(rollts_manager,data_frame_len,payload_len))
    {
        // 空间不足，切换日志块
        head_block_move(rollts_manager);
        // 切换完成 rollts_data已置为首个位置

        rollts_manager->last_valid_data_addr = rollts_manager->rollts_data.cur_addr; // 更新最后有效数据块
        rollts_manager->cur_block_data_num = 0;
        if(false == find_the_last_position_and_calc(rollts_manager,data_frame_len,payload_len))
        {
            log_error(" rollts_add: something wrong when find_the_last_position_and_calc");
#ifdef RTOS_MUTEX_ENABLE
            rollts_manager->flash_ops.mutex_unlock();
#endif
            return false;
        }
    }

    // 空间足够写入当前数据
    // WAL机制
    // 1.写入magic + offset + 数据len
    rollts_manager->flash_ops.write_data(rollts_manager->rollts_data.cur_addr,&rollts_manager->rollts_data,sizeof(rollts_data_t));
    // 2.写入数据
    rollts_manager->flash_ops.write_data(rollts_manager->rollts_data.cur_addr + sizeof(rollts_data_t),data,payload_len);
    // test
    // {
    //     uint8_t data_test[110] = {0};
    //     memset(data_test,0x00,sizeof(data));
    //     rollts_manager->flash_ops.read_data(rollts_manager->rollts_data.cur_addr + sizeof(rollts_data_t), 
    //                                         &data_test, 100);
    // }
    // 3.更新数据信息
    rollts_manager->rollts_data.pre_addr = rollts_manager->rollts_data.cur_addr;
    rollts_manager->rollts_data.cur_addr = rollts_manager->rollts_data.next_addr;
#ifdef RTOS_MUTEX_ENABLE
    rollts_manager->flash_ops.mutex_unlock();
#endif
    return true;
}


/**
 * @func: 获取总日志条数
 *
 */
int32_t rollts_get_total_record_number(rollts_manager_t *rollts_manager)
{
    if (MAGIC_VALID != rollts_manager->is_init) 
    {
        return -1;
    }
#ifdef RTOS_MUTEX_ENABLE
    rollts_manager->flash_ops.mutex_lock();
#endif
    int32_t total = 0;

    /* 当前写入 block(pre_addr)始终参与计数 */
    total += rollts_manager->cur_block_data_num;

    /* 满的 block：从 pre 的上一个 block 逆向遍历到 oldest_block 的上一个 */
    uint32_t cur_block = get_pre_block(rollts_manager, rollts_manager->mem_tab.pre_addr); /* 刚满的那个 block */

    while (cur_block != get_oldest_block(rollts_manager)) 
    {
        int32_t num = -1;
        rollts_manager->flash_ops.read_data(cur_block + offsetof(block_info_t, data_num),
                                            &num, sizeof(num));

        if (num >= 0)
        {
            total += num;
        }
        cur_block = get_pre_block(rollts_manager, cur_block);
    }
#ifdef RTOS_MUTEX_ENABLE
    rollts_manager->flash_ops.mutex_unlock();
#endif
    return total;
}

/**
 * @func:整体读取所有日志
 */
bool rollts_get_all(rollts_manager_t *rollts_manager, uint8_t *data, uint32_t max_payload_len,rollTscb cb)
{
    if (MAGIC_VALID != rollts_manager->is_init) 
    {
        return false;
    }
#ifdef RTOS_MUTEX_ENABLE
    rollts_manager->flash_ops.mutex_lock();
#endif
    rollts_data_t tmp;
    /* 最旧 block = head_backup 的下一个 */
    uint32_t current_block_addr = get_oldest_block(rollts_manager);  

    /* 循环直到遇到 head */
    while (current_block_addr != rollts_manager->mem_tab.head_addr) 
    {
        uint32_t data_addr = current_block_addr + sizeof(block_info_t);

        /* 正向遍历当前 block 的链表 */
        while (data_addr + sizeof(rollts_data_t) <= current_block_addr + rollts_manager->sys_info.single_block_size) 
        {
            rollts_manager->flash_ops.read_data(data_addr, &tmp, sizeof(rollts_data_t));

            if (tmp.magic_valid != MAGIC_DATA_VALID) 
            {
                break;   /* 本 block 数据结束 */
            }

            uint32_t copy_len = (tmp.payload_len <= max_payload_len) ? tmp.payload_len : max_payload_len;

            rollts_manager->flash_ops.read_data(data_addr + sizeof(rollts_data_t),
                                                data, copy_len);
            cb(data,copy_len);
            // test
            // data[copy_len - 1] = '\0';
            // log_info("%s", (char*)data);

            data_addr = tmp.next_addr;
        }

        /* 下一个 block */
        current_block_addr = get_next_block(rollts_manager, current_block_addr);
    }
#ifdef RTOS_MUTEX_ENABLE
    rollts_manager->flash_ops.mutex_unlock();
#endif
    return true;
}

/**
 * @func:选择性读取从旧到新编号，1=最旧，total=最新
 */
bool rollts_read_pick(rollts_manager_t *rollts_manager,
                      uint32_t start_num, 
                      uint32_t end_num,
                      uint8_t *data, 
                      uint32_t max_payload_len,
                      rollTscb cb)
{
    if (MAGIC_VALID != rollts_manager->is_init || start_num == 0 || start_num > end_num) 
    {
        return false;
    }
#ifdef RTOS_MUTEX_ENABLE
    rollts_manager->flash_ops.mutex_lock();
#endif
    rollts_data_t tmp;
    uint32_t block_addr = get_oldest_block(rollts_manager);
    uint32_t current_number = 0;
    bool found_any = false;

    while (block_addr != rollts_manager->mem_tab.head_addr) 
    {
        uint32_t data_addr = block_addr + sizeof(block_info_t);

        /* 内层遍历当前 block 的链表 */
        while (data_addr + sizeof(rollts_data_t) <= block_addr + rollts_manager->sys_info.single_block_size) 
        {
            rollts_manager->flash_ops.read_data(data_addr, &tmp, sizeof(rollts_data_t));

            if (tmp.magic_valid != MAGIC_DATA_VALID) 
            {
                break;  /* 本 block 数据结束，跳出内层循环，继续下一个 block */
            }

            current_number++;

            if (current_number >= start_num && current_number <= end_num) 
            {
                uint32_t copy_len = (tmp.payload_len <= max_payload_len) ? tmp.payload_len : max_payload_len;
                rollts_manager->flash_ops.read_data(data_addr + sizeof(rollts_data_t), data, copy_len);

                found_any = true;
                cb(data,copy_len);
                // test
                // data[copy_len - 1] = '\0';
                // log_info("[PICK %lu] %s", current_number, (char*)data);
            }

            /* 如果已经超过所需范围，可以提前结束整个遍历(优化) */
            if (current_number >= end_num) 
            {
#ifdef RTOS_MUTEX_ENABLE
                rollts_manager->flash_ops.mutex_unlock();
#endif
                return true;
            }

            data_addr = tmp.next_addr;
        }

        /* 跳到下一个 block(顺时针) */
        block_addr = get_next_block(rollts_manager, block_addr);
    }
#ifdef RTOS_MUTEX_ENABLE
    rollts_manager->flash_ops.mutex_unlock();
#endif
    return found_any;
}

/**
 * @brief 日志使用容量 百分比
 */
uint8_t rollts_capacity(rollts_manager_t *rollts_manager)
{

    if (MAGIC_VALID != rollts_manager->is_init) 
    {
        return false;
    }
#ifdef RTOS_MUTEX_ENABLE
    rollts_manager->flash_ops.mutex_lock();
#endif
    uint8_t percent = 0;
    uint32_t used_sectors = 0;
    uint32_t block_addr = get_oldest_block(rollts_manager);

    while (block_addr != rollts_manager->mem_tab.head_addr) 
    {
        if (block_addr == rollts_manager->mem_tab.pre_addr) 
        {
            if (rollts_manager->cur_block_data_num > 0) 
            {
                used_sectors++;
            }

        } 
        else 
        {
            int32_t num = 0;
            rollts_manager->flash_ops.read_data(block_addr + offsetof(block_info_t, data_num),
                                                &num, sizeof(num));
            if (num > 0) 
            {
                used_sectors++;
            }
        }
        block_addr = get_next_block(rollts_manager, block_addr);
    }

    uint32_t total = rollts_manager->sys_info.rollts_max_block_num - 3;
    percent = total ? (used_sectors * 100 + total / 2) / total : 0;  // 四舍五入算法
#ifdef RTOS_MUTEX_ENABLE
    rollts_manager->flash_ops.mutex_unlock();
#endif
    return (100 - percent);
}


uint32_t rollts_capacity_size(rollts_manager_t *rollts_manager)
{
    return (rollts_manager->sys_info.rollts_max_block_num * rollts_manager->sys_info.single_block_size - 1) /1024;
}

/**
 * @func: 初始化 rollts
 */
int rollts_init(rollts_manager_t *rollts_manager)
{
#ifdef RTOS_MUTEX_ENABLE
    rollts_manager->flash_ops.mutex_lock();
#endif
    int ret = -1;
    if(check_if_rollts_size_aligned())
    {
        if(check_if_sys_aligned(rollts_manager))
        {
            ret = 0;
        }
        else
        {
            rollts_manager_print(rollts_manager);
            //重新初始化数据库
            log_info(" rollTs invalid! reinit...");
            rollts_manager_init(rollts_manager);
            ret = rollts_format(rollts_manager);
        }
    }
    //打印 rollts_manager信息
    rollts_manager_print(rollts_manager);
    if(0 == rollts_mem_tab_init(rollts_manager))
    {
        rollts_manager->is_init = MAGIC_VALID;
    }
    memset(&rollts_manager->rollts_data,0,sizeof(rollts_data_t));
    //初始化当前块数据数量
    data_block_loop(rollts_manager);
#ifdef RTOS_MUTEX_ENABLE
    rollts_manager->flash_ops.mutex_unlock();
#endif
    return ret;
}

/**
 * @func: 清除所有日志数据
 */
bool rollts_clear(rollts_manager_t *rollts_manager)
{
#ifdef RTOS_MUTEX_ENABLE
    rollts_manager->flash_ops.mutex_lock();
#endif
    int ret = -1;
    rollts_manager_print(rollts_manager);
    //重新初始化数据库
    log_debug(" rollTs invalid! reinit...");
    rollts_manager_init(rollts_manager);
    ret = rollts_format(rollts_manager);
    //打印 rollts_manager信息
    rollts_manager_print(rollts_manager);
    if(0 == rollts_mem_tab_init(rollts_manager))
    {
        rollts_manager->is_init = MAGIC_VALID;
    }
    memset(&rollts_manager->rollts_data,0,sizeof(rollts_data_t));
    //初始化当前块数据数量
    data_block_loop(rollts_manager);
#ifdef RTOS_MUTEX_ENABLE
    rollts_manager->flash_ops.mutex_unlock();
#endif
    return ret;
}
