#ifndef INTERFACE_INCLUDE_H
#define INTERFACE_INCLUDE_H
#ifdef __cplusplus
extern "C" {
#endif

#ifdef DRIVERS_ENABLE_GPIO
#include "interface_gpio.h"
#endif
#ifdef DRIVERS_ENABLE_UART
#include "interface_uart.h"
#endif
#ifdef DRIVERS_ENABLE_SPI
#include "interface_spi.h"
#endif
#ifdef DRIVERS_ENABLE_FLASH
#include "interface_flash.h"
#endif
#ifdef DRIVERS_ENABLE_IWDG
#include "interface_iwdg.h"
#endif
#ifdef DRIVERS_ENABLE_RTC
#include "interface_rtc.h"
#endif
#ifdef DRIVERS_ENABLE_ADC
#include "interface_adc.h"
#endif
#ifdef DRIVERS_ENABLE_IICSOF
#include "interface_iicSof.h"
#endif
#ifdef DRIVERS_ENABLE_RNG
#include "interface_rng.h"
#endif
#ifdef DRIVERS_ENABLE_SDMMC
#include "interface_sdmmc.h"
#endif
#ifdef DRIVERS_ENABLE_TIM
#include "interface_tim.h"
#endif
#ifdef DRIVERS_ENABLE_DELAY
#include "interface_delay.h"
#endif
#ifdef DRIVERS_ENABLE_LTDC
#include "interface_ltdc.h"
#endif
#ifdef DRIVERS_ENABLE_DMA2D
#include "interface_dma2d.h"
#endif
#ifdef DRIVERS_ENABLE_SDRAM
#include "interface_sdram.h"
#endif
#ifdef __cplusplus
}
#endif
#endif /* INTERFACE_INCLUDE_H */
