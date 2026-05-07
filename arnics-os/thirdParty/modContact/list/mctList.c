
#include "mctList.h"

const tModemList modemList[] =
    {
        {.name = "CONSOLE",    .api = mctConsoleApiGet},

};
uint16_t mctModemLisNumGet(void)
{
    return (sizeof(modemList) / sizeof(tModemList));
}


const tModemList *mctModemListGet(void)
{
    return modemList;
}

