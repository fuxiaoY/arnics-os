/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
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
#include "rtc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 1;
  sDate.Year = 0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

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
  HAL_RTC_DeInit(&dev->hrtc);
}

int bsp_rtc_get_datetime(rtc_t *dev,rtcTimeDateTypeDef_t* dt)
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;

    // Get the current time and date from the RTC
    HAL_RTC_GetTime(&dev->hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&dev->hrtc, &sDate, RTC_FORMAT_BIN);

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
  if (HAL_RTC_SetTime(&dev->hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }

  // 设置 RTC 日期
  if (HAL_RTC_SetDate(&dev->hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  return 0;
}
static uint32_t last_call_time = 0; // 上一次调用函数的时间，单位为秒

bool bsp_rtc_check_second_update(rtc_t *dev) 
{
    rtcTimeDateTypeDef_t current_time;

    // 获取当前日期和时间
    bsp_rtc_get_datetime(dev, &current_time);
    
    // 计算当前时间的总秒数
    uint32_t current_time_in_sec = current_time.Hour * 3600 + 
                                    current_time.Minute * 60 + 
                                    current_time.Second;

    // 检查和上一次调用值是否不相等
    if (current_time_in_sec != last_call_time) 
    {
        last_call_time = current_time_in_sec; // 更新上一次调用时间
        return true; // 表示已经过去了一秒
    }
    
    return false; // 表示没有过去一秒
}

void bsp_rtc_alarm_set(rtc_t *dev, uint32_t sleep_ms)
{
  	RTC_HandleTypeDef *hrtc = &dev->hrtc;
    uint32_t time_s = sleep_ms / 1000;
    // alarm
    RTC_AlarmTypeDef sAlarm = {0};
    RTC_TimeTypeDef sTime;

	// 时钟溢出处理
    HAL_RTC_GetTime(hrtc, &sTime, RTC_FORMAT_BIN);

    uint8_t second = (sTime.Seconds + time_s) % 60;
    uint8_t minite = ((sTime.Seconds + time_s) / 60) % 60;
    uint8_t hour = (((sTime.Seconds + time_s) / 60) / 60) % 24;

    sAlarm.AlarmTime.Hours = sTime.Hours + hour;
    sAlarm.AlarmTime.Minutes = sTime.Minutes + minite;
    sAlarm.AlarmTime.Seconds = sTime.Seconds + second;
    sAlarm.AlarmTime.SubSeconds = 0;
     // 掩码设置（全匹配）
    sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
    sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
    sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
    // 1（忽略日期匹配）
    sAlarm.AlarmDateWeekDay = 1;  
    // 使用AlarmA         
    sAlarm.Alarm = RTC_ALARM_A;            
    HAL_RTC_SetAlarm_IT(hrtc, &sAlarm, RTC_FORMAT_BIN);

	  HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}
void bsp_rtc_alarm_clear(rtc_t *dev)
{
	HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);
}

void bsp_rtc_wakeup_set(rtc_t *dev, uint32_t sleep_ms)
{
    RTC_HandleTypeDef *hrtc = &dev->hrtc;
    uint32_t interval_ms = sleep_ms;

    // 计算Wakeup计数器值 2048Hz
    uint32_t wakeup_clock = RTC_WAKEUPCLOCK_RTCCLK_DIV16; // 2048Hz
    uint32_t wakeup_value = (interval_ms * 2048) / 1000;
    HAL_RTCEx_SetWakeUpTimer_IT(hrtc, wakeup_value, wakeup_clock);

    HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}
void bsp_rtc_wakeup_clear(rtc_t *dev)
{
  // 禁用Wakeup定时器
  HAL_RTCEx_DeactivateWakeUpTimer(&dev->hrtc);
	HAL_NVIC_DisableIRQ(RTC_WKUP_IRQn);
}

void bsp_rtc_irq(rtc_t *dev) 
{
  HAL_RTC_AlarmIRQHandler(&dev->hrtc);
  HAL_RTCEx_WakeUpTimerIRQHandler(&dev->hrtc);
}
