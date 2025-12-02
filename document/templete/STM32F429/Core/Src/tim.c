/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

TIM_HandleTypeDef htim10;

/* TIM10 init function */
void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 0;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 65535;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM10)
  {
  /* USER CODE BEGIN TIM10_MspInit 0 */

  /* USER CODE END TIM10_MspInit 0 */
    /* TIM10 clock enable */
    __HAL_RCC_TIM10_CLK_ENABLE();
  /* USER CODE BEGIN TIM10_MspInit 1 */
  HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 6, 0);  // 设置优先级
  HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn); 
  /* USER CODE END TIM10_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM4)
  {
    __HAL_RCC_TIM4_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM4_IRQn, 6, 0);  // 设置优先级
    HAL_NVIC_EnableIRQ(TIM4_IRQn); 
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM10)
  {
  /* USER CODE BEGIN TIM10_MspDeInit 0 */

  /* USER CODE END TIM10_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM10_CLK_DISABLE();
  /* USER CODE BEGIN TIM10_MspDeInit 1 */
  HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn); 
  /* USER CODE END TIM10_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM4)
  {
    __HAL_RCC_TIM4_CLK_DISABLE();
    HAL_NVIC_EnableIRQ(TIM4_IRQn); 
  }
}

/* USER CODE BEGIN 1 */
// .c file
/**
 * @brief 根据目标周期自动计算定时器参数
 * @param dev 定时器设备结构体指针
 * @return 0: 成功, -1: 失败
 */
static int bsp_tim_calculate_period(tim_t *dev)
{
    // 获取定时器时钟频率
    uint32_t timer_clock;
    uint32_t hclk_freq = HAL_RCC_GetHCLKFreq();
    uint32_t apb1_clock = HAL_RCC_GetPCLK1Freq();
    uint32_t apb2_clock = HAL_RCC_GetPCLK2Freq();
    
    // 根据定时器实例确定时钟源
    // APB1总线上的定时器: TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM12, TIM13, TIM14
    // APB2总线上的定时器: TIM1, TIM8, TIM9, TIM10, TIM11
    if (dev->htim.Instance == TIM1 || dev->htim.Instance == TIM8 ||
      dev->htim.Instance == TIM9 || dev->htim.Instance == TIM10 || 
      dev->htim.Instance == TIM11) {
      timer_clock = apb2_clock;
      // 如果APB2有分频(预分频系数不为1)，定时器时钟是APB2时钟的2倍
      if (hclk_freq != apb2_clock) 
      {
          timer_clock = apb2_clock * 2;
      }
  } else {
      timer_clock = apb1_clock;
      // 如果APB1有分频(预分频系数不为1)，定时器时钟是APB1时钟的2倍
      if (hclk_freq != apb1_clock) 
      {
          timer_clock = apb1_clock * 2;
      }
  }
    
    // 计算需要的计数次数
    double total_counts = dev->period * timer_clock;
    
    // 如果计数次数小于等于0xFFFF，可以直接使用
    if (total_counts <= 65535.0) 
    {
        dev->htim.Init.Prescaler = 0;  // 不分频
        dev->htim.Init.Period = (uint32_t)(total_counts + 0.5) - 1; // 四舍五入
    } 
    // 如果计数次数大于0xFFFF，需要使用预分频
    else 
    {
        uint32_t prescaler = (uint32_t)((total_counts - 1) / 65535.0) + 1;
        if (prescaler > 65535) 
        {
            // 超出范围，无法设置
            return -1;
        }
        dev->htim.Init.Prescaler = prescaler - 1;
        dev->htim.Init.Period = (uint32_t)((total_counts / prescaler) + 0.5) - 1; // 四舍五入
    }
    return 0;
}


int bsp_tim_init(tim_t *dev)
{
  if(0 != bsp_tim_calculate_period(dev))
  {
    return -1;
  }
  if (HAL_TIM_Base_Init(&dev->htim) != HAL_OK)
  {
    Error_Handler();
    return -1;
  }
  if (HAL_TIM_ConfigClockSource(&dev->htim, &dev->sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
    return -2;
  }
  if (HAL_TIMEx_MasterConfigSynchronization(&dev->htim, &dev->sMasterConfig) != HAL_OK)
  {
    Error_Handler();
    return -3;
  }

  // 启用 TIM 的更新中断
  if (HAL_TIM_Base_Start_IT(&dev->htim) != HAL_OK)
  {
    Error_Handler();
    return -4;
  }
  return 0;
}
int bsp_tim_close(tim_t *dev)
{
  if (HAL_TIM_Base_DeInit(&dev->htim) != HAL_OK)
  {
    Error_Handler();
    return -1;
  }
  else
  {
    return 0;
  }
}

void bsp_tim_irq(tim_t *dev)
{
  HAL_TIM_IRQHandler(&dev->htim);
}



