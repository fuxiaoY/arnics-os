
#include "entry_event_api.h"

#undef event_process
#undef SendEventFlagToEventCenter
#undef GetResponseMessageFromEventCenter
#undef set_event_flag

#include "eventProcess/eventMessage.h"
__attribute__((section("._entry_event_api")))
const tEVENTEntry entry_event_list =
{
    #undef event_process
    INIT_MEMBER(event_process),
    #undef SendEventFlagToEventCenter
    INIT_MEMBER(SendEventFlagToEventCenter),
    #undef GetResponseMessageFromEventCenter
    INIT_MEMBER(GetResponseMessageFromEventCenter),
    #undef set_event_flag
    INIT_MEMBER(set_event_flag),
};
