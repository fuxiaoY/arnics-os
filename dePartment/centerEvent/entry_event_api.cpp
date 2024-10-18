
#include "entry_event_api.h"

#undef Event_Process
#undef SendMessacgeToEventos_needSample
#undef GetRspMessacgeToEventos_needSample

#include "eventProcess/eventCore.h"
__attribute__((section("._entry_event_api")))
const tEVENTEntry entry_event_list =
{

    #undef Event_Process
    INIT_MEMBER(Event_Process),
    #undef SendMessacgeToEventos_needSample
    //INIT_MEMBER(SendMessacgeToEventos_needSample),
    #undef GetRspMessacgeToEventos_needSample
    //INIT_MEMBER(GetRspMessacgeToEventos_needSample),

};
