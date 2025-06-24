#ifndef SLEEP_PROCESS_H
#define SLEEP_PROCESS_H
#ifdef __cplusplus
extern "C" {
#endif
#include "../../Inc/typedef.h"
#include "../../Inc/ProjDefine.h"
#include "../../core/coreInclude.h"
#include "../../common/commonInclude.h"
#include "../../rtosInterface/entry_rtos_api.h"
#include "../../dataPlat/dataPlatInclude.h"
#include "../../thirdParty/thirdPartyInclude.h"
#include "../../drivers/driversInclude.h"
#include "../dePartment/centerAdministrative/centerAdministrative.h"


extern void ad_process(void);
extern bool readAdReq(adMessage_t *msg,time_t wait);
extern void SendMsgToAdCenter(adMessage_t *message);

#ifdef __cplusplus
}
#endif
#endif
