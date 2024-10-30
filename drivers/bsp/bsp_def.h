
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
#include "../thirdParty/thirdPartyInclude.h"

#ifdef STM32CHIP
/* include -----------------------------------------------------------*/
#include "stm32f1xx_hal.h"
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


typedef enum 
{
  UART_DEBUG = 0,
  UART_WAN,
  UART_LC,
}UART_FUNCTION;

typedef struct 
{
    UART_HandleTypeDef huart;
    int baudrate; 
    int data_bits; 
    int stop_bits;
    int parity; 
    bool dma_mode;
    UART_FUNCTION function;
    ring_buf_t *ring_rx;
    ring_buf_t *ring_tx;
    unsigned char *rx_buf;
    unsigned char *tx_buf;
    unsigned int rx_buf_size;
    unsigned int tx_buf_size;
    /* data */
}uart_t;
// UART 参数映射表的 X-macro
#define UART_PARAM_MAP_X \
    X("huart", uart_t, huart, UART_HandleTypeDef) \
    X("baudrate", uart_t, baudrate, int) \
    X("data_bits", uart_t, data_bits, int) \
    X("stop_bits", uart_t, stop_bits, int) \
    X("parity", uart_t, parity, int) \
    X("dma_mode", uart_t, dma_mode, bool) \
    X("function", uart_t, function, UART_FUNCTION) \
    X("ring_rx", uart_t, ring_rx, ring_buf_t*) \
    X("ring_tx", uart_t, ring_tx, ring_buf_t*) \
    X("rx_buf", uart_t, rx_buf, unsigned char*) \
    X("tx_buf", uart_t, tx_buf, unsigned char*) \
    X("rx_buf_size", uart_t, rx_buf_size, unsigned int) \
    X("tx_buf_size", uart_t, tx_buf_size, unsigned int)
#else


#endif

#ifdef __cplusplus
}
#endif
#endif
