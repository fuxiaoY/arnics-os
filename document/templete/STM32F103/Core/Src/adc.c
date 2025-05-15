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

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE; // 不启用扫描模式
  hadc1.Init.ContinuousConvMode = DISABLE;    // 不启用连续转换模式
  hadc1.Init.DiscontinuousConvMode = DISABLE; // 不启用不连续转换模式
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START; // 软件触发启动转换
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;      // 数据右对齐
  hadc1.Init.NbrOfConversion = 1;                  // 转换次数为1
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
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

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PC0     ------> ADC1_IN10
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

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
    PC0     ------> ADC1_IN10
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0);

  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
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
    // 启动自动校准
    if (HAL_ADCEx_Calibration_Start(&adcx->hadc) != HAL_OK)
    {
        Error_Handler();
        return -1; // 校准失败
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
        float voltage = (float)adc_value / 4095.0 * 3.3;
        
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
