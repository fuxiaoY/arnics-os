#include "command_consoleAdapter.h"



static bool cmd_reqHandle(MctInstance *inst,void *para)
{
    return true;
}

static bool cmd_revHandle(MctInstance *inst,void *para)
{
    command_t console_cmd = {0};
    mct_console_execute(inst,false,0,&console_cmd);
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
