
#include "../Inc/projDefine.h"
#include "../Inc/typedef.h"
#include "../dataPlat/globalDef.h"
#include "rtosTask.h"


#ifdef _USE_FREERTOS_
void rtosThreadDelay(uint32_t ms)
{
  osDelay(ms);
}
#else
void rtosThreadDelay(uint32_t ms)
{

}
#endif // _USE_FREERTOS_


/* ---------------------------------休眠相关 -----------------------------------------------*/
/**
 * @func StartConsleTask
 * @brief 休眠专用线程
 * @note 无
 */
void StartSleepTask(void const *argument)
{

  while (1)
  {

    rtosThreadDelay(100);
  }
}

/* ---------------------------------用户任务 -----------------------------------------------*/
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
    rtosThreadDelay(100);
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
    rtosThreadDelay(1000);
  }
}
