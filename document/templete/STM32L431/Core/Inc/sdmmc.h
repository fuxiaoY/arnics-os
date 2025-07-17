/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    sdmmc.h
  * @brief   This file contains all the function prototypes for
  *          the sdmmc.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#ifndef __SDMMC_H__
#define __SDMMC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern SD_HandleTypeDef hsd1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_SDMMC1_SD_Init(void);

/* USER CODE BEGIN Prototypes */
#include "../../arnics-os/drivers/bsp/bsp_def.h"
extern int bsp_sdmmc_init(sdmmc_t *dev);
extern void bsp_sdmmc_close(sdmmc_t *dev);
extern int bsp_sdmmc_write(sdmmc_t *dev,uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout);
extern int bsp_sdmmc_read(sdmmc_t *dev, uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout);
extern int bsp_sdmmc_erase(sdmmc_t *dev,uint32_t StartAddr, uint32_t EndAddr);
extern int bsp_sdmmc_get_card_state(sdmmc_t *dev);
extern void bsp_sdmmc_get_card_info(sdmmc_t *dev,sdCardInfo_t *CardInfo);
/* callback-----------------------------------------*/
extern void HAL_SD_AbortCallback(SD_HandleTypeDef *hsd);
extern void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd);
extern void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd);
extern void BSP_SD_AbortCallback(void);
extern void BSP_SD_WriteCpltCallback(void);
extern void BSP_SD_ReadCpltCallback(void);
extern int bsp_is_detected(void);


#ifdef __cplusplus
}
#endif

#endif /* __SDMMC_H__ */

