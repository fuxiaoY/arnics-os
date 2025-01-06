#include "guard_process.h"
#include "../../core/coreInclude.h"
#include "../../thirdParty/thirdPartyInclude.h"
#include "../../rtosInterface/entry_rtos_api.h"
#include "../../dataPlat/dataPlatInclude.h"

_SECTION( "._entry_guard_api")


void guard_process()
{
    while(1)
    {
        rtosThreadDelay(1000);
    }

}
