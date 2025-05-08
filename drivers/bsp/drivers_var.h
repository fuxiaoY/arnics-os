#ifndef _DRIVERS_VAR_H_
#define _DRIVERS_VAR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../../Inc/ProjDefine.h"
#include "../../Inc/typedef.h"
#include "bsp_def.h"

// 设备类型映射表
extern const device_type_map_t device_type_maps[];
extern const size_t device_map_size;


/* declaration --------------------------------------------------------*/

// 环形缓冲值
#define DEBUG_UART_RXBUF_SIZE  512
#define DEBUG_UART_TXBUF_SIZE  512
extern unsigned char debug_rxbuf[DEBUG_UART_RXBUF_SIZE];
extern unsigned char debug_txbuf[DEBUG_UART_TXBUF_SIZE];
extern ring_buf_t debug_rbsend;
extern ring_buf_t debug_rbrecv;

// 驱动映射
extern device_t led0_ds;     // led0
extern device_t led1_ds;     // led1
extern device_t w25q_cs_ds;  // w25q cs
extern device_t w25q_spi_ds; // w25q
extern device_t debug_ds;    // debug串口
extern device_t mcuflash_ds; // mcu flash
extern device_t iwdg_ds;     // 独立看门狗
extern device_t rtc_ds;      // rtc
extern device_t adc1_ds;     // adc1
extern device_t iicsof1_ds;  // iic sof

// 驱动实例
extern uart_t uart1;
extern io_t led0;
extern io_t led1;
extern io_t w25q_cs;
extern spi_t w25q_spi;
extern flash_t mcu_flash;
extern iwdg_t iwdg;
extern rtc_t rtc;
extern adc_t adc1;
extern device_t iicsof1_ds;
extern iicSof_t iicsof1;
/* Lists--- -----------------------------------------------------------*/
// 设备总表的 X-macro
// {设备名称（可任意），设备实例，设备类型结构体名称}
#define DEVICE_MAP_X \
    X("void", NULL, "void") \
    X("uart1", &uart1, "uart_t") \
    X("led0", &led0, "io_t") \
    X("led1", &led1, "io_t") \
    X("w25q_cs", &w25q_cs, "io_t") \
    X("w25q_spi", &w25q_spi, "spi_t") \
    X("mcu_flash", &mcu_flash, "flash_t") \
    X("iwdg", &iwdg, "iwdg_t") \
    X("rtc", &rtc, "rtc_t") \
    X("adc1", &adc1, "adc_t") \
    X("iicsof1", &iicsof1, "iicSof_t") 


#ifdef __cplusplus
}
#endif
#endif
