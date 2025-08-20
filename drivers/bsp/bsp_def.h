
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
#ifdef MCU_STM32F1
#define DRIVERS_ENABLE_UART
#define DRIVERS_ENABLE_GPIO
#define DRIVERS_ENABLE_SPI
#define DRIVERS_ENABLE_FLASH
#define DRIVERS_ENABLE_IWDG
#define DRIVERS_ENABLE_RTC
#define DRIVERS_ENABLE_ADC
#define DRIVERS_ENABLE_IICSOF

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
    uint8_t *buf;
    size_t size;
}uart_dirct_t;
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


typedef struct
{
    IWDG_HandleTypeDef hiwdg;
  /* data */
}iwdg_t;
// 独立看门狗 参数映射表的 X-macro
#define IWDG_PARAM_MAP_X \
    X("hiwdg", iwdg_t, hiwdg, IWDG_HandleTypeDef) 


typedef struct
{       
  uint8_t Year;		  /*!<RTC 年*/
  uint8_t Month;		/*!<RTC 月*/
  uint8_t Date;		  /*!<RTC 日*/	
  uint8_t Hour;		  /*!<RTC 时*/
  uint8_t Minute;		/*!<RTC 分*/	
  uint8_t Second;		/*!<RTC 秒*/			
  uint8_t Week;		  /*!<RTC 周*/	
} rtcTimeDateTypeDef_t;

typedef struct
{
    RTC_HandleTypeDef hrtc;
    rtcTimeDateTypeDef_t rtcTimeDate;
  /* data */
}rtc_t;
// rtc 参数映射表的 X-macro
#define RTC_PARAM_MAP_X \
    X("hrtc", rtc_t, hrtc, RTC_HandleTypeDef) \
    X("rtcTimeDate", rtc_t, rtcTimeDate, rtcTimeDateTypeDef_t) \
    X("Year", rtc_t, rtcTimeDate.Year, uint8_t) \
    X("Month", rtc_t, rtcTimeDate.Month, uint8_t) \
    X("Date", rtc_t, rtcTimeDate.Date, uint8_t) \
    X("Hour", rtc_t, rtcTimeDate.Hour, uint8_t) \
    X("Minute", rtc_t, rtcTimeDate.Minute, uint8_t) \
    X("Second", rtc_t, rtcTimeDate.Second, uint8_t) \
    X("Week", rtc_t, rtcTimeDate.Week, uint8_t)

typedef struct 
{
    ADC_HandleTypeDef hadc;
    ADC_ChannelConfTypeDef sConfig;
}adc_t;

#define ADC_PARAM_MAP_X \
    X("hadc", adc_t, hadc, ADC_HandleTypeDef) \
    X("sConfig", adc_t, sConfig, ADC_ChannelConfTypeDef)




typedef struct 
{
    io_t SCL;
    io_t SDA_OUT;
    io_t SDA_IN;
}iicSof_t;

#define IICSOF_PARAM_MAP_X \
    X("SCL", iicSof_t, SCL, io_t) \
    X("SDA_OUT", iicSof_t, SDA_OUT, io_t) \
    X("SDA_IN", iicSof_t, SDA_IN, io_t)
#elif defined(MCU_STM32L4)
#define DRIVERS_ENABLE_UART
#define DRIVERS_ENABLE_GPIO
#define DRIVERS_ENABLE_SPI
#define DRIVERS_ENABLE_FLASH
#define DRIVERS_ENABLE_IWDG
#define DRIVERS_ENABLE_RTC
#define DRIVERS_ENABLE_ADC
#define DRIVERS_ENABLE_IICSOF
#define DRIVERS_ENABLE_RNG
#define DRIVERS_ENABLE_SDMMC
#define DRIVERS_ENABLE_TIM
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
    uint8_t *buf;
    size_t size;
}uart_dirct_t;
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
}flash_ctl_t; //仅擦除时使用

typedef struct 
{
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t offset; //仅写入时使用
}flash_t;
// 内部flash 参数映射表的 X-macro
#define FLASH_PARAM_MAP_X \
    X("EraseInitStruct", flash_t, EraseInitStruct, FLASH_EraseInitTypeDef) \
    X("TypeErase", flash_t, EraseInitStruct.TypeErase, uint32_t) \
    X("Banks", flash_t, EraseInitStruct.Banks, uint32_t) \
    X("offset", flash_t, offset, uint32_t) 


