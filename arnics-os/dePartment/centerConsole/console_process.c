#include "console_process.h"

#if PLATFORM_MCU
#pragma arm section code = "._entry_console_api"
#endif

void console_process(void)
{
    while(1)
    {
        rtosThreadDelay(1000);
    }
}
