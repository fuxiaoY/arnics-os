/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
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

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
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
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PA0-WKUP     ------> ADC1_IN0
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PA0-WKUP     ------> ADC1_IN0
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);

  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE BEGIN 1 */
//.C
void bsp_adc_init(adc_t *adcx)
{
  if (HAL_ADC_Init(&adcx->hadc) != HAL_OK)
  {
    Error_Handler();
  }
  for(uint8_t i = 0; i < adcx->channel_num; i++)
  {
    if (HAL_ADC_ConfigChannel(&adcx->hadc, &(adcx->sConfig_p[i])) != HAL_OK)
    {
      Error_Handler();
    }
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
    for(uint8_t i = 0; i < count; i++)
    {
        if (HAL_ADC_PollForConversion(&adcx->hadc, HAL_MAX_DELAY) != HAL_OK)
        {
          HAL_ADC_Stop(&adcx->hadc); // 停止ADC转  换
          return -1; // 转换超时或失败
        }
        // 读取转换结果
        uint16_t adc_value = HAL_ADC_GetValue(&adcx->hadc);
        buf[i] = adc_value;
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
