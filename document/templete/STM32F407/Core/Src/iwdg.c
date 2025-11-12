/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    iwdg.c
  * @brief   This file provides code for the configuration
  *          of the IWDG instances.
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
#include "iwdg.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

IWDG_HandleTypeDef hiwdg;

/* IWDG init function */
void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_4;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/* USER CODE BEGIN 1 */
void bsp_iwdg_init(iwdg_t *dev)
{
  if (HAL_IWDG_Init(&dev->hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
}

void bsp_iwdg_deinit(iwdg_t *dev)
{
    if (dev != NULL) {
        // 关闭独立看门狗
        IWDG_DISABLE_WRITE_ACCESS(&dev->hiwdg);
    }
}

void bsp_iwdg_refresh(iwdg_t *dev)
{
    if (dev != NULL) {
        // 喂狗操作
        __HAL_IWDG_RELOAD_COUNTER(&dev->hiwdg);
    }
}

/* USER CODE END 1 */
