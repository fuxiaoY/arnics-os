#include "console_process.h"


#define CMD_MAX_SIZE 512
MctInstance consoleInstance;

void console_process(void)
{
    mct_init(&consoleInstance,NULL,CMD_MAX_SIZE, \
                                NULL,CMD_MAX_SIZE,command_mct_write,command_mct_read);
    mct_reg("CONSOLE",&consoleInstance,false);
    while(1)
    {
        mctApiExecute(&consoleInstance,CMD_REV_FLOW,NULL);
        rtosThreadDelay(100);
    }

}
