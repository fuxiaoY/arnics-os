/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
#include "../../arnics-os/drivers/bsp/bsp_def.h"
extern void bsp_gpio_init(io_t *io);
extern void bsp_gpio_irq_disable(io_t *io);
extern void bsp_gpio_free_cfg(io_t *io,uint32_t gpio_mode,uint32_t gpio_pull,uint32_t gpio_speed,uint8_t gpio_PinState);
extern void bsp_gpio_close(io_t *io);
extern void bsp_gpio_set(io_t *io,unsigned int statue);
extern GPIO_PinState bsp_gpio_get(io_t *io);
extern void bsp_gpio_toggle(io_t *io);
extern void bsp_gpio_irq(io_t *io);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

