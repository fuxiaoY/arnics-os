#include "entry_guard_api.h"

#undef guard_process
#include "guard_process.h"

__attribute__((section("._entry_guard_api")))
const tGUARDEntry entry_guard_list =
{

    #undef guard_process
    INIT_MEMBER(guard_process),
};

