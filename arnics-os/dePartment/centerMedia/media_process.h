#ifndef MEDIA_PROCESS_H
#define MEDIA_PROCESS_H
#ifdef __cplusplus
extern "C" {
#endif
#include "../../Inc/typedef.h"
#include "../../core/coreInclude.h"
#include "../../thirdParty/thirdPartyInclude.h"
#include "../../rtosInterface/entry_rtos_api.h"
#include "../../dataPlat/entry_dataPlat_api.h"
#include "../../drivers/driversInclude.h"




extern void media_process(void);
extern bool readMediaReq(MediaMessage_t *msg,time_t wait);
extern void SendMsgToMediaCenter(MediaMessage_t *message);

#ifdef __cplusplus
}
#endif
#endif
