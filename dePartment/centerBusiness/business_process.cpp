#include "business_process.h"

_SECTION( "._entry_business_api")
wisdomStackPart_t business_state = WORKSTAT_INIT;


/*-------------------------------------------------------------------------------------*/

/*状态主函数-----------------------------------------------------------------------------*/

// 定义状态列表 
const static SEPARATE_STATE BusinessList[] = {
    SEPARATE_DEFINE_STATE(WORKSTAT_INIT, business_init)
};

// 默认的回调函数 
static void SEPARATE_DEFAULT_NOT_FOUND_CALLBACK() {
    business_state = WORKSTAT_INIT;
}
// 初始化内核
SEPARATE_INIT_KERNEL(BusinessList, wisdomStackPart_t, &(business_state), SEPARATE_DEFAULT_NOT_FOUND_CALLBACK);

void business_process(void)
{
    // 运行内核 
    SEPARATE_RUN_KERNEL(1);
}



void business_init()
{
    while(1)
    {
        rtosThreadDelay(1000);
    }
}
