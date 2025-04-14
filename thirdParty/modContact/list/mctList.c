
#include "mctList.h"
#include "console/command_consoleAdapter.h"

//#include "A7680C/command_A7680CAdapter.h"
const tModemList modemList[] =
    {
        {.name = "CONSOLE",    .api = mctConsoleApiGet},
        {.name = "MODBUS",    .api = mctModbusApiGet},
        //{.name = "A7680C",      .api = CMD_A7680CApiGet},
        //{.name = "Y7025",        .api = CMD_Y7025ApiGet},

};
uint16_t mctModemLisNumGet(void)
{
    return (sizeof(modemList) / sizeof(tModemList));
}


const tModemList *mctModemListGet(void)
{
    return modemList;
}

