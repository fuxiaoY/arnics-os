/**
 * @file    queue_port.h
 * @brief   队列描述符、后端操作集接口与初始化声明。
 *
 * @copyright (c) 2026 arnics-os. Licensed under the project LICENSE.
 */

#ifndef QUEUE_PORT_H
#define QUEUE_PORT_H

#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#include "rtosInterface/queue/queue_defs.h"

/** @brief 队列后端操作集类型。 */
typedef struct queue_ops_t queue_ops_t;

/** @brief 队列描述符。 */
typedef struct
{
    const char*       name;       // 队列名
    uint32_t          item_size;  // 元素大小(字节)
    uint32_t          len;        // 队列深度
    void*             handle;     // 平台队列句柄，初始化时回填
    const queue_ops_t* ops;       // 后端操作函数表
} queue_t;

/** @brief 队列后端操作集，每个平台提供一份 const 实例。 */
struct queue_ops_t
{
    void*  (*create)(uint32_t item_size, uint32_t len);            // 创建队列，返回句柄
    bool   (*send)(void* q, const void* item, uint32_t delay);     // 入队
    bool   (*recv)(void* q, void* out_item, uint32_t delay);       // 出队
    bool   (*peek)(void* q, void* out);                            // 窥探队首不移除
    uint32_t (*count)(void* q);                                    // 当前元素数
};

/**
 * @brief 按 ID 获取队列描述符。
 * @param id 队列 ID
 * @return 描述符指针；id 越界返回 NULL
 */
const queue_t* queue_get(queue_id_t id);

/**
 * @brief 创建 QUEUE_TABLE 中登记的全部队列，启动时调用一次。
 */
void queue_init_all(void);

/**
 * @brief 初始化跨部门互斥量，启动时调用一次。
 */
void compat_init_mutexes(void);

#endif /* QUEUE_PORT_H */
