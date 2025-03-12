#ifndef _ARNICSPORT_H
#define _ARNICSPORT_H


#ifdef __cplusplus
extern "C" {
#endif

#include "../Inc/projDefine.h"
#include "../Inc/typedef.h"


// +-------------------------------------------------+
// | 平台             | STM32CHIP      | STM32系列 |
// +-------------------------------------------------+

// +-------------------------------------------------+
//  编程粒度
//  nor flash:   1 bit
//  stm32f2/f4:  8 bit
//  stm32f1:     32 bit
//  stm32l4:     64 bit
#ifdef MCU_STM32F1
#define STM32CHIP
#include "stm32f1xx_hal.h"
#define MCU_GRAN 32
#define _TOTAL_FLASH_SIZE_ 256 // 芯片容量 （KB）
#define MCU_START_ADDR 0x08000000


#elif defined(MCU_STM32L4)
#define STM32CHIP
#include "stm32l4xx_hal.h"
#define MCU_GRAN 32
#define _TOTAL_FLASH_SIZE_ 256 // 芯片容量 （KB）
#define MCU_START_ADDR 0x08000000
#endif

#ifdef __cplusplus
}
#endif
#endif
