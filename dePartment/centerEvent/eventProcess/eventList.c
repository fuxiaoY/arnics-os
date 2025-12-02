#include "eventList.h"
#include "eventCore.h"
#include "drivers/driversInclude.h"
#include "thirdParty/thirdPartyInclude.h"
#include "rtosInterface/entry_rtos_api.h"

#if defined(__CC_ARM) || defined(__GNUC__) 
#pragma arm section code = "._entry_event_api"
#endif




