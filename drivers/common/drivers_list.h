

/* information */
/**
  ******************************************************************************
  * @file           : _DRIVERS_LIST_H_
  * @brief          : 驱动列表
  * 
  * 该文件定义驱动抽象层列表，用于驱动管理器查找驱动
  * 
  * 
  * @version        : 1.0.0
  * @date           : 2020-10-01
  ******************************************************************************

  * @attention
  *
  * Copyright (c) 2023 ARSTUDIO.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************

  */
/* information */
#ifndef _DRIVERS_LIST_H_
#define _DRIVERS_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../../Inc/ProjDefine.h"
#include "../../Inc/typedef.h"
#include "../bsp/drivers_var.h"

struct device_t
{
    int ds;       /* 设备号 */
    void *device; /* 设备指针 */
    dev_operations *dev_ops; /* 设备操作函数 */
};
/* define -------------------------------------------------------------*/

/* declaration --------------------------------------------------------*/
// 声明设备参数表
#ifdef DRIVERS_ENABLE_UART
extern const param_map_t uart_param_map[];
extern const size_t uart_param_map_size ;
extern dev_operations uart_ops;
#endif
#ifdef DRIVERS_ENABLE_GPIO
extern const param_map_t io_param_map[];
extern const size_t io_param_map_size ;
extern dev_operations io_ops;
#endif
#ifdef DRIVERS_ENABLE_SPI
extern const param_map_t spi_param_map[];
extern const size_t spi_param_map_size ;
extern dev_operations spi_ops;
#endif
#ifdef DRIVERS_ENABLE_FLASH
extern const param_map_t flash_param_map[];
extern const size_t flash_param_map_size ;
extern dev_operations flash_ops;
#endif
#ifdef DRIVERS_ENABLE_IWDG
extern const param_map_t iwdg_param_map[];
extern const size_t iwdg_param_map_size ;
extern dev_operations iwdg_ops;
#endif
#ifdef DRIVERS_ENABLE_RTC
extern const param_map_t rtc_param_map[];
extern const size_t rtc_param_map_size ;
extern dev_operations rtc_ops;
#endif
#ifdef DRIVERS_ENABLE_ADC
extern const param_map_t adc_param_map[];
extern const size_t adc_param_map_size ;
extern dev_operations adc_ops;
#endif
#ifdef DRIVERS_ENABLE_IICSOF
extern const param_map_t iicsof_param_map[];
extern const size_t iicsof_param_map_size ;
extern dev_operations iicsof_ops;
#endif
#ifdef DRIVERS_ENABLE_RNG
extern const param_map_t rng_param_map[];
extern const size_t rng_param_map_size ;
extern dev_operations rng_ops;
#endif
/* declaration from outside--------------------------------------------*/
// 声明设备映射表
extern const device_map_t device_map[];
extern const size_t device_type_maps_size;



 
#ifdef __cplusplus
}
#endif
#endif
