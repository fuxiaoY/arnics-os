
#include "entry_ad_api.h"

#undef ad_process
#undef readAdReq
#undef SendMsgToAdCenter
#include "ad_process.h"

__attribute__((section("._entry_ad_api")))
const tADEntry entry_ad_list =
{
    #undef sleep_process
    INIT_MEMBER(ad_process),
    #undef readSleepReq
    INIT_MEMBER(readAdReq),
    #undef SendMsgToSleepCenter
    INIT_MEMBER(SendMsgToAdCenter),
};
