/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "adc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};
  memset(&hadc1,0,sizeof(ADC_HandleTypeDef)); 
  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel*/
  
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel*/
  
 
  sConfig.Channel = ADC_CHANNEL_VREFINT;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* ADC1 clock enable */
    __HAL_RCC_ADC_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PC2     ------> ADC1_IN3
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PC2     ------> ADC1_IN3
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_2);

  /* USER CODE BEGIN ADC1_MspDeInit 1 */
  // 配置 GPIC2 为模拟输入
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
//.C
void bsp_adc_init(adc_t *adcx)
{
  if (HAL_ADC_Init(&adcx->hadc) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_ADC_ConfigChannel(&adcx->hadc, &adcx->sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}
void bsp_adc_close(adc_t *adcx)
{
  if (HAL_ADC_DeInit(&adcx->hadc) != HAL_OK)
  {
    Error_Handler();
  }
}
int bsp_adc_read(adc_t *adcx, uint16_t *buf, size_t count)
{
    if (count == 0)
    {
        return 0; // 如果没有要读取的数据，直接返回
    }
    // 确保 ADC 处于停止状态
    HAL_ADC_Stop(&adcx->hadc);

    if (HAL_ADCEx_Calibration_Start(&adcx->hadc, ADC_SINGLE_ENDED) != HAL_OK)
    {
        // 错误处理
        return -1;
    }
    // 等待校准完成
    while (HAL_ADC_GetState(&adcx->hadc) & HAL_ADC_STATE_BUSY)
    {
        // 等待
    }

    // 启动单次转换
    if (HAL_ADC_Start(&adcx->hadc) != HAL_OK)
    {
        Error_Handler();
        return -1; // 启动转换失败
    }

    // 循环进行多次采样
    for (size_t i = 0; i < count; i++)
    {
        // 等待转换完成
        if (HAL_ADC_PollForConversion(&adcx->hadc, HAL_MAX_DELAY) != HAL_OK)
        {
            Error_Handler();
            HAL_ADC_Stop(&adcx->hadc); // 停止ADC转换
            return -1; // 转换超时或失败
        }

        // 读取转换结果
        uint16_t adc_value = HAL_ADC_GetValue(&adcx->hadc);
        // 将ADC值转换为电压值
        float voltage = (float)adc_value / 4095.0f * 3.3f;
        
        buf[i] = (uint16_t)(voltage * 1000); // 以毫伏为单位存储



    }

    // 停止ADC转换
    if (HAL_ADC_Stop(&adcx->hadc) != HAL_OK)
    {
        Error_Handler();
        return -1; // 停止转换失败
    }

    return count; // 返回读取的样本数
}

/* USER CODE END 1 */
