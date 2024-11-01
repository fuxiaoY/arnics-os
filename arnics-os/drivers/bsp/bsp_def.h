
/* information */
/**
  ******************************************************************************
  * @file           : bsp_def.h
  * @brief          : ST系列驱动结构体
  * 
  * 该文件定义驱动相关结构体
  * 涉及到具体芯片类型，需要改变hal库头文件
  * 向南接口：bsp各驱动文件请包含此头文件
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
#include "../port/arnicsPort.h"

/* typedef -----------------------------------------------------------*/
// I/O 结构体
typedef struct
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_TypeDef* GPIOx;
    GPIO_PinState PinState;
} io_t;
// IO 参数映射表的 X-macro
#define IO_PARAM_MAP_X \
    X("GPIOx", io_t, GPIOx, GPIO_TypeDef*) \
    X("GPIO_InitStruct", io_t, GPIO_InitStruct, GPIO_InitTypeDef) \
    X("Pin", io_t, GPIO_InitStruct.Pin, uint32_t) \
    X("Mode", io_t, GPIO_InitStruct.Mode, uint32_t) \
    X("Pull", io_t, GPIO_InitStruct.Pull, uint32_t) \
    X("Speed", io_t, GPIO_InitStruct.Speed, uint32_t) \
    X("PinState", io_t, PinState, uint32_t)

typedef struct 
{
    UART_HandleTypeDef huart;
    bool dma_mode;
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
    X("Instance", uart_t, huart.Instance, USART_TypeDef) \
    X("BaudRate", uart_t, huart.Init.BaudRate, uint32_t) \
    X("WordLength", uart_t, huart.Init.WordLength, uint32_t) \
    X("StopBits", uart_t, huart.Init.StopBits, uint32_t) \
    X("Parity", uart_t, huart.Init.Parity, uint32_t) \
    X("dma_mode", uart_t, dma_mode, bool) \
    X("ring_rx", uart_t, ring_rx, ring_buf_t*) \
    X("ring_tx", uart_t, ring_tx, ring_buf_t*) \
    X("rx_buf", uart_t, rx_buf, unsigned char*) \
    X("tx_buf", uart_t, tx_buf, unsigned char*) \
    X("rx_buf_size", uart_t, rx_buf_size, unsigned int) \
    X("tx_buf_size", uart_t, tx_buf_size, unsigned int)


typedef struct
{
  SPI_HandleTypeDef hspi;

  /* data */
}spi_t;
// SPI 参数映射表的 X-macro
#define SPI_PARAM_MAP_X \
    X("hspi", spi_t, hspi, SPI_HandleTypeDef) \
    X("Instance", spi_t, hspi.Instance, SPI_TypeDef) \
    X("Mode", spi_t, hspi.Init.Mode, uint32_t) \
    X("Direction", spi_t, hspi.Init.Direction, uint32_t) \
    X("DataSize", spi_t, hspi.Init.DataSize, uint32_t) \
    X("CLKPolarity", spi_t, hspi.Init.CLKPolarity, uint32_t) \
    X("CLKPhase", spi_t, hspi.Init.CLKPhase, uint32_t) \
    X("NSS", spi_t, hspi.Init.NSS, uint32_t) \
    X("BaudRatePrescaler", spi_t, hspi.Init.BaudRatePrescaler, uint32_t) \
    X("FirstBit", spi_t, hspi.Init.FirstBit, uint32_t) \
    X("TIMode", spi_t, hspi.Init.TIMode, uint32_t) \
    X("CRCCalculation", spi_t, hspi.Init.CRCCalculation, uint32_t) \
    X("CRCPolynomial", spi_t, hspi.Init.CRCPolynomial, uint32_t)

typedef struct 
{
    uint32_t offset;
    size_t size;
}flash_ctl_t;

typedef struct 
{
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t offset;
}flash_t;
// 内部flash 参数映射表的 X-macro
#define FLASH_PARAM_MAP_X \
    X("EraseInitStruct", flash_t, EraseInitStruct, FLASH_EraseInitTypeDef) \
    X("TypeErase", flash_t, EraseInitStruct.TypeErase, uint32_t) \
    X("Banks", flash_t, EraseInitStruct.Banks, uint32_t) \
    X("PageAddress", flash_t, EraseInitStruct.PageAddress, uint32_t) \
    X("NbPages", flash_t, EraseInitStruct.NbPages, uint32_t) \
    X("offset", flash_t, offset, uint32_t) 

#ifdef __cplusplus
}
#endif
#endif
