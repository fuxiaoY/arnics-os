
#include "entry_media_api.h"

#undef media_process
#undef readMediaReq
#undef SendMsgToMediaCenter
#include "media_process.h"

__attribute__((section("._entry_media_api")))
const tMEDIAEntry entry_media_list =
{
    #undef media_process
    INIT_MEMBER(media_process),
    #undef readMediaReq
    INIT_MEMBER(readMediaReq),
    #undef SendMsgToMediaCenter
    INIT_MEMBER(SendMsgToMediaCenter),
};
