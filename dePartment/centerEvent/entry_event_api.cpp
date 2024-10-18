
#include "entry_event_api.h"
#ifdef __USE_EVENTOS_
#undef eventOS_Init
#undef eventKernelStart
#else
#undef Event_Process
#endif
#undef SendMessacgeToEventos_needSample
#undef GetRspMessacgeToEventos_needSample

#include "event_api.h"
#include "event_process.h"
__attribute__((section("._entry_event_api")))
const tEVENTEntry entry_event_list =
{
    #ifdef __USE_EVENTOS_
    #undef eventOS_Init
    INIT_MEMBER(eventOS_Init),
    #undef eventKernelStart
    INIT_MEMBER(eventKernelStart),
    #else
    #undef Event_Process
    INIT_MEMBER(Event_Process),
    #endif
    #undef SendMessacgeToEventos_needSample
    INIT_MEMBER(SendMessacgeToEventos_needSample),
    #undef GetRspMessacgeToEventos_needSample
    INIT_MEMBER(GetRspMessacgeToEventos_needSample),

};
