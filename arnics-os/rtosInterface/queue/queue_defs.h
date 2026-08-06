
#ifndef QUEUE_DEFS_H
#define QUEUE_DEFS_H

#include "Inc/projDefine.h"
#include "Inc/typedef.h"

#define QUEUE_TABLE(X) \
    X(AD_REQ,      1, sizeof(adMessage_t)) \
    X(AD_RSP,      1, sizeof(adMessage_t)) \
    X(EVENTOS_REQ, 3, sizeof(message_t)) \
    X(EVENTOS_RSP, 3, sizeof(message_t)) \
    X(MEDIA_REQ,   3, sizeof(mediaMessage_t)) \
    X(MEDIA_RSP,   3, sizeof(mediaMessage_t))

typedef enum {
#define X(name, len, size) QUEUE_ID_##name,
    QUEUE_TABLE(X)
#undef X
    QUEUE_ID_COUNT
} queue_id_t;

#endif
