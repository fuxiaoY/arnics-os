/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;




void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(uartHandle->Instance==LPUART1)
  {
  /* USER CODE BEGIN LPUART1_MspInit 0 */

  /* USER CODE END LPUART1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
    PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* LPUART1 clock enable */
    __HAL_RCC_LPUART1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**LPUART1 GPIO Configuration
    PB10     ------> LPUART1_RX
    PB11    ------> LPUART1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    __HAL_UART_ENABLE_IT(uartHandle, UART_IT_RXNE);
    /* LPUART1 interrupt Init */
    HAL_NVIC_SetPriority(LPUART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(LPUART1_IRQn);
  /* USER CODE BEGIN LPUART1_MspInit 1 */

  /* USER CODE END LPUART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
    PeriphClkInit.Usart1ClockSelection = RCC_USART2CLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART1 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{
  if(uartHandle->Instance==LPUART1)
  {
  /* USER CODE BEGIN LPUART1_MspDeInit 0 */

  /* USER CODE END LPUART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LPUART1_CLK_DISABLE();

    /**LPUART1 GPIO Configuration
    PB10     ------> LPUART1_RX
    PB11     ------> LPUART1_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);
    /* LPUART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(LPUART1_IRQn);

  /* USER CODE END LPUART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);


    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA2     ------> USART1_TX
    PA3     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
// .c file
void bsp_usart_init(uart_t *dev)
{
  if (HAL_UART_Init(&dev->huart) != HAL_OK)
  {
    Error_Handler();
  }
  __HAL_UART_ENABLE_IT(&dev->huart,UART_IT_RXNE);
}
void bsp_usart_close(uart_t *dev)
{
  HAL_UART_DeInit(&dev->huart);
}
void bsp_uart_IT_enable(uart_t *dev)
{
  __HAL_UART_ENABLE_IT(&dev->huart, UART_IT_TXE);
}

void bsp_usart_write(uart_t *dev, void *buf, size_t count)
{
  HAL_UART_Transmit(&dev->huart, (uint8_t *)buf, count, 10000);  // 10s 超时时间
}
void bsp_usart_read(uart_t *dev, void *buf, size_t count)
{
  HAL_UART_Receive(&dev->huart, (uint8_t *)buf, count, 10000);  // 10s 超时时间
}

int bsp_uart_dirct_send(uart_t *dev,uart_dirct_t *buffer)
{
  return HAL_UART_Transmit(&dev->huart, buffer->buf, buffer->size, 10000);  // 10s 超时时间
}

void bsp_uart_wakeup_set(uart_t *dev)
{
  UART_HandleTypeDef *huart = &dev->huart;
  // 清除唤醒事件标志位
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
  // 唤醒事件
  UART_WakeUpTypeDef UART_WakeUpStruct;
  UART_WakeUpStruct.WakeUpEvent = UART_WAKEUP_ON_READDATA_NONEMPTY;
  HAL_UARTEx_StopModeWakeUpSourceConfig(huart,UART_WakeUpStruct);
  // 使能从stop唤醒
  __HAL_UART_ENABLE_IT(huart, UART_IT_WUF);
  HAL_UARTEx_EnableClockStopMode(huart);
  HAL_UARTEx_EnableStopMode(huart);
}
void bsp_uart_wakeup_clear(uart_t *dev)
{
  UART_HandleTypeDef *huart = &dev->huart;
  __HAL_PWR_CLEAR_FLAG(UART_IT_WUF);
  __HAL_UART_DISABLE_IT(huart, UART_IT_WUF);
  __HAL_RCC_LPUART1_CLK_ENABLE();
  HAL_UARTEx_DisableStopMode(huart);
  HAL_UARTEx_DisableClockStopMode(huart);
}

void bsp_uart_irq(uart_t *dev)
{
  HAL_UART_IRQHandler(&dev->huart);
}


void bsp_uart_irq_ring(uart_t *dev)
{
  unsigned char data;

  if (__HAL_UART_GET_FLAG(&dev->huart, UART_FLAG_RXNE)) // 接收正确
  {
    HAL_UART_Receive(&dev->huart, &data, 1, 0);
    ring_buf_put(dev->ring_rx, &data, 1);
  }

  if (__HAL_UART_GET_FLAG(&dev->huart, UART_FLAG_TXE)) // 发送缓冲区已空
  {
    if (ring_buf_get(dev->ring_tx, &data, 1))
    {
      HAL_UART_Transmit(&dev->huart, &data, 1, 0);
    }
    else
    {
      __HAL_UART_DISABLE_IT(&dev->huart, UART_IT_TXE);
    }
  }

  if (__HAL_UART_GET_FLAG(&dev->huart, UART_FLAG_IDLE))
  {
    __HAL_UART_CLEAR_IDLEFLAG(&dev->huart); // 清IDLE标志
  }

  /*异常处理 ----------------------------------- ----------------------------*/
  if (__HAL_UART_GET_FLAG(&dev->huart, UART_FLAG_ORE)) // 过载错误
  {
    HAL_UART_Receive(&dev->huart, &data, 1, 0);
    __HAL_UART_CLEAR_OREFLAG(&dev->huart);
  }
}
/* USER CODE END 1 */
