/* information */
/**
  ******************************************************************************
  * @file           : dwt.h
  * @brief          : 内核计数器的延时实现
  * 
  * @version        : 1.0.0
  * @date           : 2025-11-12
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 fuxiaoy.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#ifndef __DWT_H__
#define __DWT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -----------------------------------------------------------------*/
#include "Inc/typedef.h"
#include "main.h"
#include "../../arnics-os/drivers/bsp/bsp_def.h"
/* function prototypes ------------------------------------------------------*/
/**
 * @brief 获取当前时间，单位为DWT CYCLE
 */
extern uint32_t dwt_time_get(void);

/**
 * @brief 清除DWT计数器
 */
extern void dwt_time_clear(void);
/**
 * @brief 获取经过的时间，单位us
 */
extern uint32_t dwt_get_elapsed_time_us(uint32_t start_cycle, uint32_t end_cycle);

/**
 * @brief bsp delay接口
 */
extern void bsp_delay_init(delay_t *delay);
extern void bsp_delay_deinit(delay_t *delay);
extern void bsp_delay_ms(delay_t *delay, uint32_t ms);
extern void bsp_delay_us(delay_t *delay,uint32_t us);

#ifdef __cplusplus
}
#endif
#endif
