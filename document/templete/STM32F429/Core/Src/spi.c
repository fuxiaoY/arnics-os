/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
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
#include "spi.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI5)
  {
  /* USER CODE BEGIN SPI5_MspInit 0 */

  /* USER CODE END SPI5_MspInit 0 */
    /* SPI5 clock enable */
    __HAL_RCC_SPI5_CLK_ENABLE();

    __HAL_RCC_GPIOF_CLK_ENABLE();
    /**SPI5 GPIO Configuration
    PF7     ------> SPI5_SCK
    PF8     ------> SPI5_MISO
    PF9     ------> SPI5_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI5_MspInit 1 */

  /* USER CODE END SPI5_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI5)
  {
  /* USER CODE BEGIN SPI5_MspDeInit 0 */

  /* USER CODE END SPI5_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI5_CLK_DISABLE();

    /**SPI5 GPIO Configuration
    PF7     ------> SPI5_SCK
    PF8     ------> SPI5_MISO
    PF9     ------> SPI5_MOSI
    */
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9);

  /* USER CODE BEGIN SPI5_MspDeInit 1 */

  /* USER CODE END SPI5_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */


void bsp_spi_init(spi_t *dev)
{

  if (HAL_SPI_Init(&dev->hspi) != HAL_OK)
  {
    Error_Handler();
  }
}

void bsp_spi_close(spi_t *dev)
{
  HAL_SPI_DeInit(&dev->hspi);
}

int bsp_spi_read(spi_t *dev, void *buf, size_t count)
{
  return HAL_SPI_Receive(&dev->hspi, (uint8_t*)buf, count, 1000);
}

int bsp_spi_write(spi_t *dev, void *buf, size_t count)
{
  return HAL_SPI_Transmit(&dev->hspi, (uint8_t*)buf, count, 1000);
}
// HAL_SPI_STATE_RESET      = 0x00U,    /*!< Peripheral not Initialized                         */
// HAL_SPI_STATE_READY      = 0x01U,    /*!< Peripheral Initialized and ready for use           */
// HAL_SPI_STATE_BUSY       = 0x02U,    /*!< an internal process is ongoing                     */
// HAL_SPI_STATE_BUSY_TX    = 0x03U,    /*!< Data Transmission process is ongoing               */
// HAL_SPI_STATE_BUSY_RX    = 0x04U,    /*!< Data Reception process is ongoing                  */
// HAL_SPI_STATE_BUSY_TX_RX = 0x05U,    /*!< Data Transmission and Reception process is ongoing */
// HAL_SPI_STATE_ERROR      = 0x06U,    /*!< SPI error state                                    */
// HAL_SPI_STATE_ABORT      = 0x07U     /*!< SPI abort is ongoing                               */
int bsp_spi_state_get(spi_t *dev)
{
  HAL_SPI_StateTypeDef result = HAL_SPI_GetState(&dev->hspi);
  switch (result) 
  {
    case HAL_SPI_STATE_RESET:
      return -1;
    case HAL_SPI_STATE_READY:
      return 0;
    case HAL_SPI_STATE_BUSY:
      return -2;
    case HAL_SPI_STATE_BUSY_TX:
      return -3;
    case HAL_SPI_STATE_BUSY_RX:
      return -4; 
    case HAL_SPI_STATE_BUSY_TX_RX:
      return -5;
    case HAL_SPI_STATE_ERROR:
      return -6;
    case  HAL_SPI_STATE_ABORT:
      return -7;
    default:
      return -8;
  }
}

/* USER CODE END 1 */
