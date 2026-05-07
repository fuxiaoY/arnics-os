#include "guard_process.h"
#include "core/coreInclude.h"
#include "thirdParty/thirdPartyInclude.h"
#include "rtosInterface/entry_rtos_api.h"
#include "dataPlat/dataPlatInclude.h"
#if PLATFORM_MCU
_SECTION( "._entry_guard_api")
#endif



void guard_process()
{
    while(1)
    {
        rtosThreadDelay(1000);
    }

}
