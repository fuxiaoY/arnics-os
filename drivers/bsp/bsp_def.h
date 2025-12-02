
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

#include "Inc/ProjDefine.h"
#include "Inc/typedef.h"
#include "drivers/common/drivers_common.h"
#include "thirdParty/thirdPartyInclude.h"
#include "port/arnicsPort.h"

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
#define DRIVERS_ENABLE_DELAY
#define DRIVERS_ENABLE_LTDC
#define DRIVERS_ENABLE_DMA2D
#define DRIVERS_ENABLE_SDRAM
#define DRIVERS_ENABLE_CAN

#ifdef MCU_STM32F1
/* typedef -----------------------------------------------------------*/
#ifdef DRIVERS_ENABLE_GPIO
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
	
#endif /* DRIVERS_ENABLE_GPIO */

#ifdef DRIVERS_ENABLE_UART
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

#endif /* DRIVERS_ENABLE_UART */

#ifdef DRIVERS_ENABLE_SPI
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

#endif /* DRIVERS_ENABLE_SPI */

#ifdef DRIVERS_ENABLE_FLASH
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

#endif /* DRIVERS_ENABLE_FLASH */

#ifdef DRIVERS_ENABLE_IWDG
typedef struct
{
    IWDG_HandleTypeDef hiwdg;
  /* data */
}iwdg_t;
// 独立看门狗 参数映射表的 X-macro
#define IWDG_PARAM_MAP_X \
    X("hiwdg", iwdg_t, hiwdg, IWDG_HandleTypeDef) 

#endif /* DRIVERS_ENABLE_IWDG */

#ifdef DRIVERS_ENABLE_RTC
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

#endif /* DRIVERS_ENABLE_RTC */

#ifdef DRIVERS_ENABLE_ADC
typedef struct 
{
    ADC_HandleTypeDef hadc;
    uint8_t channel_num;
    ADC_ChannelConfTypeDef *sConfig_p;
}adc_t;

#define ADC_PARAM_MAP_X \
    X("hadc", adc_t, hadc, ADC_HandleTypeDef) \
    X("sConfig", adc_t, sConfig_p, ADC_ChannelConfTypeDef *)

#endif /* DRIVERS_ENABLE_ADC */

#ifdef DRIVERS_ENABLE_IICSOF
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
    
#endif /* DRIVERS_ENABLE_IICSOF */

#ifdef DRIVERS_ENABLE_DELAY
// delay结构体
typedef struct
{
    bool use_systick;
} delay_t;
// IO 参数映射表的 X-macro
#define DELAY_PARAM_MAP_X \
    X("use_systick", delay_t, use_systick, bool) 
#endif /* DRIVERS_ENABLE_DELAY */

#ifdef DRIVERS_ENABLE_CAN
typedef struct
{
    CAN_TxHeaderTypeDef txHeader;
    CAN_HandleTypeDef hcan;
    CAN_FilterTypeDef  sFilterConfig;
    ring_buf_t *ring1_rx;
    ring_buf_t *ring1_tx;
    unsigned char *rx1_buf;
    unsigned char *tx1_buf;
    unsigned int rx_buf1_size;
    unsigned int tx_buf1_size;
  /* data */
}can_t;

// CAN 参数映射表的 X-macro
#define CAN_PARAM_MAP_X \
    X("hcan", can_t, hcan, CAN_HandleTypeDef) \
    X("Instance", can_t, hcan.Instance, CAN_TypeDef) \
    X("Mode", can_t, hcan.Init.Mode, uint32_t) \
    X("TimeTriggeredMode", can_t, hcan.Init.TimeTriggeredMode, uint32_t) \
    X("AutoBusOff", can_t, hcan.Init.AutoBusOff, uint32_t) \
    X("AutoWakeUp", can_t, hcan.Init.AutoWakeUp, uint32_t) \
    X("AutoRetransmission", can_t, hcan.Init.AutoRetransmission, uint32_t) \
    X("ReceiveFifoLocked", can_t, hcan.Init.ReceiveFifoLocked, uint32_t) \
    X("TransmitFifoPriority", can_t, hcan.Init.TransmitFifoPriority, uint32_t) \
    X("Prescaler", can_t, hcan.Init.Prescaler, uint32_t) \
    X("SyncJumpWidth", can_t, hcan.Init.SyncJumpWidth, uint32_t) \
    X("TimeSeg1", can_t, hcan.Init.TimeSeg1, uint32_t) \
    X("TimeSeg2", can_t, hcan.Init.TimeSeg2, uint32_t)
#endif /* DRIVERS_ENABLE_CAN */

#elif defined(MCU_STM32L4)
/* typedef -----------------------------------------------------------*/
#ifdef DRIVERS_ENABLE_GPIO
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

#endif /* DRIVERS_ENABLE_GPIO */

#ifdef DRIVERS_ENABLE_UART
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

#endif /* DRIVERS_ENABLE_UART */

#ifdef DRIVERS_ENABLE_SPI
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

