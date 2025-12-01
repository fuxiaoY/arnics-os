#include "../ad_process.h"

#pragma arm section code = "._entry_ad_api"


#define MAX_IWDG_TIME_MS 	(25 * 1000)   // 看门狗最大休眠时间
void vPortSuppressTicksAndSleepUsr(uint32_t xExpectedIdleTime)
{

}
/*--------------------------------------------------------------------------------------*/
/*休眠前后低功耗相关操作------------------------------------------------------------------*/
// 休眠前
static void before_sleep()
{

}
// 休眠后
static void after_sleep()
{

}

void SleepControl(uint32_t sleepTime_ms)
{
    // 检查其他中心是否满足休眠条件
    //
    // 查看是否有待查收消息 查看休假申请单
    /*---------------------------------------------------*/
    /*媒体中心--------------------------------------------*/
    if (CheckMediaReqMesgNum() > 0)
    {
        xUFLOG_DBG("admi","sleep",NO_HEX,"Meida need work! Reject!");
        //  休眠条件不满足，继续等待
        return;
    }

    /*---------------------------------------------------*/
    /*消息中心--------------------------------------------*/
    if (CheckEventRspMesgNum() > 0)
    {
        xUFLOG_DBG("admi","sleep",NO_HEX,"evntOS need work! Reject!");
        //  休眠条件不满足，继续等待
        return;
    }

    /*---------------------------------------------------*/
    /*督查中心--------------------------------------------*/

    if (true == sys_sleep_status.guard_is_working)
    {
        xUFLOG_DBG("admi","sleep",NO_HEX,"Guard  working! Reject!");
        //  休眠条件不满足，继续等待
        return;
    }

    if(0 != sys_sleep_status.working_status)
    {
        xUFLOG_DBG("admi","sleep",NO_HEX,"Reject! sys_sleep_status.val = %d", sys_sleep_status.working_status);
        sleep_status_denial_result(sys_sleep_status);
        //  休眠条件不满足，继续等待
        return;
    }
    /*---------------------------------------------------*/
    /*开始休眠--------------------------------------------*/
    uint32_t xSleepTime = sleepTime_ms;
    if (xSleepTime >= 1000 && g_system_cfg.allow_sleep) // 只支持大于1s的休眠
    {
        // 进入低功耗模式休眠
        rtosTaskSuspendAll();
        xUFLOG_ALW("admi","sleep",NO_HEX,"-------------Sleep-------------");
        xUFLOG_ALW("admi","sleep",NO_HEX,"Expected sleep time: %lu ms", xSleepTime);
        before_sleep();
        vPortSuppressTicksAndSleepUsr(xSleepTime);
        after_sleep();
        xUFLOG_ALW("admi","sleep",NO_HEX,"------------Wake Up------------");
        rtosTaskResumeAll();
    }
}

static adMessage_t msg ={0}; // 消息结构体实例

void sleep_process()
{
	while (1)
	{
		if (readAdReq(&msg, BLOCK_DELAY))
		{
			SleepControl(msg.sleepTime_ms);
		}
		rtosThreadDelay(rtosScheduling);
	}
}
