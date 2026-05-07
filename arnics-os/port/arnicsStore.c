#include "Inc/projDefine.h"     
#include "Inc/typedef.h"         
// ---------------------------------------------------------
// [MCU 环境实现]: 使用 FAL 库
// ---------------------------------------------------------
#if PLATFORM_MCU
#include "thirdParty/fal/inc/fal.h"

bool internal_plat_erase(const char *part_name, size_t offset, size_t len) 
{
    const struct fal_partition* p = fal_partition_find(part_name);
    if(!p) 
    {
        return false;
    }
    return (fal_partition_erase(p, offset, len) >= 0);
}

bool internal_plat_write(const char *part_name, size_t offset, const uint8_t *buf, size_t len) 
{
    const struct fal_partition* p = fal_partition_find(part_name);
    if(!p) 
    {
        return false;
    }
    return (fal_partition_write(p, offset, buf, len) >= 0);
}

bool internal_plat_read(const char *part_name, size_t offset, uint8_t *buf, size_t len) 
{
    const struct fal_partition* p = fal_partition_find(part_name);
    if(!p) 
    {
        return false;
    }
    return (fal_partition_read(p, offset, buf, len) >= 0);
}

// ---------------------------------------------------------
// [lINUX & WIN实现]: 使用标准 C 文件 I/O 模拟 Flash
// ---------------------------------------------------------
#elif (PLATFORM_LINUX || PLATFORM_WIN)
#include <stdio.h>
#include <string.h>

// 辅助函数：根据 part_name 生成本地文件名 (例如 "arnicsPara" -> "./arnicsPara.bin")
static void get_sim_filename(const char* part_name, char* filename_out, size_t max_len) 
{
    snprintf(filename_out, max_len, "./%s.bin", part_name);
}

// 模拟擦除：在文件中填入 0xFF
bool internal_plat_erase(const char *part_name, size_t offset, size_t len) 
{
    char filename[64];
    get_sim_filename(part_name, filename, sizeof(filename));
    
    // 使用 "r+b" 打开，如果文件不存在则用 "w+b" 创建
    FILE *fp = fopen(filename, "r+b");
    if (!fp) 
    {
        fp = fopen(filename, "w+b");
        if (!fp) 
        {
            return false;
        }
    }
    
    fseek(fp, offset, SEEK_SET);
    uint8_t ff = 0xFF;
    for(size_t i = 0; i < len; i++) 
    {
        fwrite(&ff, 1, 1, fp);
    }
    fclose(fp);
    return true;
}

bool internal_plat_write(const char *part_name, size_t offset, const uint8_t *buf, size_t len) 
{
    char filename[64];
    get_sim_filename(part_name, filename, sizeof(filename));
    
    FILE *fp = fopen(filename, "r+b");
    if (!fp) 
    {
        fp = fopen(filename, "w+b");
        if (!fp) 
        {
            return false;
        }
    }
    
    fseek(fp, offset, SEEK_SET);
    size_t written = fwrite(buf, 1, len, fp);
    fclose(fp);
    return (written == len);
}

bool internal_plat_read(const char *part_name, size_t offset, uint8_t *buf, size_t len) 
{
    char filename[64];
    get_sim_filename(part_name, filename, sizeof(filename));
    
    FILE *fp = fopen(filename, "rb");
    if (!fp) 
    {
        // 文件不存在时，模拟 Flash 未初始化的状态 (全 0xFF)
        memset(buf, 0xFF, len);
        return true; 
    }
    
    fseek(fp, offset, SEEK_SET);
    size_t read_bytes = fread(buf, 1, len, fp);
    fclose(fp);
    
    // 如果读取的长度不足，剩余部分填充 0xFF
    if (read_bytes < len) 
    {
        memset(buf + read_bytes, 0xFF, len - read_bytes);
    }
    return true;
}
#endif
