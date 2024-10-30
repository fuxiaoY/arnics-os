

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


/* define -------------------------------------------------------------*/

/* declaration --------------------------------------------------------*/
// 声明设备参数表
extern const param_map_t uart_param_map[];
extern const size_t uart_param_map_size ;

extern const param_map_t io_param_map[];
extern const size_t io_param_map_size ;
extern const dev_operations io_ops;
/* declaration from outside--------------------------------------------*/
// 声明设备映射表
extern const device_map_t device_map[];
extern const size_t device_type_maps_size;



 
#ifdef __cplusplus
}
#endif
#endif
