#include "console_process.h"
#include "../../core/coreInclude.h"
#include "../../rtosInterface/entry_rtos_api.h"
#include "../../thirdParty/thirdPartyInclude.h"
#include "../../drivers/driversInclude.h"

#define CMD_MAX_SIZE 512
MctInstance consoleInstance;
uint8_t console_tx_cache[CMD_MAX_SIZE];
uint8_t console_rx_cache[CMD_MAX_SIZE];

void console_process(void)
{
    mct_init(&consoleInstance,console_tx_cache,CMD_MAX_SIZE, \
                                console_rx_cache,CMD_MAX_SIZE,command_mct_write,command_mct_read);
    mct_reg("CONSOLE",&consoleInstance,false);
    while(1)
    {
        mctApiExecute(&consoleInstance,CMD_REV_FLOW,NULL);
        rtosThreadDelay(100);
    }

}
