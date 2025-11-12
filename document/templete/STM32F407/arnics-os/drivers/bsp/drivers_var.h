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

#define MODEM_UART_RXBUF_SIZE  1024
#define MODEM_UART_TXBUF_SIZE  1024
extern unsigned char modem_rxbuf[MODEM_UART_RXBUF_SIZE];
extern unsigned char modem_txbuf[MODEM_UART_TXBUF_SIZE];
extern ring_buf_t modem_rbsend;
extern ring_buf_t modem_rbrecv;
// 驱动映射
extern device_t w25q_cs_ds;        // w25q cs
extern device_t w25q_spi_ds;       // w25q
extern device_t mcuflash_ds;       // mcuflash
extern device_t iwdg_ds;           // 独立看门狗
extern device_t rtc_ds;            // rtc
extern device_t rng_ds;

extern device_t debug_ds;          // debug串口
extern device_t wan_uart_ds;       // wan串口

extern device_t tim4_ds;

extern device_t cat1_pen_ds;       // cat1 pen
extern device_t cat1_pwrkey_ds;    // cat1 pwr 硬件已逻辑反向
extern device_t cat1_dtr_ds;       // cat1 dtr 硬件已逻辑反向
extern device_t cat1_rst_ds;       // cat1 rst

extern device_t nb_pen_ds; 
extern device_t nb_dtr_ds;         //  硬件已逻辑反向
extern device_t nb_rst_ds; 

extern device_t mpu6050_i2c_ds;    // MPU6050 I2C
extern device_t mpu6050_pow_ds;    // MPU6050电源控制引脚

extern device_t rfidpow_ds;        // A12   RFID读卡器的POW线
extern device_t rfidirq_ds;        // A15   RFID读卡器的IRQ线
extern device_t rfidrst_ds;        // B6    RFID读卡器的RST线
extern device_t rfidncs_ds;        // C12   RFID读卡器的NCS线
extern device_t rc522_spi_ds;      // C12   RFID读卡器的NCS线

extern device_t vl53l0x_i2c_ds;     // 红外I2C
extern device_t vl53l0x_pow_ds;     // A11    红外供电控制
extern device_t vl53l0x_cs1_ds;     // C7     红外1片选
extern device_t vl53l0x_cs2_ds;     // C6     红外2片选
// 北斗
extern device_t bd_power_ds;
extern device_t bd_hot_power_ds;
extern device_t bd_uart_ds;        // 北斗串口

extern device_t buzzer_ds;
// 震动开关
extern device_t exit_xxw_ds; 
extern device_t adc_battery_ds;
// 驱动实例


// 驱动实例默认值
extern uart_t uart1;
extern uart_t uart2;

// W25Q
extern io_t w25q_cs;
extern spi_t w25q_spi;
// mcu flash
extern flash_t mcu_flash;
// iwdg
extern iwdg_t iwdg;
// rtc
extern rtc_t rtc;
// rng
extern rng_t hrng;
//tim4
extern tim_t tim4;
//4G
extern io_t cat1_pen;
extern io_t cat1_pwrkey;
extern io_t cat1_dtr;
// mpu6050
extern io_t mpu6050_pow;
extern iicSof_t mpu6050_i2c;
// vl53l0x
extern io_t vl53l0x_cs1;
extern io_t vl53l0x_cs2;
extern io_t vl53l0x_pow;
extern iicSof_t vl53l0x_i2c;
// rc522
extern io_t rfidpow;
extern io_t rfidirq;
extern io_t rfidrst;
extern io_t rfidncs;
extern spi_t rc522_spi;

extern io_t io_c9;
extern io_t io_a8;
extern io_t io_b15;
extern io_t io_b11;

/* Lists--- -----------------------------------------------------------*/
// 设备总表的 X-macro
// {设备名称（可任意），设备实例，设备类型结构体名称}
#define DEVICE_MAP_X \
    X("void", NULL, "void") \
    X("uart1", &uart1, "uart_t") \
    X("uart2", &uart2, "uart_t") \
    X("w25q_cs", &w25q_cs, "io_t") \
    X("w25q_spi", &w25q_spi, "spi_t") \
    X("iwdg", &iwdg, "iwdg_t") \
    X("rtc", &rtc, "rtc_t") \
    X("rng", &hrng, "rng_t") \
    X("mcu_flash", &mcu_flash, "flash_t") \
    X("tim4", &tim4, "tim_t") 


    
#ifdef __cplusplus
}
#endif
#endif

