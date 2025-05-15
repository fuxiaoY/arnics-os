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

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
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

    // /* USART1 DMA Init */
    // /* USART1_RX Init */
    // hdma_usart1_rx.Instance = DMA2_Channel7;
    // hdma_usart1_rx.Init.Request = DMA_REQUEST_2;
    // hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    // hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    // hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    // hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    // hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    // hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    // hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    // if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    // {
    //   Error_Handler();
    // }

    // __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    // /* USART1_TX Init */
    // hdma_usart1_tx.Instance = DMA2_Channel6;
    // hdma_usart1_tx.Init.Request = DMA_REQUEST_2;
    // hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    // hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    // hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    // hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    // hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    // hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    // hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    // if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    // {
    //   Error_Handler();
    // }

    // __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
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

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
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
