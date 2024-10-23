#include "entry_business_api.h"

#undef business_process
#include "business_process.h"

__attribute__((section("._entry_business_api")))
const tBUSINESSEntry entry_business_list =
{

    #undef business_process
    INIT_MEMBER(business_process),
};

