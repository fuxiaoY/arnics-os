#ifndef _TASKTIMER_H_
#define _TASKTIMER_H_
#include "../Inc/projDefine.h"
#include "../Inc/typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    uint32_t start;
    uint32_t interval;
} tSwTimer;

extern uint32_t usr_xTaskGetTickCount();

/**
  * @brief  软定时器定时间隔设置
  * @param  [in]  t-软定时器
  * @param  [in]  interval-定时间隔，单位MS
  * @retval None
  */
extern void halTimerInterval(tSwTimer* t, uint32_t interval);

/**
  * @brief  软定时器定时重新开始
  * @param  [in]  t-软定时器
  * @retval None
  */
extern void halTimerRestart(tSwTimer* t);

/**
  * @brief  查询软定时器是否定时到期
  * @param  [in]  t-软定时器
  * @retval 0-定时未到期
  * @retval 1-定时已到期
  */
extern BOOL halTimerExpired(const tSwTimer* t);

/**
  * @brief  软定时器离到期还有多少MS
  * @param  [in]  t-软定时器
  * @retval 剩余时间，单位MS
  */
extern uint32_t halTimerRemainingCorrected(const tSwTimer* t);

/**
  * @brief  软定时器中的最小值
  * @param  [in]  t-软定时器
  * @retval 剩余时间，单位MS
  */
extern uint32_t findMinimum(tSwTimer* a1, tSwTimer* b1, tSwTimer* c1,uint32_t d) ;

#ifdef __cplusplus
}
#endif

#endif
