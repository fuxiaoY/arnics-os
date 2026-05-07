
#include "entry_media_api.h"

#undef media_process
#undef readMediaReq
#undef SendMsgToMediaCenter
#include "media_process.h"
#if PLATFORM_MCU
__attribute__((section("._entry_media_api")))
#endif
const tMEDIAEntry entry_media_list =
{
    #undef media_process
    INIT_MEMBER(media_process),
    #undef readMediaReq
    INIT_MEMBER(readMediaReq),
    #undef SendMsgToMediaCenter
    INIT_MEMBER(SendMsgToMediaCenter),
};