#endif /* DRIVERS_ENABLE_SPI */

#ifdef DRIVERS_ENABLE_FLASH
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

#endif /* DRIVERS_ENABLE_FLASH */

#ifdef DRIVERS_ENABLE_IWDG
typedef struct
{
    IWDG_HandleTypeDef hiwdg;
  /* data */
}iwdg_t;
// 独立看门狗 参数映射表的 X-macro
#define IWDG_PARAM_MAP_X \
    X("hiwdg", iwdg_t, hiwdg, IWDG_HandleTypeDef) 

#endif /* DRIVERS_ENABLE_IWDG */

#ifdef DRIVERS_ENABLE_RTC
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

#endif /* DRIVERS_ENABLE_RTC */

#ifdef DRIVERS_ENABLE_ADC
typedef struct 
{
    ADC_HandleTypeDef hadc;
    uint8_t channel_num;
    ADC_ChannelConfTypeDef *sConfig_p;
}adc_t;

#define ADC_PARAM_MAP_X \
    X("hadc", adc_t, hadc, ADC_HandleTypeDef) \
    X("sConfig", adc_t, sConfig_p, ADC_ChannelConfTypeDef *)

#endif /* DRIVERS_ENABLE_ADC */

#ifdef DRIVERS_ENABLE_IICSOF
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

#endif /* DRIVERS_ENABLE_IICSOF */

#ifdef DRIVERS_ENABLE_RNG
typedef struct 
{
    RNG_HandleTypeDef hrng;

}rng_t;
#define RNG_PARAM_MAP_X \
    X("hrng", rng_t, hrng, RNG_HandleTypeDef) \

#endif /* DRIVERS_ENABLE_RNG */

#ifdef DRIVERS_ENABLE_SDMMC
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

#endif /* DRIVERS_ENABLE_SDMMC */

#ifdef DRIVERS_ENABLE_TIM
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

#endif /* DRIVERS_ENABLE_TIM */

#ifdef DRIVERS_ENABLE_DELAY
// delay结构体
typedef struct
{
    bool use_systick;
} delay_t;
// IO 参数映射表的 X-macro
#define DELAY_PARAM_MAP_X \
    X("use_systick", delay_t, use_systick, bool) 
#endif /* DRIVERS_ENABLE_DELAY */

#elif defined(MCU_STM32F4)
/* typedef -----------------------------------------------------------*/
#ifdef DRIVERS_ENABLE_GPIO
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

#endif /* DRIVERS_ENABLE_GPIO */

#ifdef DRIVERS_ENABLE_UART
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

#endif /* DRIVERS_ENABLE_UART */

#ifdef DRIVERS_ENABLE_SPI
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

#endif /* DRIVERS_ENABLE_SPI */

#ifdef DRIVERS_ENABLE_FLASH
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

#endif /* DRIVERS_ENABLE_FLASH */

#ifdef DRIVERS_ENABLE_IWDG
typedef struct
{
    IWDG_HandleTypeDef hiwdg;
  /* data */
}iwdg_t;
// 独立看门狗 参数映射表的 X-macro
#define IWDG_PARAM_MAP_X \
    X("hiwdg", iwdg_t, hiwdg, IWDG_HandleTypeDef) 

#endif /* DRIVERS_ENABLE_IWDG */

#ifdef DRIVERS_ENABLE_RTC
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

#endif /* DRIVERS_ENABLE_RTC */

#ifdef DRIVERS_ENABLE_ADC
typedef struct 
{
    ADC_HandleTypeDef hadc;
    uint8_t channel_num;
    ADC_ChannelConfTypeDef *sConfig_p;
}adc_t;

#define ADC_PARAM_MAP_X \
    X("hadc", adc_t, hadc, ADC_HandleTypeDef) \
    X("sConfig", adc_t, sConfig_p, ADC_ChannelConfTypeDef *)

#endif /* DRIVERS_ENABLE_ADC */

#ifdef DRIVERS_ENABLE_IICSOF
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

#endif /* DRIVERS_ENABLE_IICSOF */

#ifdef DRIVERS_ENABLE_RNG
typedef struct 
{
    RNG_HandleTypeDef hrng;

}rng_t;
#define RNG_PARAM_MAP_X \
    X("hrng", rng_t, hrng, RNG_HandleTypeDef) \

#endif /* DRIVERS_ENABLE_RNG */

#ifdef DRIVERS_ENABLE_SDMMC
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

#endif /* DRIVERS_ENABLE_SDMMC */

#ifdef DRIVERS_ENABLE_TIM
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

#endif /* DRIVERS_ENABLE_TIM */

#ifdef DRIVERS_ENABLE_DELAY
// delay结构体
typedef struct
{
    bool use_systick;
} delay_t;
// IO 参数映射表的 X-macro
#define DELAY_PARAM_MAP_X \
    X("use_systick", delay_t, use_systick, bool) 
#endif /* DRIVERS_ENABLE_DELAY */

