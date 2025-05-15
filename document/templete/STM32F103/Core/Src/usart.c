/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */


/* USART1 init function */

void MX_USART1_UART_Init(void)
{


}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
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
