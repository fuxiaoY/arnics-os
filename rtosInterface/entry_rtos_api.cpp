#include "entry_rtos_api.h"
#undef rtosEventQueueReceive
#undef rtosEventQueueSend
#undef rtosEventQueueTake
#undef rtosThreadDelay
#undef TakeEventosMsgQueueMutex
#undef ReleaseEventosMsgQueueMutex
#undef TakeEventosMutex
#undef ReleaseEventosMutex
#undef CheckEventRspMesgNum
#undef PeekEventRspMesg

#include "rtosPort.h"
__attribute__((section("._entry_rtos_api")))
const tRTOSEntry entry_rtos_list =
{
    #undef rtosEventQueueReceive
    INIT_MEMBER(rtosEventQueueReceive),
    #undef rtosEventQueueSend
    INIT_MEMBER(rtosEventQueueSend),
    #undef rtosEventQueueTake
    INIT_MEMBER(rtosEventQueueTake),
    #undef rtosThreadDelay
    INIT_MEMBER(rtosThreadDelay),
    #undef TakeEventosMsgQueueMutex
    INIT_MEMBER(TakeEventosMsgQueueMutex),
    #undef ReleaseEventosMsgQueueMutex
    INIT_MEMBER(ReleaseEventosMsgQueueMutex),
    #undef TakeEventosMutex
    INIT_MEMBER(TakeEventosMutex),
    #undef ReleaseEventosMutex
    INIT_MEMBER(ReleaseEventosMutex),
    #undef CheckEventRspMesgNum
    INIT_MEMBER(CheckEventRspMesgNum),
    #undef PeekEventRspMesg
    INIT_MEMBER(PeekEventRspMesg),
};
