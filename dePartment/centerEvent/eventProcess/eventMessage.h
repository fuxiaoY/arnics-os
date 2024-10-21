#ifndef _EVENMESSAGE_H
#define _EVENMESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../Inc/projDefine.h"
#include "../../../Inc/typedef.h"
#include "../../../core/coreInclude.h"
#include "../../../rtosInterface/rtosInclude.h"
#include "../../../dataPlat/entry_dataPlat_api.h"
#include "eventCore.h"
extern BaseType_t NonBlockingQueueReceive(QueueHandle_t queue, Message_t *message);
extern BaseType_t BlockingQueueReceive(QueueHandle_t queue, Message_t *message);
extern time_t SendEventFlagToEventCenter(EVENT_BYTE_T *eventflag, time_t wait);
extern bool GetResponseMessageFromEventCenter(time_t ID, time_t wait);
#ifdef __cplusplus
}
#endif
#endif
