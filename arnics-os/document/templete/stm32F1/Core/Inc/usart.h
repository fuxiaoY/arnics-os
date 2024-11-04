/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the functdevn prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attentdevn
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
/* Define to prevent recursive inclusdevn -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "../../arnics-os/drivers/bsp/bsp_def.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */



/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
extern void bsp_usart_init(uart_t *dev);
extern void bsp_usart_close(uart_t *dev);
extern void bsp_uart_IT_enable(uart_t *dev);
extern void bsp_usart_write(uart_t *dev, void *buf, size_t count);
extern void bsp_usart_read(uart_t *dev, void *buf, size_t count);
extern int bsp_uart_dirct_send(uart_t *dev,uart_dirct_t *buffer);
extern void bsp_uart_irq(uart_t *dev);
extern void bsp_uart_irq_ring(uart_t *dev);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

