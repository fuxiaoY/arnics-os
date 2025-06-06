/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rng.h
  * @brief   This file contains all the function prototypes for
  *          the rng.c file
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
#ifndef __RNG_H__
#define __RNG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */


#include "../../arnics-os/drivers/bsp/bsp_def.h"
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_RNG_Init(void);
extern void bsp_rng_init(rng_t *dev);
extern void bsp_rng_deinit(rng_t *dev);
extern int bsp_rng_read(rng_t *dev, uint32_t *buf, size_t count);
/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __RNG_H__ */

