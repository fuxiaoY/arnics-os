/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32l4xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "main.h"
#include "stm32l4xx_it.h"
#include "FreeRTOS.h"
#include "task.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "include.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_spi2_rx;
extern DMA_HandleTypeDef hdma_spi2_tx;
extern TIM_HandleTypeDef htim7;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */

void LPUART1_IRQHandler(void)
{
  dev_irq(&debug_ds);
}

void RTC_Alarm_IRQHandler(void) 
{
  dev_irq(&rtc_ds);
}

void RTC_WKUP_IRQHandler(void)
{
  dev_irq(&rtc_ds);
}

void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
  arnics_systick_handler();
  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
#if (INCLUDE_xTaskGetSchedulerState == 1 )
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
#endif /* INCLUDE_xTaskGetSchedulerState */
  xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState == 1 )
  }
#endif /* INCLUDE_xTaskGetSchedulerState */
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32L4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel4 global interrupt.
  */
void DMA1_Channel4_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel4_IRQn 0 */

  /* USER CODE END DMA1_Channel4_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi2_rx);
  /* USER CODE BEGIN DMA1_Channel4_IRQn 1 */

  /* USER CODE END DMA1_Channel4_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel5 global interrupt.
  */
void DMA1_Channel5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel5_IRQn 0 */

  /* USER CODE END DMA1_Channel5_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi2_tx);
  /* USER CODE BEGIN DMA1_Channel5_IRQn 1 */

  /* USER CODE END DMA1_Channel5_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  dev_irq(&debug_ds);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  dev_irq(&wan_uart_ds);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}
/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */

  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}

/**
  * @brief This function handles DMA2 channel6 global interrupt.
  */
void DMA2_Channel6_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Channel6_IRQn 0 */

  /* USER CODE END DMA2_Channel6_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_tx);
  /* USER CODE BEGIN DMA2_Channel6_IRQn 1 */

  /* USER CODE END DMA2_Channel6_IRQn 1 */
}

/**
  * @brief This function handles DMA2 channel7 global interrupt.
  */
void DMA2_Channel7_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Channel7_IRQn 0 */

  /* USER CODE END DMA2_Channel7_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
  /* USER CODE BEGIN DMA2_Channel7_IRQn 1 */

  /* USER CODE END DMA2_Channel7_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @brief This function handles EXTI Line0 interrupt.
  */
 void EXTI0_IRQHandler(void)
 {
   /* USER CODE BEGIN EXTI0_IRQn 0 */
 
   /* USER CODE END EXTI0_IRQn 0 */
   HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
   /* USER CODE BEGIN EXTI0_IRQn 1 */
 
   /* USER CODE END EXTI0_IRQn 1 */
 }
 
 /**
   * @brief This function handles EXTI Line1 interrupt.
   */
 void EXTI1_IRQHandler(void)
 {
   /* USER CODE BEGIN EXTI1_IRQn 0 */
 
   /* USER CODE END EXTI1_IRQn 0 */
   HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
   /* USER CODE BEGIN EXTI1_IRQn 1 */
 
   /* USER CODE END EXTI1_IRQn 1 */
 }
 
 /**
   * @brief This function handles EXTI Line2 interrupt.
   */
 void EXTI2_IRQHandler(void)
 {
   /* USER CODE BEGIN EXTI2_IRQn 0 */
 
   /* USER CODE END EXTI2_IRQn 0 */
   HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
   /* USER CODE BEGIN EXTI2_IRQn 1 */
 
   /* USER CODE END EXTI2_IRQn 1 */
 }
 
 /**
   * @brief This function handles EXTI Line3 interrupt.
   */
 void EXTI3_IRQHandler(void)
 {
   /* USER CODE BEGIN EXTI3_IRQn 0 */
 
   /* USER CODE END EXTI3_IRQn 0 */
   HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
   /* USER CODE BEGIN EXTI3_IRQn 1 */
 
   /* USER CODE END EXTI3_IRQn 1 */
 }
 
 /**
   * @brief This function handles EXTI Line4 interrupt.
   */
 void EXTI4_IRQHandler(void)
 {
   /* USER CODE BEGIN EXTI4_IRQn 0 */
 
   /* USER CODE END EXTI4_IRQn 0 */
   HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
   /* USER CODE BEGIN EXTI4_IRQn 1 */
 
   /* USER CODE END EXTI4_IRQn 1 */
 }
 
 /**
   * @brief This function handles EXTI Line[9:5] interrupts.
   */
 void EXTI9_5_IRQHandler(void)
 {
   /* USER CODE BEGIN EXTI9_5_IRQn 0 */
 
   /* USER CODE END EXTI9_5_IRQn 0 */
   HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
   HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
   HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
   HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
   HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
   /* USER CODE BEGIN EXTI9_5_IRQn 1 */
 
   /* USER CODE END EXTI9_5_IRQn 1 */
 }
 
 /**
   * @brief This function handles EXTI Line[15:10] interrupts.
   */
 void EXTI15_10_IRQHandler(void)
 {
   /* USER CODE BEGIN EXTI15_10_IRQn 0 */
 
   /* USER CODE END EXTI15_10_IRQn 0 */
   HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
   HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
   HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
   HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
   HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
   HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
   /* USER CODE BEGIN EXTI15_10_IRQn 1 */
 
   /* USER CODE END EXTI15_10_IRQn 1 */
 } 
