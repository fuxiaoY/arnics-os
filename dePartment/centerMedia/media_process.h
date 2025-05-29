#ifndef MEDIA_PROCESS_H
#define MEDIA_PROCESS_H
#ifdef __cplusplus
extern "C" {
#endif
#include "../../Inc/ProjDefine.h"
#include "../../Inc/typedef.h"
#include "../../core/coreInclude.h"
#include "../../thirdParty/thirdPartyInclude.h"
#include "../../rtosInterface/entry_rtos_api.h"
#include "../../dataPlat/dataPlatInclude.h"





extern void media_process(void);
extern bool readMediaReq(mediaMessage_t *msg,time_t wait);
extern void SendMsgToMediaCenter(mediaMessage_t *message);

#ifdef __cplusplus
}
#endif
#endif
