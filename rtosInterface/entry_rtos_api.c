#include "entry_rtos_api.h"
#undef cpuInfo
#undef vApplicationMallocFailedHook
#undef vApplicationStackOverflowHook
#undef rtosTaskSuspendAll
#undef rtosTaskResumeAll
#undef rtosThreadDelayUntil
#undef rtosThreadDelay

#undef rtosAdGetMsg
#undef rtosAdSendMsg
#undef rtosTakeMsgFromAd
#undef rtosDeliverMsgToAd
#undef TakeAdMsgQueueMutex
#undef ReleaseAdMsgQueueMutex
#undef CheckAdqueueSpacesAvailable
#undef CheckAdRspMesgNum
#undef CheckAdReqMesgNum
#undef PeekAdRspMesg
#undef CheckAdQueueSpacesAvailable

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
#undef CheckEventQueueSpacesAvailable

#undef rtosMediaGetMsg
#undef rtosMediaSendMsg
#undef rtosTakeMsgFromMedia
#undef rtosDeliverMsgToMedia
#undef TakeMediaMutex
#undef ReleaseMediaMsgQueueMutex
#undef CheckMediaRspMesgNum
#undef CheckMediaReqMesgNum
#undef PeekMediaRspMesg
#undef CheckMediaQueueSpacesAvailable
#include "rtosPort.h"
__attribute__((section("._entry_rtos_api")))
const tRTOSEntry entry_rtos_list =
{
    #undef cpuInfo
    INIT_MEMBER(cpuInfo),
    #undef rtosTaskSuspendAll
    INIT_MEMBER(rtosTaskSuspendAll),
    #undef rtosTaskResumeAll
    INIT_MEMBER(rtosTaskResumeAll),
    #undef rtosThreadDelayUntil
    INIT_MEMBER(rtosThreadDelayUntil),
    #undef rtosThreadDelay
    INIT_MEMBER(rtosThreadDelay),

    #undef rtosAdGetMsg
    INIT_MEMBER(rtosAdGetMsg),
    #undef rtosAdSendMsg
    INIT_MEMBER(rtosAdSendMsg),
    #undef rtosTakeMsgFromAd
    INIT_MEMBER(rtosTakeMsgFromAd),
    #undef rtosDeliverMsgToAd
    INIT_MEMBER(rtosDeliverMsgToAd),
    #undef TakeAdMsgQueueMutex
    INIT_MEMBER(TakeAdMsgQueueMutex),
    #undef ReleaseAdMsgQueueMutex
    INIT_MEMBER(ReleaseAdMsgQueueMutex),
    #undef CheckAdqueueSpacesAvailable
    INIT_MEMBER(CheckAdqueueSpacesAvailable),
    #undef CheckAdRspMesgNum
    INIT_MEMBER(CheckAdRspMesgNum),
    #undef CheckAdReqMesgNum
    INIT_MEMBER(CheckAdReqMesgNum),     
    #undef PeekAdRspMesg
    INIT_MEMBER(PeekAdRspMesg),
    #undef CheckAdQueueSpacesAvailable
    INIT_MEMBER(CheckAdQueueSpacesAvailable),

    #undef rtosEventosGetMsg
    INIT_MEMBER(rtosEventosGetMsg),
    #undef rtosEventosSendMsg
    INIT_MEMBER(rtosEventosSendMsg),
    #undef rtosTakeMsgFromEventos
    INIT_MEMBER(rtosTakeMsgFromEventos),
    #undef rtosDeliverMsgToEventos
    INIT_MEMBER(rtosDeliverMsgToEventos),
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
    #undef CheckEventQueueSpacesAvailable
    INIT_MEMBER(CheckEventQueueSpacesAvailable),

    #undef rtosMediaGetMsg
    INIT_MEMBER(rtosMediaGetMsg),
    #undef rtosMediaSendMsg
    INIT_MEMBER(rtosMediaSendMsg),
    #undef rtosTakeMsgFromMedia
    INIT_MEMBER(rtosTakeMsgFromMedia),
    #undef rtosDeliverMsgToMedia
    INIT_MEMBER(rtosDeliverMsgToMedia),
    #undef TakeMediaMutex
    INIT_MEMBER(TakeMediaMutex),
    #undef ReleaseMediaMsgQueueMutex
    INIT_MEMBER(ReleaseMediaMsgQueueMutex),
    #undef CheckMediaRspMesgNum
    INIT_MEMBER(CheckMediaRspMesgNum),
    #undef PeekMediaRspMesg
    INIT_MEMBER(PeekMediaRspMesg),
    #undef CheckMediaQueueSpacesAvailable
    INIT_MEMBER(CheckMediaQueueSpacesAvailable),
};
