#include "event_center_test_support.h"
#include "rtosInterface/rtosInterface.h"
#include "dePartment/centerEvent/eventProcess/eventCore.h"
#include "rtosInterface/queue/queue_port.h"

void compat_init_mutexes(void);
void onWaittingOutMessage(void);
void eventAction(void);
void onResetState(void);

void event_test_runtime_init(void)
{
    static bool initialized = false;
    if (!initialized)
    {
        queue_init_all();
        compat_init_mutexes();
        initialized = true;
    }
    event_init();
    event_test_drain_queues();
    event_test_employees_reset();
}

void event_test_process_one(void)
{
    onWaittingOutMessage();
    eventAction();
    onResetState();
}

void event_test_drain_queues(void)
{
    message_t message;
    while (rtosEventosGetMsg(&message, 0u)) {}
    while (rtosTakeMsgFromEventos(&message, 0u)) {}
}
