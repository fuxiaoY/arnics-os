
#include "rtosInterface/queue/queue_port.h"
#include "dePartment/centerAdministrative/centerAdministrative.h"
#include "dePartment/centerEvent/centerEvent.h"
#include "dePartment/centerMedia/centerMedia.h"

extern const queue_ops_t queue_ops_win;
extern const queue_ops_t queue_ops_linux;
extern const queue_ops_t queue_ops_freertos;

#if PLATFORM_WIN
#define CURRENT_QUEUE_OPS (&queue_ops_win)
#elif PLATFORM_LINUX
#define CURRENT_QUEUE_OPS (&queue_ops_linux)
#elif PLATFORM_MCU
#define CURRENT_QUEUE_OPS (&queue_ops_freertos)
#else
#error "Unknown platform"
#endif

static queue_t g_queues[QUEUE_ID_COUNT] = {
#define X(queue_name, queue_len, queue_size) \
    { .name = #queue_name, .item_size = queue_size, .len = queue_len, .handle = NULL, .ops = CURRENT_QUEUE_OPS },
    QUEUE_TABLE(X)
#undef X
};

const queue_t* queue_get(queue_id_t id)
{
    if (id >= QUEUE_ID_COUNT) return NULL;
    return &g_queues[id];
}

void queue_init_all(void)
{
    for (uint8_t i = 0; i < QUEUE_ID_COUNT; i++)
    {
        queue_t* q = &g_queues[i];
        if (q->ops && q->ops->create)
        {
            q->handle = q->ops->create(q->item_size, q->len);
        }
    }
}
