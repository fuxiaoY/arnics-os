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


// static void CurrentTimeToString(rtcTimeDateTypeDef_t* DT, char* buffer, size_t bufferSize)
// {
//   snprintf(buffer, bufferSize, "%02d-%02d-%02d %02d:%02d:%02d", DT->Year, DT->Month, DT->Date,DT->Hour, DT->Minute, DT->Second);
// }



void business_init()
{
    // uint32_t id = 0;
    // uint32_t randome[5];


    // EventFlag_t event_action;
    // memset(&event_action,0,sizeof(event_action));
    // SET_MESSAGE(event_action,led0_toggle_msg.led0_toggle_req.a = 1);
    // SET_MESSAGE(event_action,led1_action_msg.req_blink_count = 5);  

    // set_event_flag(&event_action,"led1_action",true);
    // id = SendEventCallToEventCenter(event_action, BLOCK_DELAY);
    // GetResponseMessageFromEventCenter(id, BLOCK_DELAY, &event_action.msg);
    // rtosThreadDelay(1000);
    // add_event_flag(&event_action,"led0_toggle",true);
    // id = SendEventCallToEventCenter(event_action, BLOCK_DELAY);
    // GetResponseMessageFromEventCenter(id, BLOCK_DELAY, &event_action.msg);

    while(1)
    {        
        // rtcTimeDateTypeDef_t DT;
        // dev_ctl(&rtc_ds,RTC_GETDATETIME,&DT);
        // char timeStr[32] ={0};
        // CurrentTimeToString(&DT, timeStr, sizeof(timeStr));
        // char tsdb[60] = {0};

        // snprintf(tsdb,sizeof(tsdb) ,"[%s]add test record count: %x,%x",timeStr,randome[0],randome[1]);
        // ts_record((uint8_t*)tsdb, strlen(tsdb));
        // ULOG_ALWAYS("add test record: %x,%x", randome[0],randome[1]);
        rtosThreadDelay(1000);
    }

}
