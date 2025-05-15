
#include "entry_event_api.h"

#undef event_process
#undef SendEventCallToEventCenter
#undef GetResponseMessageFromEventCenter
#undef add_event_flag
#undef set_event_flag

#include "eventProcess/eventCore.h"
__attribute__((section("._entry_event_api")))
const tEVENTEntry entry_event_list =
{
    #undef event_process
    INIT_MEMBER(event_process),
    #undef SendEventCallToEventCenter
    INIT_MEMBER(SendEventCallToEventCenter),
    #undef GetResponseMessageFromEventCenter
    INIT_MEMBER(GetResponseMessageFromEventCenter),
    #undef add_event_flag
    INIT_MEMBER(add_event_flag),
    #undef set_event_flag
    INIT_MEMBER(set_event_flag),
};
