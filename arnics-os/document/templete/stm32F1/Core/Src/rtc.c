/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
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
#include "rtc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    HAL_PWR_EnableBkUpAccess();
    /* Enable BKP CLK enable for backup registers */
    __HAL_RCC_BKP_CLK_ENABLE();
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void bsp_rtc_init(rtc_t *dev)
{
  if (HAL_RTC_Init(&dev->hrtc) != HAL_OK)
  {
    Error_Handler();
  }
}
void bsp_rtc_deinit(rtc_t *dev)
{
  HAL_RTC_MspDeInit(&dev->hrtc);
}

int bsp_rtc_get_datetime(rtc_t *dev,rtcTimeDateTypeDef_t* dt)
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;

    // Get the current time and date from the RTC
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

    // Convert BCD values to binary
    dt->Year = sDate.Year;
    dt->Month = sDate.Month;
    dt->Date = sDate.Date;
    dt->Week = sDate.WeekDay; // Assuming WeekDay is also in BIN format

    dt->Hour = sTime.Hours;
    dt->Minute = sTime.Minutes;
    dt->Second = sTime.Seconds;


    // Return a success code
    return 0; // Assuming 0 represents success
}
int bsp_rtc_set_datetime(rtc_t *dev,rtcTimeDateTypeDef_t* dt)
{
  // Set the time and date on the RTC
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  sTime.Hours = dt->Hour;
  sTime.Minutes = dt->Minute;
  sTime.Seconds = dt->Second;

  
  sDate.Year = dt->Year;
  sDate.Month = dt->Month;
  sDate.Date = dt->Date;
  sDate.WeekDay = dt->Week;

  // Set the time and date on the RTC

    // 设置 RTC 时间
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }

  // 设置 RTC 日期
  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  return 0;
}
/* USER CODE END 1 */
