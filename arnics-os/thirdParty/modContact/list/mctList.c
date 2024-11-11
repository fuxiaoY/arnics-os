
#include "mctList.h"
#include "console/command_consoleAdapter.h"

const tModemList modemList[] =
    {
        {.name = "CONSOLE",    .api = mctConsoleApiGet},
        //{.name = "Y7025",      .api = CMD_Y7025ApiGet},
        //{.name = "BG95",        .api = CMD_BG95ApiGet},
        //{.name = "L710",        .api = CMD_L710ApiGet},
        //{.name = "BC92",        .api = CMD_BC92ApiGet},
        //{.name = "L651",        .api = CMD_L651ApiGet},

};
uint16_t mctModemLisNumGet(void)
{
    return (sizeof(modemList) / sizeof(tModemList));
}


const tModemList *mctModemListGet(void)
{
    return modemList;
}

