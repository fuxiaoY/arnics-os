/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
        * Free pins are configured automatically as Analog (this feature is enabled through
        * the Code Generation settings)
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();



  /*Configure GPIO pins : PC13 PC0 PC1 PC2
                           PC3 PC4 PC5 PC6
                           PC7 PC8 PC9 PC10
                           PC11 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2
                          |GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 PA5 PA6 PA7
                           PA11 PA12 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB10
                           PB11 PB12 PB13 PB14
                           PB15 PB4 PB5 PB6
                           PB7 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

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
  else 
  {
      // 处理未知的 GPIO 端口
  }

  HAL_GPIO_Init(io->GPIOx, &io->GPIO_InitStruct);

  HAL_GPIO_WritePin(io->GPIOx, io->GPIO_InitStruct.Pin, io->PinState);

}
void bsp_gpio_close(io_t *io)
{
  HAL_GPIO_Init(io->GPIOx, &io->GPIO_InitStruct);

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
