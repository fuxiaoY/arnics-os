#include "eventList.h"
#include "eventCore.h"
//#include "drivers/driversInclude.h"
#include "thirdParty/thirdPartyInclude.h"
#include "rtosInterface/entry_rtos_api.h"

#if PLATFORM_MCU
#pragma arm section code = "._entry_event_api"
#endif


