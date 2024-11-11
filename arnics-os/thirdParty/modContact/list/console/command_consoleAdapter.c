#include "command_consoleAdapter.h"



static bool cmd_reqHandle(MctInstance *inst,void *para)
{
    mct_console_execute(inst,true,CMD_CONSOLE_ID_TEST1,para);
    return true;
}

static bool cmd_revHandle(MctInstance *inst,void *para)
{
    mct_console_execute(inst,false,0,para);
    return true;
}


static const tCmdApi funList[] =
{
    {.id = CMD_STICKFRAME, .fun = cmd_reqHandle},
    {.id = CMD_USERCALL,   .fun = cmd_revHandle},
        
};

tCmdApi const *mctConsoleApiGet(void)
{
    return funList;
}
