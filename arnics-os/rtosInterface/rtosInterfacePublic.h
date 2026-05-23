#ifndef _RTOSINTERFACEPUBLIC_H_
#define _RTOSINTERFACEPUBLIC_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/projDefine.h"
#include "Inc/typedef.h"

#define BLOCK_DELAY 0xffffffffUL

typedef enum  
{
    rtosPriorityIdle          = -3,          ///< priority: idle (lowest)
    rtosPriorityLow           = -2,          ///< priority: low
    rtosPriorityBelowNormal   = -1,          ///< priority: below normal
    rtosPriorityNormal        =  0,          ///< priority: normal (default)
    rtosPriorityAboveNormal   = +1,          ///< priority: above normal
    rtosPriorityHigh          = +2,          ///< priority: high
    rtosPriorityRealtime      = +3,          ///< priority: realtime (highest)
    rtosPriorityError         =  0x84        ///< system cannot determine priority or thread has illegal priority
} rtosPriority_e;


/*---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/
// 任务列表
/**
 * @func StartConsleTask
 * @brief 初始化线程
 * @note 无
 */
extern void StartInitTask(void const *argument);

/**
 * @func StartConsleTask
 * @brief 行政管理线程
 * @note 无
 */
extern void StartAdTask(void const *argument);

/**
 * @func StartConsleTask
 * @brief 控制台线程
 * @param 无
 * @retval 无
 * @note 无
 * @attention
 */
extern void StartConsleTask(void const *argument);

/**
 * @func StartEventTask
 * @brief 事件中心线程
 * @param 无
 * @retval 无
 * @note 无
 * @attention
 */
extern void StartEventTask(void const *argument);

/**
 * @func StartMediaTask
 * @brief 媒体中心线程
 * @param 无
 * @retval 无
 * @note 无
 * @attention
 */
extern void StartMediaTask(void const *argument);
/**
 * @func StartMaintTask
 * @brief 决策层线程
 * @param 无
 * @retval 无
 * @note 无
 * @attention
 */
extern void StartMaintTask(void const *argument);


/**
 * @func StartGuardTask
 * 
 * @brief 督察中心线程
 * @param 无
 * @retval 无
 * @note 
 * @attention
 */
extern void StartGuardTask(void const *argument);

#ifdef __cplusplus
}
#endif
#endif
