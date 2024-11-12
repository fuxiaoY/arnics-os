#include "command_consoleAdapter.h"



static bool cmd_reqHandle(MctInstance *inst,void *para)
{
    mct_console_execute(inst,true,CMD_CONSOLE_ID_REV,para);
    return true;
}

static bool cmd_revHandle(MctInstance *inst,void *para)
{
    mct_console_execute(inst,false,0,para);
    return true;
}


static const tCmdApi funList[] =
{
    {.id = CMD_REV_FLOW,   .fun = cmd_revHandle},
        
};

tCmdApi const *mctConsoleApiGet(void)
{
    return funList;
}
