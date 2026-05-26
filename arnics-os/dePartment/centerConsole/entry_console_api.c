#include "entry_console_api.h"

#undef console_process
#include "console_process.h"
#if defined(__CC_ARM) || defined(__GNUC__) 
#pragma arm section code = "._entry_console_api"
#endif

const tconsoleEntry entry_console_list =
{

    #undef console_process
    INIT_MEMBER(console_process),
};

