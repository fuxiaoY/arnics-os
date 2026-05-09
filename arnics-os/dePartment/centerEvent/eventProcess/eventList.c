#include "eventList.h"
#include "eventCore.h"
//#include "drivers/driversInclude.h"
#include "thirdParty/thirdPartyInclude.h"
#include "rtosInterface/entry_rtos_api.h"

#if PLATFORM_MCU
#pragma arm section code = "._entry_event_api"
#endif


void led0_toggle(void *argv)
{
    UFLOG_ALW("led0_toggle trigger");
    rtosThreadDelay(1000);
}

void led1_action(void *argv)
{
    UFLOG_ALW("led1_action trigger");
}

