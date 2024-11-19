
#include "mctList.h"
#include "console/command_consoleAdapter.h"
//#include "A7680C/command_A7680CAdapter.h"
const tModemList modemList[] =
    {
        {.name = "CONSOLE",    .api = mctConsoleApiGet},
        //{.name = "A7680C",      .api = CMD_A7680CApiGet},


};
uint16_t mctModemLisNumGet(void)
{
    return (sizeof(modemList) / sizeof(tModemList));
}


const tModemList *mctModemListGet(void)
{
    return modemList;
}

