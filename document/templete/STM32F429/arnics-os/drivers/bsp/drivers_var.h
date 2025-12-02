#ifndef _DRIVERS_VAR_H_
#define _DRIVERS_VAR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Inc/ProjDefine.h"
#include "Inc/typedef.h"
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

#define MODEM_UART_RXBUF_SIZE  1024
#define MODEM_UART_TXBUF_SIZE  1024
extern unsigned char modem_rxbuf[MODEM_UART_RXBUF_SIZE];
extern unsigned char modem_txbuf[MODEM_UART_TXBUF_SIZE];
extern ring_buf_t modem_rbsend;
extern ring_buf_t modem_rbrecv;

extern uint16_t ltdc_lcd_framebuf[272][480]        __attribute__((at(0xC0000000)));
extern uint16_t ltdc_lcd_framebuf_cache[272][480]  __attribute__((at(0xC0040000)));

// 驱动映射
extern device_t delay_ds;
extern device_t debug_ds;
extern device_t mcuflash_ds;       // mcuflash
extern device_t iwdg_ds;           // 独立看门狗
extern device_t rtc_ds;            // rtc
extern device_t rng_ds;

extern device_t wan_uart_ds;
extern device_t w25q_cs_ds;        // w25q cs
extern device_t w25q_spi_ds;       // w25q
extern device_t lcd_ds;
extern device_t dma2d_ds;
extern device_t sdram_ds;
extern device_t gt9xxx_i2c_ds;
extern device_t gt9xxx_io_int_ds;
extern device_t gt9xxx_io_rst_ds;
/* instance --------------------------------------------------------*/
// delay
extern delay_t delay;
// debug
extern uart_t uart1;
// iwdg
extern iwdg_t iwdg;
// rng
extern rng_t hrng;
// rtc
extern rtc_t rtc;
//flash
extern flash_t mcu_flash;
// spi
extern io_t io_f6;
extern spi_t w25q_spi;
// lcd
extern ltdc_t lcd_ltdc;
//dma2d
extern dma2d_t dma2d;
//sdram
extern sdram_t sdram;

extern io_t io_h7;
extern io_t io_i8;
extern iicSof_t gtxx_i2c;
/* Lists--- -----------------------------------------------------------*/
// 设备总表的 X-macro
// {设备名称（可任意），设备实例，设备类型结构体名称}
#define DEVICE_MAP_X \
    X("void", NULL, "void") \
    X("delay",          &delay,         "delay_t")  \
    X("uart1",          &uart1,         "uart_t")   \
    X("mcu_flash",      &mcu_flash,     "flash_t")  \
    X("iwdg",           &iwdg,          "iwdg_t")   \
    X("rtc",            &rtc,           "rtc_t")    \
    X("rng",            &hrng,          "rng_t")    \
    X("w25q_cs",        &io_f6,         "io_t")     \
    X("w25q_spi",       &w25q_spi,      "spi_t")    \
    X("lcd",            &lcd_ltdc,      "ltdc_t")   \
    X("dma2d",          &dma2d,         "dma2d_t")  \
    X("sdram",          &sdram,         "sdram_t")  \
    X("gt9xxx_i2c",     &gtxx_i2c,      "iicSof_t") \
    X("gt9xxx_io_int",  &io_h7,         "io_t")     \
    X("gt9xxx_io_rst",  &io_i8,         "io_t")


#ifdef __cplusplus
}
#endif
#endif

