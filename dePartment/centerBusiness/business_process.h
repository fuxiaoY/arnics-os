#ifndef BUSINESS_PROCESS_H
#define BUSINESS_PROCESS_H
#ifdef __cplusplus
extern "C" {
#endif
#include "../../core/coreInclude.h"
#include "../../thirdParty/thirdPartyInclude.h"
#include "../../rtosInterface/entry_rtos_api.h"
#include "../../dataPlat/dataPlatInclude.h"
#include "../centerEvent/entry_event_api.h"
#include "../../drivers/driversInclude.h"

extern void business_process(void);
extern void business_init();

#ifdef __cplusplus
}
#endif
#endif
