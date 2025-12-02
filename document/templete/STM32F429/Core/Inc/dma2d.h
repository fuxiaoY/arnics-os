/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dma2d.h
  * @brief   This file contains all the function prototypes for
  *          the dma2d.c file
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
#ifndef __DMA2D_H__
#define __DMA2D_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "Inc/typedef.h"
#include "main.h"
#include "../../arnics-os/drivers/bsp/bsp_def.h"


/* USER CODE BEGIN Private defines */
extern int bsp_dma2d_init(dma2d_t *dma2d);
extern int bsp_dma2d_close(dma2d_t *dma2d);
extern int bsp_dma2d_fill_buffer(dma2d_t *dma2d,
                                uint32_t xPos,  uint32_t yPos, 
                                uint32_t xSize, uint32_t ySize, 
                                uint32_t color,
                                size_t Timeout);
extern int bsp_dma2d_copy_buffer(dma2d_t *dma2d,
                                uint32_t *pSrc, 
                                uint32_t xPos,  uint32_t yPos,
                                uint32_t xSize, uint32_t ySize,
                                size_t Timeout);
extern int bsp_dma2d_free_copy_buffer(dma2d_t *dma2d,
                                  uint32_t *pSrc, 
                                  uint32_t xPos,  uint32_t yPos,
                                  uint32_t xSize, uint32_t ySize,
                                  size_t Timeout);
extern int bsp_dma2d_blending(dma2d_t *dma2d,
                                uint16_t *background_buffer, 
                                uint16_t *foreground_buffer, 
                                uint32_t xPos,  uint32_t yPos,
                                uint32_t xSize, uint32_t ySize,
                                size_t Timeout);
#ifdef __cplusplus
}
#endif

#endif /* __DMA2D_H__ */

