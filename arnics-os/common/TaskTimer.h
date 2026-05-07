/* information */
/**
  ******************************************************************************
  * @file           : TASKTIMER_H
  * @brief          : 软定时器相关的头文件
  * 
  * 该文件定义了软定时器的相关结构体、变量和函数声明。
  * 主要用于任务调度和定时控制。
  * 
  * @version        : 1.0.0
  * @date           : 2023-10-01
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 ARSTUDIO.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* information */
#ifndef _TASKTIMER_H_
#define _TASKTIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------*/
#include "Inc/projDefine.h"      ///< 项目定义头文件
#include "Inc/typedef.h"         ///< 类型定义头文件

/* typedef -----------------------------------------------------------*/
/**
 * @struct tSwTimer
 * @brief 软定时器结构体
 * 
 * 该结构体用于存储软定时器的起始时间和间隔时间。
 */
typedef struct
{
    uint32_t start;       ///< 定时器起始时间
    uint32_t interval;    ///< 定时器间隔时间，单位MS
} tSwTimer;

/* function prototypes -----------------------------------------------*/
/**
 * @fn arnics_getTick
 * @brief 获取当前的任务计数器值
 * 
 * @return 当前的任务计数器值
 */
extern uint32_t arnics_getTick(void);

/**
 * @fn arnics_systick_handler
 * @brief 系统滴答中断处理函数
 * 
 * 该函数用于处理系统滴答中断，更新系统时间。
 * 
 * @retval None
 */
extern void arnics_systick_handler(void);

/**
 * @fn arnics_addTick
 * @brief 增加任务计数器值
 * 
 * @param [in] addTime - 要增加的时间，单位MS
 * @retval None
 */
extern void arnics_addTick(uint32_t addTime);

/**
 * @fn halTimerInterval
 * @brief 设置软定时器的定时间隔
 * 
 * @param [in] t - 软定时器结构体指针
 * @param [in] interval - 定时间隔，单位MS
 * @retval None
 */
extern void halTimerInterval(tSwTimer* t, uint32_t interval);

/**
 * @fn halTimerRestart
 * @brief 重新启动软定时器
 * 
 * @param [in] t - 软定时器结构体指针
 * @retval None
 */
extern void halTimerRestart(tSwTimer* t);

/**
 * @fn halTimerExpired
 * @brief 查询软定时器是否已到期
 * 
 * @param [in] t - 软定时器结构体指针
 * @retval 0 - 定时未到期
 * @retval 1 - 定时已到期
 */
extern bool halTimerExpired(const tSwTimer* t);

/**
 * @fn halTimerRemainingCorrected
 * @brief 获取软定时器离到期的时间
 * 
 * @param [in] t - 软定时器结构体指针
 * @retval 剩余时间，单位MS
 */
extern uint32_t halTimerRemainingCorrected(const tSwTimer* t);
extern uint32_t halTimerRemaining(const tSwTimer* t);
/**
 * @fn findMinimum
 * @brief 查找多个软定时器中的最小剩余时间
 * 
 * @param [in] a1 - 软定时器结构体指针
 * @param [in] b1 - 软定时器结构体指针
 * @param [in] c1 - 软定时器结构体指针
 * @param [in] d - 默认值
 * @retval 最小剩余时间，单位MS
 */
extern uint32_t findMinimum(tSwTimer* a1, tSwTimer* b1, tSwTimer* c1, uint32_t d);

#ifdef __cplusplus
}
#endif

#endif // _TASKTIMER_H_
/************************ (C) COPYRIGHT ARSTUDIO *****END OF FILE***************************/
