
/* information */
/**
  ******************************************************************************
  * @file           : bsp_def.h
  * @brief          : ST系列驱动结构体
  * 
  * 该文件定义驱动相关结构体
  * 涉及到具体芯片类型，需要改变hal库头文件
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
#ifndef _BSP_DEF_H_
#define _BSP_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../../Inc/ProjDefine.h"
#include "../../Inc/typedef.h"
#include "../common/drivers_common.h"


#ifdef STM32CHIP
/* include -----------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "usart.h"

/* typedef -----------------------------------------------------------*/
// I/O 结构体
typedef struct
{
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin;
    uint32_t GPIO_Mode;
    uint32_t GPIO_Pull;
    uint32_t GPIO_Speed;
    GPIO_PinState PinState;
} io_t;
// IO 参数映射表的 X-macro
#define IO_PARAM_MAP_X \
    X("GPIO_Pin", io_t, GPIO_Pin, uint16_t) \
    X("GPIO_Mode", io_t, GPIO_Mode, uint32_t) \
    X("GPIO_Pull", io_t, GPIO_Pull, uint32_t) \
    X("GPIO_Speed", io_t, GPIO_Speed, uint32_t) \
    X("PinState", io_t, PinState, GPIO_PinState)


typedef struct 
{
    UART_HandleTypeDef *huart;
    int baudrate; 
    int data_bits; 
    int stop_bits;
    int parity; 
    bool dma_mode;
    /* data */
}uart_t;
// UART 参数映射表的 X-macro
#define UART_PARAM_MAP_X \
    X("huart", uart_t, huart, UART_HandleTypeDef *) \
    X("baudrate", uart_t, baudrate, int) \
    X("data_bits", uart_t, data_bits, int) \
    X("stop_bits", uart_t, stop_bits, int) \
    X("parity", uart_t, parity, int) \
    X("dma_mode", uart_t, dma_mode, bool)
#else


#endif

#ifdef __cplusplus
}
#endif
#endif
