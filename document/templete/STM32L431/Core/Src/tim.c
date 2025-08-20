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

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{
  if(tim_baseHandle->Instance==TIM7)
  {
  /* USER CODE BEGIN TIM7_MspInit 0 */

  /* USER CODE END TIM7_MspInit 0 */
    /* TIM7 clock enable */
    __HAL_RCC_TIM7_CLK_ENABLE();

    /* TIM7 interrupt Init */
    HAL_NVIC_SetPriority(TIM7_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(TIM7_IRQn);
  /* USER CODE BEGIN TIM7_MspInit 1 */

  /* USER CODE END TIM7_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM15)
  {
  /* USER CODE BEGIN TIM15_MspInit 0 */

  /* USER CODE END TIM15_MspInit 0 */
    /* TIM15 clock enable */
    __HAL_RCC_TIM15_CLK_ENABLE();

    /* TIM15 interrupt Init */
    HAL_NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);
  /* USER CODE BEGIN TIM15_MspInit 1 */

  /* USER CODE END TIM15_MspInit 1 */
  }
}
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{
  if(tim_baseHandle->Instance==TIM7)
  {
  /* USER CODE BEGIN TIM7_MspDeInit 0 */

  /* USER CODE END TIM7_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM7_CLK_DISABLE();

    /* TIM7 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM7_IRQn);
  /* USER CODE BEGIN TIM7_MspDeInit 1 */

  /* USER CODE END TIM7_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM15)
  {
  /* USER CODE BEGIN TIM15_MspDeInit 0 */

  /* USER CODE END TIM15_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM15_CLK_DISABLE();

    /* TIM15 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM1_BRK_TIM15_IRQn);
  /* USER CODE BEGIN TIM15_MspDeInit 1 */

  /* USER CODE END TIM15_MspDeInit 1 */
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
    uint32_t timer_clock = HAL_RCC_GetPCLK1Freq();
    
    // 如果是高级定时器或通用定时器，需要考虑预分频情况
    // 对于STM32L4系列，TIM2-7, TIM12-14在APB1上
    // TIM1, TIM8, TIM15-17在APB2上
    
    // 计算需要的计数次数
    uint32_t total_counts = (uint32_t)(dev->period * timer_clock);
    
    // 如果计数次数小于等于0xFFFF，可以直接使用
    if (total_counts <= 0xFFFF) 
    {
        dev->htim.Init.Prescaler = 0;  // 不分频
        dev->htim.Init.Period = total_counts - 1;
    } 
    // 如果计数次数大于0xFFFF，需要使用预分频
    else 
    {
        uint32_t prescaler = (total_counts - 1) / 0xFFFF;
        if (prescaler > 0xFFFF) 
        {
            // 超出范围，无法设置
            return -1;
        }
        dev->htim.Init.Prescaler = prescaler;
        dev->htim.Init.Period = (total_counts / (prescaler + 1)) - 1;
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



