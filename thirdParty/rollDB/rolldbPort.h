#ifndef ROLLDBPORT_H
#define ROLLDBPORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "core/rollTs.h"



/**
 * @brief 数据库初始化
 * @note 
 */
extern void ts_init(void);

/**
 * @brief 数据库注销
 * @note 注销数据库，擦除系统分区
 */
extern void ts_deinit(void);

/**
 * @brief 日志清除
 * @note 清除所有日志数据，包括系统分区数据（调用系统分区无效初始化） 
 *     
 */
extern void ts_clear(void);

/**
 * @brief 日志追加
 * @note 追加一条日志数据 
 *     
 */
extern void ts_record(uint8_t *data, uint32_t len);

/**
 * @brief 日志整体读取
 * 
 */
extern void ts_read_all(void);

/**
 * @brief 日志选择读取
 * 
 */
extern void ts_read_pick(uint32_t start, uint32_t end);


/**
 * @brief 日志数量读取
 * 
 */
extern uint32_t ts_read_num(void);

/**
 * @brief 当前剩余容量 
 * @return 返回百分比
 */
extern uint32_t ts_read_capacity(void);
/**
 * @brief 当前剩余容量大小
 * @return 返回字节数
 */
extern uint32_t ts_read_capacity_size(void);
/**
 * @brief 数据库修复
 * @note 修复数据库，擦除系统分区
 */
extern void ts_repair(void);
/**
 * @brief 系统信息读取
 */
void ts_read_sysinfo(void);
// 单元测试用例
extern void rolldbSetUp(void);
extern void rolldbTearDown(void) ;
extern void testRollDB(void) ;


#ifdef __cplusplus
}
#endif


#endif // ROLLDBPORT_H

