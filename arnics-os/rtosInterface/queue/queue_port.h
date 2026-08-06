
#ifndef QUEUE_PORT_H
#define QUEUE_PORT_H

#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#include "rtosInterface/queue/queue_defs.h"

typedef struct queue_ops_t queue_ops_t;

typedef struct {
    const char* name;
    uint32_t item_size;
    uint32_t len;
    void* handle;
    const queue_ops_t* ops;
} queue_t;

struct queue_ops_t {
    void* (*create)(uint32_t item_size, uint32_t len);
    bool  (*send)(void* q, const void* item, uint32_t delay);
    bool  (*recv)(void* q, void* out_item, uint32_t delay);
    bool  (*peek)(void* q, void* out_item);
    uint32_t (*count)(void* q);
};

const queue_t* queue_get(queue_id_t id);
void queue_init_all(void);
void compat_init_mutexes(void);

#endif
