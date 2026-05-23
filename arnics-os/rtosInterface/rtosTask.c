#include "rtosInterfacePublic.h"
#include "rtosInterface.h"
#include "core/arnicsCore.h"
#include "dePartment/centerEvent/entry_event_api.h"
#include "dePartment/centerBusiness/entry_business_api.h"
//#include "dePartment/centerConsole/entry_console_api.h"
#include "dePartment/centerMedia/entry_media_api.h"
#include "dePartment/centerGuard/entry_guard_api.h"
#include "dePartment/centerAdministrative/entry_ad_api.h"

#if PLATFORM_MCU
extern void freertos_task_init(void);
#elif PLATFORM_LINUX
extern void linux_os_init(void);
#elif PLATFORM_WIN
extern void win_os_init(void);
#else
#endif
extern void os_task_create(void);
/*---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/
// 统一初始化入口
void arnics_task_init(void)
{
#if PLATFORM_MCU
    freertos_task_init();
#elif PLATFORM_LINUX
    linux_os_init();
#elif PLATFORM_WIN
    win_os_init();
#else
    #error "Please select a platform"
#endif
}

/*---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/
// 任务列表
/**
 * @func StartConsleTask
 * @brief 初始化线程
 * @note 无
 */
void StartInitTask(void const *argument)
{
  arnics_core_init();
  os_task_create();
  rtosTaskSelfDelete();
}

/**
 * @func StartAdTask
 * @brief 行政管理线程
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
 * @brief 控制台线程
 * @param 无
 * @retval 无
 * @note 无
 * @attention
 */
void StartConsleTask(void const *argument)
{

while(1)
{
  //console_process();
  rtosThreadDelay(100);
}
}

/**
 * @func StartEventTask
 * @brief 事件中心线程
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
 * @brief 决策层线程
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
 * 
 * @brief 督察中心线程
 * @param 无
 * @retval 无
 * @note 
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
