#include "command_consoleAdapter.h"
#include "../../dataPlat/dataLink.h"


static bool cmd_reqHandle(MctInstance *inst,void *para)
{
    return true;
}

static bool cmd_revHandle(MctInstance *inst,void *para)
{
    command_t console_cmd = {0};
    return mct_console_execute(inst,NULL_CMD_SEEK,&console_cmd);
}


static const tCmdApi funList[] =
{
    {.id = CMD_REV_FLOW,   .fun = cmd_revHandle},
        
};

tCmdApi const *mctConsoleApiGet(void)
{
    return funList;
}
