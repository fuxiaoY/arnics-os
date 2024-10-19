#include "entry_dataPlat_api.h"
#undef  arnics_systick
#undef  arnics_start
#undef  global_cfg       
#undef  global_state     
#undef  eventosWantSleep

#undef UnitySystemInterface
#include "globalData.h"
#include "globalDataManager.h"

__attribute__((section("._entry_dataPlat_api")))
const tDATAPLATEntry entry_dataplat_list =
{
    #undef arnics_systick
    INIT_MEMBER(arnics_systick),
    #undef arnics_start
    INIT_MEMBER(arnics_start),
    #undef global_cfg
    INIT_MEMBER(global_cfg),
    #undef usr_systick
    INIT_MEMBER(global_state),
    #undef eventosWantSleep
    INIT_MEMBER(eventosWantSleep),
    #undef UnitySystemInterface
    INIT_MEMBER(UnitySystemInterface),
};