typedef struct
{
    IWDG_HandleTypeDef hiwdg;
  /* data */
}iwdg_t;
// 独立看门狗 参数映射表的 X-macro
#define IWDG_PARAM_MAP_X \
    X("hiwdg", iwdg_t, hiwdg, IWDG_HandleTypeDef) 


typedef struct
{       
  uint8_t Year;		  /*!<RTC 年*/
  uint8_t Month;		/*!<RTC 月*/
  uint8_t Date;		  /*!<RTC 日*/	
  uint8_t Hour;		  /*!<RTC 时*/
  uint8_t Minute;		/*!<RTC 分*/	
  uint8_t Second;		/*!<RTC 秒*/			
  uint8_t Week;		  /*!<RTC 周*/	
} rtcTimeDateTypeDef_t;

typedef struct
{
    RTC_HandleTypeDef hrtc;
    rtcTimeDateTypeDef_t rtcTimeDate;
  /* data */
}rtc_t;
// rtc 参数映射表的 X-macro
#define RTC_PARAM_MAP_X \
    X("hrtc", rtc_t, hrtc, RTC_HandleTypeDef) \
    X("rtcTimeDate", rtc_t, rtcTimeDate, rtcTimeDateTypeDef_t) \
    X("Year", rtc_t, rtcTimeDate.Year, uint8_t) \
    X("Month", rtc_t, rtcTimeDate.Month, uint8_t) \
    X("Date", rtc_t, rtcTimeDate.Date, uint8_t) \
    X("Hour", rtc_t, rtcTimeDate.Hour, uint8_t) \
    X("Minute", rtc_t, rtcTimeDate.Minute, uint8_t) \
    X("Second", rtc_t, rtcTimeDate.Second, uint8_t) \
    X("Week", rtc_t, rtcTimeDate.Week, uint8_t)

typedef struct 
{
    ADC_HandleTypeDef hadc;
    ADC_ChannelConfTypeDef sConfig;
}adc_t;

#define ADC_PARAM_MAP_X \
    X("hadc", adc_t, hadc, ADC_HandleTypeDef) \
    X("sConfig", adc_t, sConfig, ADC_ChannelConfTypeDef)




typedef struct 
{
    io_t SCL;
    io_t SDA_OUT;
    io_t SDA_IN;
}iicSof_t;

#define IICSOF_PARAM_MAP_X \
    X("SCL", iicSof_t, SCL, io_t) \
    X("SDA_OUT", iicSof_t, SDA_OUT, io_t) \
    X("SDA_IN", iicSof_t, SDA_IN, io_t)

typedef struct 
{
    RNG_HandleTypeDef hrng;

}rng_t;
#define RNG_PARAM_MAP_X \
    X("hrng", rng_t, hrng, RNG_HandleTypeDef) \


typedef struct
{
  uint32_t CardType;          /*!< Specifies the card Type                         */
  uint32_t CardVersion;       /*!< Specifies the card version                      */
  uint32_t Class;             /*!< Specifies the class of the card class           */
  uint32_t RelCardAdd;        /*!< Specifies the Relative Card Address             */
  uint32_t BlockNbr;          /*!< Specifies the Card Capacity in blocks           */
  uint32_t BlockSize;         /*!< Specifies one block size in bytes               */
  uint32_t LogBlockNbr;       /*!< Specifies the Card logical Capacity in blocks   */
  uint32_t LogBlockSize;      /*!< Specifies logical block size in bytes           */
}sdCardInfo_t;

typedef struct 
{
    SD_HandleTypeDef hsd;
}sdmmc_t;
#define SDMMC_PARAM_MAP_X \
    X("hsd", sdmmc_t, hsd, SD_HandleTypeDef) 

typedef struct
{
    float period; 
    TIM_HandleTypeDef htim;
    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_MasterConfigTypeDef sMasterConfig;
}tim_t;

#define TIM_PARAM_MAP_X \
    X("period", tim_t, period, float) \
    X("htim", tim_t, htim, TIM_HandleTypeDef) \
    X("sClockSourceConfig", tim_t, sClockSourceConfig, TIM_ClockConfigTypeDef) \
    X("sMasterConfig", tim_t, sMasterConfig, TIM_MasterConfigTypeDef)


#endif
#ifdef __cplusplus
}
#endif
#endif
