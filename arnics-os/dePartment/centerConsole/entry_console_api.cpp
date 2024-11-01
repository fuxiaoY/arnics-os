#include "entry_console_api.h"

#undef console_process
#include "console_process.h"

__attribute__((section("._entry_console_api")))
const tconsoleEntry entry_console_list =
{

    #undef console_process
    INIT_MEMBER(console_process),
};

