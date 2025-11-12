/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

/* USER CODE BEGIN 2 */

/* USER CODE BEGIN 2 */
void bsp_gpio_init(io_t *io)
{
  if (io->GPIOx == GPIOA) 
  {
      __HAL_RCC_GPIOA_CLK_ENABLE();
  } 
  else if (io->GPIOx == GPIOB) 
  {
      __HAL_RCC_GPIOB_CLK_ENABLE();
  } 
  else if (io->GPIOx == GPIOC) 
  {
      __HAL_RCC_GPIOC_CLK_ENABLE();
  } 
  else if (io->GPIOx == GPIOD) 
  {
      __HAL_RCC_GPIOD_CLK_ENABLE();
  } 
  else if (io->GPIOx == GPIOE) 
  {
      __HAL_RCC_GPIOE_CLK_ENABLE();
  } 
  else 
  {
      // ????? GPIO ??
  }
  HAL_GPIO_Init(io->GPIOx, &io->GPIO_InitStruct);

  // ????????????????
  if(io->GPIO_InitStruct.Mode == GPIO_MODE_OUTPUT_PP
    || io->GPIO_InitStruct.Mode == GPIO_MODE_OUTPUT_OD)
  {
      HAL_GPIO_WritePin(io->GPIOx, io->GPIO_InitStruct.Pin, io->PinState);
  }

  // ?????IO??
  if(io->GPIO_InitStruct.Mode == GPIO_MODE_IT_RISING
  || io->GPIO_InitStruct.Mode == GPIO_MODE_IT_FALLING
  || io->GPIO_InitStruct.Mode == GPIO_MODE_IT_RISING_FALLING)
  {
      if(io->GPIO_InitStruct.Pin == GPIO_PIN_0)
      {
          HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0);
          HAL_NVIC_EnableIRQ(EXTI0_IRQn);
      }
      else if(io->GPIO_InitStruct.Pin == GPIO_PIN_1)
      {
          HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0);
          HAL_NVIC_EnableIRQ(EXTI1_IRQn);
      }
      else if(io->GPIO_InitStruct.Pin == GPIO_PIN_2)
      {
          HAL_NVIC_SetPriority(EXTI2_IRQn, 5, 0);
          HAL_NVIC_EnableIRQ(EXTI2_IRQn);
      }
      else if(io->GPIO_InitStruct.Pin == GPIO_PIN_3)
      {
          HAL_NVIC_SetPriority(EXTI3_IRQn, 5, 0);
          HAL_NVIC_EnableIRQ(EXTI3_IRQn);
      }
      else if(io->GPIO_InitStruct.Pin == GPIO_PIN_4)
      {
          HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
          HAL_NVIC_EnableIRQ(EXTI4_IRQn);
      }
      else if(io->GPIO_InitStruct.Pin == GPIO_PIN_5
           || io->GPIO_InitStruct.Pin == GPIO_PIN_6
           || io->GPIO_InitStruct.Pin == GPIO_PIN_7
           || io->GPIO_InitStruct.Pin == GPIO_PIN_8
           || io->GPIO_InitStruct.Pin == GPIO_PIN_9)
      {
          HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
          HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
      }
      else if(io->GPIO_InitStruct.Pin == GPIO_PIN_10
           || io->GPIO_InitStruct.Pin == GPIO_PIN_11
           || io->GPIO_InitStruct.Pin == GPIO_PIN_12
           || io->GPIO_InitStruct.Pin == GPIO_PIN_13
           || io->GPIO_InitStruct.Pin == GPIO_PIN_14
           || io->GPIO_InitStruct.Pin == GPIO_PIN_15)
      {
          HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
          HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
      }
  }
}
void bsp_gpio_irq_disable(io_t *io)
{
    if(io->GPIO_InitStruct.Pin == GPIO_PIN_0)
    {
        HAL_NVIC_DisableIRQ(EXTI0_IRQn);
    }
    else if(io->GPIO_InitStruct.Pin == GPIO_PIN_1)
    {
        HAL_NVIC_DisableIRQ(EXTI1_IRQn);
    }
    else if(io->GPIO_InitStruct.Pin == GPIO_PIN_2)
    {
        HAL_NVIC_DisableIRQ(EXTI2_IRQn);
    }
    else if(io->GPIO_InitStruct.Pin == GPIO_PIN_3)
    {
        HAL_NVIC_DisableIRQ(EXTI3_IRQn);
    }
    else if(io->GPIO_InitStruct.Pin == GPIO_PIN_4)
    {
        HAL_NVIC_DisableIRQ(EXTI4_IRQn);
    }
    else if(io->GPIO_InitStruct.Pin == GPIO_PIN_5
          || io->GPIO_InitStruct.Pin == GPIO_PIN_6
          || io->GPIO_InitStruct.Pin == GPIO_PIN_7
          || io->GPIO_InitStruct.Pin == GPIO_PIN_8
          || io->GPIO_InitStruct.Pin == GPIO_PIN_9)
    {
        HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
    }
    else if(io->GPIO_InitStruct.Pin == GPIO_PIN_10
          || io->GPIO_InitStruct.Pin == GPIO_PIN_11
          || io->GPIO_InitStruct.Pin == GPIO_PIN_12
          || io->GPIO_InitStruct.Pin == GPIO_PIN_13
          || io->GPIO_InitStruct.Pin == GPIO_PIN_14
          || io->GPIO_InitStruct.Pin == GPIO_PIN_15)
    {
        HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
    }
}

void bsp_gpio_free_cfg(io_t *io,uint32_t gpio_mode,uint32_t gpio_pull,uint32_t gpio_speed,uint8_t gpio_PinState)
{
  io_t ioStruct = {0};

  ioStruct.GPIOx = io->GPIOx;
  ioStruct.GPIO_InitStruct.Pin = io->GPIO_InitStruct.Pin;
  ioStruct.GPIO_InitStruct.Mode = gpio_mode;
  ioStruct.GPIO_InitStruct.Pull = gpio_pull;
  ioStruct.GPIO_InitStruct.Speed = gpio_speed;
  ioStruct.PinState = (GPIO_PinState)gpio_PinState;
  bsp_gpio_init(&ioStruct);

}
void bsp_gpio_close(io_t *io)
{
  HAL_GPIO_DeInit(io->GPIOx, io->GPIO_InitStruct.Pin);

}
void bsp_gpio_set(io_t *io,unsigned int statue)
{
  HAL_GPIO_WritePin(io->GPIOx, io->GPIO_InitStruct.Pin, (GPIO_PinState)statue);
}
GPIO_PinState bsp_gpio_get(io_t *io)
{
  return HAL_GPIO_ReadPin(io->GPIOx, io->GPIO_InitStruct.Pin);
}

void bsp_gpio_toggle(io_t *io)
{
  HAL_GPIO_TogglePin(io->GPIOx, io->GPIO_InitStruct.Pin);
}
void bsp_gpio_irq(io_t *io)
{
  HAL_GPIO_EXTI_IRQHandler(io->GPIO_InitStruct.Pin);
}

/* USER CODE END 2 */
