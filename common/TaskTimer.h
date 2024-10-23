#ifndef _TASKTIMER_H_
#define _TASKTIMER_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "../Inc/projDefine.h"
#include "../Inc/typedef.h"

typedef struct
{
    uint32_t start;
    uint32_t interval;
} tSwTimer;

extern uint32_t arnics_getTick();

extern void arnics_systick_handler();

extern void arnics_addTick(uint32_t addTime);

/**
  * @brief  ����ʱ����ʱ�������
  * @param  [in]  t-����ʱ��
  * @param  [in]  interval-��ʱ�������λMS
  * @retval None
  */
extern void halTimerInterval(tSwTimer* t, uint32_t interval);

/**
  * @brief  ����ʱ����ʱ���¿�ʼ
  * @param  [in]  t-����ʱ��
  * @retval None
  */
extern void halTimerRestart(tSwTimer* t);

/**
  * @brief  ��ѯ����ʱ���Ƿ�ʱ����
  * @param  [in]  t-����ʱ��
  * @retval 0-��ʱδ����
  * @retval 1-��ʱ�ѵ���
  */
extern BOOL halTimerExpired(const tSwTimer* t);

/**
  * @brief  ����ʱ���뵽�ڻ��ж���MS
  * @param  [in]  t-����ʱ��
  * @retval ʣ��ʱ�䣬��λMS
  */
extern uint32_t halTimerRemainingCorrected(const tSwTimer* t);

/**
  * @brief  ����ʱ���е���Сֵ
  * @param  [in]  t-����ʱ��
  * @retval ʣ��ʱ�䣬��λMS
  */
extern uint32_t findMinimum(tSwTimer* a1, tSwTimer* b1, tSwTimer* c1,uint32_t d) ;

#ifdef __cplusplus
}
#endif

#endif
