
#include "TaskTimer.h"
#include "../dataPlat/dataPlatInclude.h"
uint32_t arnics_getTick(void)
{
  return arnics_systick;
}

void arnics_systick_handler(void)
{
  arnics_systick++;
}
void arnics_addTick(uint32_t addTime)
{
  arnics_systick += addTime;
}

/**
 * @brief  软定时器定时间隔设置
 * @param  [in]  t-软定时器
 * @param  [in]  interval-定时间隔，单位MS
 * @retval None
 */
void halTimerInterval(tSwTimer *t, uint32_t interval)
{
  t->interval = interval;
  t->start = arnics_getTick();
}
/**
 * @brief  软定时器定时重新开始
 * @param  [in]  t-软定时器
 * @retval None
 */
void halTimerRestart(tSwTimer *t)

{
  t->start = arnics_getTick();
}

/**
  * @brief  查询软定时器是否定时到期 uint32_t wrap-around特性应用
  * @param  [in]  t-软定时器
  * @retval 0-定时未到期
  * @retval 1-定时已到期
  */
bool halTimerExpired(const tSwTimer* t)
{
    uint32_t diff = (arnics_getTick() - t->start) + 1;
    return boolof(t->interval < diff);
}

/**
 * @brief  软定时器离到期还有多少MS，考虑溢出情况
 * @param  [in]  t-软定时器结构体指针
 * @retval 剩余时间，单位MS，如已过期则返回0
 */
uint32_t halTimerRemainingCorrected(const tSwTimer *t)
{
    uint32_t currentTick = arnics_getTick();
    uint32_t startTick = t->start;
    uint32_t interval = t->interval;

    // 直接计算 endTick，利用 C 语言的自动溢出处理机制
    uint32_t endTick = startTick + interval;

    // 计算 endTick 和 startTick 之间的差值
    int32_t endDiff = (int32_t)(endTick - startTick);

    // 计算 currentTick 和 startTick 之间的差值
    int32_t currentDiff = (int32_t)(currentTick - startTick);

    // 检查是否发生了加法溢出
    if (endDiff < 0) {
        // 如果发生了溢出，计算正确的 endDiff
        endDiff = (int32_t)(0xffffffff - startTick + interval + 1);
    }

    // 检查是否发生了 currentTick 的溢出
    if (currentDiff < 0) {
        // 如果发生了溢出，计算正确的 currentDiff
        currentDiff = (int32_t)(0xffffffffUL - startTick + currentTick + 1);
    }

    // 检查是否已经超时
    if (endDiff <= currentDiff) {
        // 如果已经超时，返回 0
        return 0;
    }

    // 计算剩余时间
    return (uint32_t)(endDiff - currentDiff);
}
/**
  * @brief  软定时器离到期还有多少MS -uint32_t wrap-around特性应用
  * @param  [in]  t-软定时器
  * @retval 剩余时间，单位MS
  */
uint32_t halTimerRemaining(const tSwTimer* t)
{
    return (uint32_t)(t->start + t->interval - arnics_getTick());
}

/**
 * @brief  软定时器中的最小值
 * @param  [in]  t-软定时器  d1:标准值
 * @retval 剩余时间，单位MS
 */
uint32_t findMinimum(tSwTimer *a1, tSwTimer *b1, tSwTimer *c1, uint32_t d)
{
  uint32_t a = halTimerRemainingCorrected(a1);
  uint32_t b = halTimerRemainingCorrected(b1);
  uint32_t c = halTimerRemainingCorrected(c1);
  //0xffffffff表示关闭
  if(0xFFFFFFFF == a1->interval )
  {
    a = 0xffffffff;
  }
  if(0xFFFFFFFF == b1->interval )
  {
    b = 0xffffffff;
  }
  if(0xFFFFFFFF == c1->interval )
  {
    c = 0xffffffff;
  }
  uint32_t min = a;
  if (b < min)
  {
    min = b;
  }
  if (c < min)
  {
    min = c;
  }
  if (d < min)
  {
    min = d;
  }
  return min;
}
