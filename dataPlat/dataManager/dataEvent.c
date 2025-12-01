#include "dataEvent.h"



bool evt_state_get(event_status_t* event, EVT_STA state)
{
    return boolof(event->status & state);
}


void evt_state_set(event_status_t* event,EVT_STA state)
{
    event->status |= state;
}

void evt_state_clear(event_status_t* event,EVT_STA state)
{
    event->status &= ~state;
}

void evt_state_init(event_status_t* event)
{
    event->status = 0;
}
