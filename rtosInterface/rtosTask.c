
#include "../Inc/projDefine.h"
#include "../Inc/typedef.h"
#include "rtosTask.h"
#include "rtosPort.h"
#include "../dePartment/centerEvent/entry_event_api.h"
#include "../dePartment/centerBusiness/entry_business_api.h"
#include "../dePartment/centerConsole/entry_console_api.h"
#include "../dePartment/centerMedia/entry_media_api.h"
#include "../dePartment/centerGuard/entry_guard_api.h"
#include "../dePartment/centerAdministrative/entry_ad_api.h"
/* ---------------------------------用户任务 -----------------------------------------------*/
/**
 * @func StartAdTask
 * @brief 行政管理专用线程
 * @note 无
 */
void StartAdTask(void const *argument)
{

  while (1)
  {
    ad_process();
    rtosThreadDelay(100);
  }
}

/**
 * @func StartConsleTask
 * @brief 控制台专用线程
 * @param 无
 * @retval 无
 * @note 无
 * @attention
 */
void StartConsleTask(void const *argument)
{

while(1)
{
  console_process();
  rtosThreadDelay(100);
}
}

/**
 * @func StartEventTask
 * @brief eventOS事件线程
 * @param 无
 * @retval 无
 * @note 无  
 * @attention
 */
void StartEventTask(void const *argument)
{

  while (1)
  {
    event_process();
    rtosThreadDelay(100);
  }

}
/**
 * @func StartMaintTask
 * @brief 媒体中心线程
 * @param 无
 * @retval 无
 * @note 无
 * @attention
 */
void StartMediaTask(void const *argument)
{

  while (1)
  {
    media_process();
    rtosThreadDelay(1000);
  }
}

/**
 * @func StartMaintTask
 * @brief 主功能线程
 * @param 无
 * @retval 无
 * @note 无
 * @attention
 */
void StartMaintTask(void const *argument)
{

  while (1)
  {
    business_process();
    rtosThreadDelay(1000);
  }
}

/**
 * @func StartGuardTask
 * @brief 守护线程
 * @param 无
 * @retval 无
 * @note 用于喂狗操作
 * @attention
 */
void StartGuardTask(void const *argument)
{
  while (1)
  {
    guard_process();
    rtosThreadDelay(1000);
  }
}
