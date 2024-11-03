#include "entry_rtos_api.h"
#undef rtosThreadDelay
#undef rtosTaskSuspendAll
#undef rtosTaskResumeAll
#undef rtosEventosGetMsg
#undef rtosEventosSendMsg
#undef rtosTakeMsgFromEventos
#undef rtosDeliverMsgToEventos
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
    #undef rtosEventosGetMsg
    INIT_MEMBER(rtosEventosGetMsg),
    #undef rtosEventosSendMsg
    INIT_MEMBER(rtosEventosSendMsg),
    #undef rtosTakeMsgFromEventos
    INIT_MEMBER(rtosTakeMsgFromEventos),
    #undef rtosDeliverMsgToEventos
    INIT_MEMBER(rtosDeliverMsgToEventos),
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
