#include "entry_dataPlat_api.h"
#undef usr_systick
#undef  global_cfg       // 全局配置参数
#undef  global_state     // 全局运行参数
#include "globalData.h"

__attribute__((section("._entry_dataPlat_api")))
const tDATAPLATEntry entry_dataplat_list =
{
    #undef usr_systick
    INIT_MEMBER(usr_systick),
    #undef global_cfg
    INIT_MEMBER(global_cfg),
    #undef usr_systick
    INIT_MEMBER(global_state),
};

