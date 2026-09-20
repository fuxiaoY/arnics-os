/**
 * @file    queue_defs.h
 * @brief   队列清单与队列 ID 定义。
 *
 * @copyright (c) 2026 arnics-os. Licensed under the project LICENSE.
 */

#ifndef QUEUE_DEFS_H
#define QUEUE_DEFS_H

#include "Inc/projDefine.h"
#include "Inc/typedef.h"

/* 消息类型由使用方包含；X-Macro 仅定义，sizeof 在展开点求值。 */

/**
 * @brief 全部队列清单(X-Macro)。
 * @param X    展开器宏
 * @param name 队列标识，展开为 QUEUE_ID_##name
 * @param len  队列深度
 * @param size 单个元素大小
 */
#define QUEUE_TABLE(X) \
    X(AD_REQ,      1, sizeof(adMessage_t)) \
    X(AD_RSP,      1, sizeof(adMessage_t)) \
    X(EVENTOS_REQ, 3, sizeof(message_t)) \
    X(EVENTOS_RSP, 3, sizeof(message_t)) \
    X(MEDIA_REQ,   3, sizeof(mediaMessage_t)) \
    X(MEDIA_RSP,   3, sizeof(mediaMessage_t))

/**
 * @brief 队列 ID 枚举，由 QUEUE_TABLE 展开生成。
 */
typedef enum
{
#define X(name, len, size) QUEUE_ID_##name,
    QUEUE_TABLE(X)
#undef X
    QUEUE_ID_COUNT       // 队列总数哨兵
} queue_id_t;

#endif /* QUEUE_DEFS_H */
