/* information */
/**
  ******************************************************************************
  * @file           : USRMALLOC_H
  * @brief          : 自定义内存分配头文件
  * 
  * 该文件定义了自定义内存分配和释放的宏定义。
  * 主要用于在不同环境下选择合适的内存管理函数。
  * 
  * @version        : 1.0.0
  * @date           : 2023-10-01
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
#ifndef _ARNICSMALLOC_H_
#define _ARNICSMALLOC_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------*/
#include "Inc/projDefine.h"      
#include "Inc/typedef.h"         

/* macro -------------------------------------------------------------*/
/**
 * @def arnicsFree
 * @brief 自定义内存释放函数
 * 
 * 如果定义了 PLATFORM_MCU，则使用 FreeRTOS 的 vPortFree 函数，
 * 否则使用标准库的 free 函数。
 */
#if PLATFORM_MCU
#include "FreeRTOS.h"
#define arnicsFree vPortFree
#else
#define arnicsFree free
#endif

/**
 * @def arnicsMalloc
 * @brief 自定义内存分配函数
 * 
 * 如果定义了 PLATFORM_MCU，则使用 FreeRTOS 的 pvPortMalloc 函数，
 * 否则使用标准库的 malloc 函数。
 */
#if PLATFORM_MCU
#define arnicsMalloc pvPortMalloc
#else
#define arnicsMalloc malloc
#endif

#ifdef __cplusplus
}
#endif

#endif // _ARNICSMALLOC_H_
/************************ (C) COPYRIGHT ARSTUDIO *****END OF FILE***************************/
