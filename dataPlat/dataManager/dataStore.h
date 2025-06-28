
#ifndef _DATA_STORE_H_
#define _DATA_STORE_H_
#include "Inc/projDefine.h"
#include "Inc/typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif


/* define -----------------------------------------------------------*/
#define     DATA_ADDR(struct,x)      (offsetof(struct, x))

#define STRUCT_WITH_CRC_NAME(var) arnicsDataPlat##var##_t
/**
 * @brief 单一空间数据结构定义
 *         single space data structure definition
 */
#define STRUCT_WITH_CRC(type,var) \
typedef struct \
{ \
    uint16_t crc16; \
    type var; \
} STRUCT_WITH_CRC_NAME(var)
/**
 * @brief 连续空间数据结构定义
 *         continuous space data structure definition
 */
#define SOTRE_SUPPORT(var) SMALLOC(STRUCT_WITH_CRC_NAME(var),var##_store, sizeof(STRUCT_WITH_CRC_NAME(var)))
/**
 * @brief 数据操作赋值
 * 
 */
#define operation_set(op,structs,var) \
    do{ \
        (op).data_struct_ptr = (void*)&(structs->var##_store); \
        (op).data_len = sizeof(var); \
        (op).data_offset = offsetof(STRUCT_WITH_CRC_NAME(var), var);  \
        (op).crc_offset = offsetof(STRUCT_WITH_CRC_NAME(var), crc16); \
    }while(0)
/**
 * @brief 数据函数定义宏
 *        data function definition macro
 */
#define DEFINE_DATA_ACTION_FUNC(type_t,param) \
static bool param##_data_action(type_t *p_arnics_para_store, dataAction_e action) { \
    store_operation_t op; \
    operation_set(op, p_arnics_para_store, param); \
    if (DATA_PACK == action) { \
        return value_pack_with_crc16(&op, (uint8_t*)&param, sizeof(param)); \
    } else if (DATA_LOAD == action) { \
        return value_load_with_crc16(&op, (uint8_t*)&param, sizeof(param)); \
    } \
    return false; \
}

/**
 * @brief 数据函数操作宏
 *        data function operation macro
 */
#define  DATA_ACTION_FUNC(p_store,param,action) \
param##_data_action(p_store,action)

/* typedef -----------------------------------------------------------*/
typedef enum
{
    DATA_PACK,
    DATA_LOAD,
}dataAction_e;

typedef struct 
{
    void*  data_struct_ptr;        // 数据指针
    size_t data_len;        // 数据大小
    uint16_t data_offset;    // 数据偏移量
    uint16_t crc_offset;    // crc字段偏移量
} store_operation_t;
/**
 * @brief 数据打包
 * 
 * @param op 数据操作结构体
 * @param src_buf 数据源指针
 * @param src_len 数据源大小
 * @attention 注意源地址数据结构和目的地址数据结构的匹配性
 */
extern bool value_pack_with_crc16(const store_operation_t* op,uint8_t* src_buf,size_t src_len);
/**
 * @brief 数据装载
 * 
 * @param op 数据操作结构体
 * @param dest_buf 数据目的指针
 * @param dest_len 数据目的大小
 * @attention 注意源地址数据结构和目的地址数据结构的匹配性
 */
extern bool value_load_with_crc16(const store_operation_t* op,uint8_t* dest_buf,size_t dest_len);
/**
 * @brief 数据保存到指定分区
 * 
 * @param part_name 分区名称
 * @param offset 偏移量
 * @param buf 数据缓冲区
 * @param len 数据长度
 */
extern bool data_save(const char *part_name,size_t offset,uint8_t *buf ,size_t len);
/**
 * @brief 从指定分区加载数据
 * 
 * @param part_name 分区名称
 * @param offset 偏移量
 * @param buf 数据缓冲区
 * @param len 数据长度
 */
extern bool data_load(const char *part_name,size_t offset,uint8_t *buf ,size_t len);
#ifdef __cplusplus
}
#endif

#endif