#ifdef DRIVERS_ENABLE_LTDC
typedef struct  
{                             
    uint32_t                     pwidth;              // LCD面板的宽度,固定参数,不随显示方向改变,如果为0,说明没有任何RGB屏接入
    uint32_t                    pheight;              // LCD面板的高度,固定参数,不随显示方向改变
    uint16_t                        hsw;              // 水平同步宽度
    uint16_t                        vsw;              // 垂直同步宽度
    uint16_t                        hbp;              // 水平后廊
    uint16_t                        vbp;              // 垂直后廊
    uint16_t                        hfp;              // 水平前廊
    uint16_t                        vfp;              // 垂直前廊 
}ltdc_devi; 

typedef struct  
{
    uint8_t                      layerx;             // 层序号: 0-第1层(LAYER0), 1-第2层(LAYER1)
    uint32_t                    bufaddr;             // 帧缓冲区起始地址: 该层图像数据在内存中的首地址
    uint32_t                  pixformat;             // 像素格式: LTDC_Pixelformat
    uint8_t                       alpha;             // 层透明度: 取值范围0-255, 0表示完全透明, 255表示完全不透明
    uint8_t                      alpha0;             // 默认透明度: 当像素无数据时使用的透明度值, 0-255
    uint8_t                       bfac1;             // 混合因子1: 4-恒定Alpha, 6-像素Alpha × 恒定Alpha
    uint8_t                       bfac2;             // 混合因子2: 5-恒定Alpha, 7-像素Alpha × 恒定Alpha
    uint32_t                    bkcolor;             // 背景颜色: 32位RGB888格式, 仅低24位有效, 用作层的默认背景色
}ltdc_config;

typedef struct
{
    // clock
    uint32_t                    pllsain;
    uint32_t                    pllsair;
    uint32_t                 pllsaidivr;
    // init
    uint8_t               backcolor_red;
    uint8_t             backcolor_green;
    uint8_t              backcolor_blue;
    uint32_t                 HSPolarity;
    uint32_t                 VSPolarity;
    uint32_t                 DEPolarity;
    uint32_t                 PCPolarity;
    // config
    uint8_t             ltdc_config_num;
    ltdc_config        ltdc_configer[2];
    // lcd config
    ltdc_devi                  ltdc_dev;
    LTDC_HandleTypeDef     LTDC_Handler;	
}ltdc_t; 

#define LTDC_PARAM_MAP_X \
    X("pllsain",           ltdc_t, pllsain,           uint32_t)       \
    X("pllsair",           ltdc_t, pllsair,           uint32_t)       \
    X("pllsaidivr",        ltdc_t, pllsaidivr,        uint32_t)       \
    X("backcolor_red",     ltdc_t, backcolor_red,     uint8_t)        \
    X("backcolor_green",   ltdc_t, backcolor_green,   uint8_t)        \
    X("backcolor_blue",    ltdc_t, backcolor_blue,    uint8_t)        \
    X("HSPolarity",        ltdc_t, HSPolarity,        uint32_t)       \
    X("VSPolarity",        ltdc_t, VSPolarity,        uint32_t)       \
    X("DEPolarity",        ltdc_t, DEPolarity,        uint32_t)       \
    X("PCPolarity",        ltdc_t, PCPolarity,        uint32_t)       \
    X("ltdc_config_num",   ltdc_t, ltdc_config_num,   uint8_t)        \
    X("ltdc_config",       ltdc_t, ltdc_configer[2],  ltdc_config)    \
    X("ltdc_dev",          ltdc_t, ltdc_dev,          ltdc_devi)      \
    X("LTDC_Handler",      ltdc_t, LTDC_Handler,      LTDC_HandleTypeDef)

#endif /* DRIVERS_ENABLE_LTDC */

#ifdef DRIVERS_ENABLE_DMA2D
typedef struct
{
    uint8_t layer_num;
    uint16_t pixel_size;
    DMA2D_HandleTypeDef hdma2d;
    uint16_t *dst_addr;
    uint16_t width;
    uint16_t height;

}dma2d_t;
#define DMA2D_PARAM_MAP_X \
    X("layer_num",     dma2d_t, layer_num, uint8_t)          \
    X("hdma2d"   ,     dma2d_t, hdma2d, DMA2D_HandleTypeDef) \
    X("dst_addr",      dma2d_t, dst_addr, uint32_t *)        


#endif /* DRIVERS_ENABLE_DMA2D */

#ifdef DRIVERS_ENABLE_SDRAM
typedef struct
{
    SDRAM_HandleTypeDef     SDRAM_Handler;
    FMC_SDRAM_TimingTypeDef SDRAM_Timing;
}sdram_t;
#define SDRAM_PARAM_MAP_X \
    X("SDRAM_Handler", sdram_t, SDRAM_Handler, SDRAM_HandleTypeDef) \
    X("SDRAM_Timing",  sdram_t, SDRAM_Timing, FMC_SDRAM_TimingTypeDef)

#endif /* DRIVERS_ENABLE_SDRAM */

#endif
#ifdef __cplusplus
}
#endif
#endif
