#include "guard_process.h"
#include "core/coreInclude.h"
#include "thirdParty/thirdPartyInclude.h"
#include "rtosInterface/entry_rtos_api.h"
#include "dataPlat/dataPlatInclude.h"
#if PLATFORM_MCU
_SECTION( "._entry_guard_api")
#endif

void arnics_sys_tick_check(void)
{
    static uint32_t last_tick     = 0xff;
    uint32_t     current_tick     = arnics_getTick();
    if (current_tick == last_tick)
    {
        UFLOG_ERR("guard:arnics_systick is not changing, system clock may not be running!");
    }
    else 
    {
        last_tick = current_tick;
    }
}

void guard_process()
{

    while(1)
    {
        // 检查 tick 在变化，表明时钟未失效
        arnics_sys_tick_check();
        rtosThreadDelay(1000);
    }

}
