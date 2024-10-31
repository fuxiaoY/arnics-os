#ifndef _RTOSTASK_H_
#define _RTOSTASK_H_ 

#ifdef __cplusplus
extern "C" {
#endif

#include "../Inc/ProjDefine.h"
#include "../Inc/typedef.h"


/**
 * @func StartConsleTask
 * @brief 休眠专用线程
 * @note 无
 */
extern void StartSleepTask(void const *argument);

/**
 * @func StartConsleTask
 * @brief 控制台专用线程
 * @param 无
 * @retval 无
 * @note 无
 * @attention
 */
extern void StartConsleTask(void const *argument);

/**
 * @func StartEventTask
 * @brief eventOS事件线程
 * @param 无
 * @retval 无
 * @note 无
 * @attention
 */
extern void StartEventTask(void const *argument);


/**
 * @func StartMaintTask
 * @brief 主功能线程
 * @param 无
 * @retval 无
 * @note 无
 * @attention
 */
extern void StartMaintTask(void const *argument);


/**
 * @func StartGuardTask
 * 
 * @brief 守护线程
 * @param 无
 * @retval 无
 * @note 用于喂狗操作
 * @attention
 */
extern void StartGuardTask(void const *argument);



#ifdef __cplusplus
}
#endif
#endif
