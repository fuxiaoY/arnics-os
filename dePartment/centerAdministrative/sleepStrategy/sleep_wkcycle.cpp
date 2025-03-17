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
	/*业务层---------------------------------------------*/
	if (FALSE == BusinessWantSleep)
	{
		// ULOG_ALWAYS(" Business  working! Reject!");
		//  休眠条件不满足，继续等待
		return;
	}
	/*---------------------------------------------------*/
	/*媒体中心--------------------------------------------*/
	if (FALSE == MediaWantSleep)
	{
		// ULOG_ALWAYS("Meida  working! Reject!");
		//  休眠条件不满足，继续等待
		return;
	}
	if (CheckMediaRspMesgNum() > 0)
	{
		// ULOG_ALWAYS("Meida need work! Reject!");
		//  休眠条件不满足，继续等待
		return;
	}
	/*---------------------------------------------------*/
	/*消息中心--------------------------------------------*/
	if (FALSE == eventosWantSleep)
	{
		// ULOG_ALWAYS("evntOS  working! Reject!");
		//  休眠条件不满足，继续等待
		return;
	}

	if (CheckEventRspMesgNum() > 0)
	{
		// ULOG_ALWAYS("evntOS need work! Reject!");
		//  休眠条件不满足，继续等待
		return;
	}
	/*---------------------------------------------------*/
	/*控制台--------------------------------------------*/
	if (FALSE == ConsoleWantSleep)
	{
		// ULOG_ALWAYS("Console  working! Reject!");
		//  休眠条件不满足，继续等待
		return;
	}
	/*---------------------------------------------------*/
	/*督查中心--------------------------------------------*/

	if (FALSE == GuardWantSleep)
	{
		// ULOG_ALWAYS("Guard  working! Reject!");
		//  休眠条件不满足，继续等待
		return;
	}
	/*---------------------------------------------------*/
	/*开始休眠--------------------------------------------*/
	uint32_t xSleepTime = sleepTime_ms;
	if (xSleepTime >= 1000 && ALLOWSLEEP) // 只支持大于1s的休眠
	{
		// 进入低功耗模式休眠
		rtosTaskSuspendAll();
		ULOG_ALWAYS("-------------Sleep-------------");
		ULOG_ALWAYS("  Expected sleep time: %lu ms", xSleepTime);
		before_sleep();
		vPortSuppressTicksAndSleepUsr(xSleepTime);
		after_sleep();
		ULOG_ALWAYS("------------Wake Up------------\r\n");
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
