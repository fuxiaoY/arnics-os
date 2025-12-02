
#include "drivers_list.h"
#include "drivers/component/inc/interface_Include.h"
/* Lists--- -----------------------------------------------------------*/
#ifdef DRIVERS_ENABLE_UART
// 使用 X-macro 生成 UART 参数映射表
const param_map_t uart_param_map[] = {
    #define X(name, struct_type, field, field_type) \
        {name, offsetof(struct_type, field), sizeof(field_type)},
    UART_PARAM_MAP_X
    #undef X
};
const size_t uart_param_map_size = sizeof(uart_param_map) / sizeof(param_map_t);
dev_operations uart_ops = {
                        uart_open,
                        uart_close,
                        uart_read,
                        uart_write,
                        uart_ctl,
                        uart_irq};
#endif               
/* Lists--- -----------------------------------------------------------*/
#ifdef DRIVERS_ENABLE_GPIO
// 使用 X-macro 生成 IO 参数映射表
const param_map_t io_param_map[] = {
    #define X(name, struct_type, field, field_type) \
        {name, offsetof(struct_type, field), sizeof(field_type)},
    IO_PARAM_MAP_X
    #undef X
};
const size_t io_param_map_size = sizeof(io_param_map) / sizeof(param_map_t);

dev_operations io_ops = {
                        gpio_open,
                        gpio_close,
                        NULL,
                        NULL,
                        gpio_ctl,
                        gpio_irq};
#endif
/* Lists--- -----------------------------------------------------------*/
#ifdef DRIVERS_ENABLE_SPI
// 使用 X-macro 生成 SPI 参数映射表
const param_map_t spi_param_map[] = {
    #define X(name, struct_type, field, field_type) \
        {name, offsetof(struct_type, field), sizeof(field_type)},
    SPI_PARAM_MAP_X
    #undef X
};
const size_t spi_param_map_size = sizeof(spi_param_map) / sizeof(param_map_t);

dev_operations spi_ops = {
                        spi_open,
                        spi_close,
                        spi_read,
                        spi_write,
                        spi_ctl,
                        NULL};
#endif
/* Lists--- -----------------------------------------------------------*/
#ifdef DRIVERS_ENABLE_FLASH
// 使用 X-macro 生成 flash 参数映射表
const param_map_t flash_param_map[] = {
    #define X(name, struct_type, field, field_type) \
        {name, offsetof(struct_type, field), sizeof(field_type)},
    FLASH_PARAM_MAP_X
    #undef X
};
const size_t flash_param_map_size = sizeof(flash_param_map) / sizeof(param_map_t);

dev_operations flash_ops = {
                        NULL,
                        NULL,
                        flash_read,
                        flash_write,
                        flash_ctl,
                        NULL};
#endif
/* Lists--- -----------------------------------------------------------*/
#ifdef DRIVERS_ENABLE_IWDG
// 使用 X-macro 生成 独立看门狗 参数映射表
const param_map_t iwdg_param_map[] = {
    #define X(name, struct_type, field, field_type) \
        {name, offsetof(struct_type, field), sizeof(field_type)},
    IWDG_PARAM_MAP_X
    #undef X
};
const size_t iwdg_param_map_size = sizeof(iwdg_param_map) / sizeof(param_map_t);

dev_operations iwdg_ops = {
                        iwdg_open,
                        iwdg_close,
                        NULL,
                        NULL,
                        iwdg_ctl,
                        NULL};
#endif
/* Lists--- -----------------------------------------------------------*/
#ifdef DRIVERS_ENABLE_RTC
// 使用 X-macro 生成 rtc 参数映射表
const param_map_t rtc_param_map[] = {
    #define X(name, struct_type, field, field_type) \
        {name, offsetof(struct_type, field), sizeof(field_type)},
    RTC_PARAM_MAP_X
    #undef X
};
const size_t rtc_param_map_size = sizeof(rtc_param_map) / sizeof(param_map_t);

dev_operations rtc_ops = {
                        rtc_open,
                        rtc_close,
                        NULL,
                        NULL,
                        rtc_ctl,
                        rtc_irq};
#endif
/* Lists--- -----------------------------------------------------------*/
#ifdef DRIVERS_ENABLE_ADC
// 使用 X-macro 生成 adc 参数映射表
const param_map_t adc_param_map[] = {
    #define X(name, struct_type, field, field_type) \
        {name, offsetof(struct_type, field), sizeof(field_type)},
    ADC_PARAM_MAP_X
    #undef X
};
const size_t adc_param_map_size = sizeof(adc_param_map) / sizeof(param_map_t);

dev_operations adc_ops = {
                        adc_open,
                        adc_close,
                        adc_read,
                        NULL,
                        NULL,
                        NULL};
#endif
/* Lists--- -----------------------------------------------------------*/
#ifdef DRIVERS_ENABLE_IICSOF
// 使用 X-macro 生成 iicsof 参数映射表
const param_map_t iicsof_param_map[] = {
    #define X(name, struct_type, field, field_type) \
        {name, offsetof(struct_type, field), sizeof(field_type)},
    IICSOF_PARAM_MAP_X
    #undef X
};
const size_t iicsof_param_map_size = sizeof(iicsof_param_map) / sizeof(param_map_t);

dev_operations iicsof_ops = {
                        iic_open,
                        iic_close,
                        NULL,
                        NULL,
                        iic_ctl,
                        NULL};

#endif
/* Lists--- -----------------------------------------------------------*/
#ifdef DRIVERS_ENABLE_RNG
// 使用 X-macro 生成 rng 参数映射表
const param_map_t rng_param_map[] = {
    #define X(name, struct_type, field, field_type) \
        {name, offsetof(struct_type, field), sizeof(field_type)},
    RNG_PARAM_MAP_X
    #undef X
};
const size_t rng_param_map_size = sizeof(rng_param_map) / sizeof(param_map_t);

dev_operations rng_ops = {
                        rng_open,
                        rng_close,
                        rng_read,
                        NULL,
                        NULL,
                        NULL};
#endif
/* Lists--- -----------------------------------------------------------*/
#ifdef DRIVERS_ENABLE_SDMMC
// 使用 X-macro 生成 sdmmc 参数映射表
const param_map_t sdmmc_param_map[] = {
    #define X(name, struct_type, field, field_type) \
        {name, offsetof(struct_type, field), sizeof(field_type)},
    SDMMC_PARAM_MAP_X
    #undef X
};
const size_t sdmmc_param_map_size = sizeof(sdmmc_param_map) / sizeof(param_map_t);

dev_operations sdmmc_ops = {
                        sdmmc_open,
                        sdmmc_close,
                        NULL,
                        NULL,
                        sdmmc_ctl,
                        NULL};
#endif
/* Lists--- -----------------------------------------------------------*/
#ifdef DRIVERS_ENABLE_TIM
// 使用 X-macro 生成 rng 参数映射表
const param_map_t tim_param_map[] = {
    #define X(name, struct_type, field, field_type) \
        {name, offsetof(struct_type, field), sizeof(field_type)},
    TIM_PARAM_MAP_X
    #undef X
};
const size_t tim_param_map_size = sizeof(tim_param_map) / sizeof(param_map_t);

dev_operations tim_ops = {
                        tim_open,
                        tim_close,
                        NULL,
                        NULL,
                        tim_ctl,
                        tim_irq};
#endif
/* Lists--- -----------------------------------------------------------*/
#ifdef DRIVERS_ENABLE_DELAY
// 使用 X-macro 生成 rng 参数映射表
const param_map_t delay_param_map[] = {
    #define X(name, struct_type, field, field_type) \
        {name, offsetof(struct_type, field), sizeof(field_type)},
    DELAY_PARAM_MAP_X
    #undef X
};
const size_t delay_param_map_size = sizeof(delay_param_map) / sizeof(param_map_t);

dev_operations delay_ops = {
                        delay_open,
                        delay_close,
                        NULL,
                        NULL,
                        delay_ctl,
                        NULL};
#endif
/* Lists--- -----------------------------------------------------------*/
#ifdef DRIVERS_ENABLE_LTDC
// 使用 X-macro 生成 rng 参数映射表
const param_map_t ltdc_param_map[] = {
    #define X(name, struct_type, field, field_type) \
        {name, offsetof(struct_type, field), sizeof(field_type)},
    LTDC_PARAM_MAP_X
    #undef X
};
const size_t ltdc_param_map_size = sizeof(ltdc_param_map) / sizeof(param_map_t);

dev_operations ltdc_ops = {
                        ltdc_open,
                        ltdc_close,
                        NULL,
                        NULL,
                        NULL,
                        NULL};
#endif
/* Lists--- -----------------------------------------------------------*/
#ifdef DRIVERS_ENABLE_DMA2D
// 使用 X-macro 生成 rng 参数映射表
const param_map_t dma2d_param_map[] = {
    #define X(name, struct_type, field, field_type) \
        {name, offsetof(struct_type, field), sizeof(field_type)},
    DMA2D_PARAM_MAP_X
    #undef X
};
const size_t dma2d_param_map_size = sizeof(dma2d_param_map) / sizeof(param_map_t);

dev_operations dma2d_ops = {
                        dma2d_open,
                        dma2d_close,
                        NULL,
                        NULL,
                        dma2d_ctl,
                        NULL};
#endif
/* Lists--- -----------------------------------------------------------*/
#ifdef DRIVERS_ENABLE_SDRAM
// 使用 X-macro 生成 rng 参数映射表
const param_map_t sdram_param_map[] = {
    #define X(name, struct_type, field, field_type) \
        {name, offsetof(struct_type, field), sizeof(field_type)},
    SDRAM_PARAM_MAP_X
    #undef X
};
const size_t sdram_param_map_size = sizeof(sdram_param_map) / sizeof(param_map_t);

dev_operations sdram_ops = {
                        sdram_open,
                        sdram_close,
                        NULL,
                        NULL,
                        NULL,
                        NULL};
#endif
/* Lists--- -----------------------------------------------------------*/
#ifdef DRIVERS_ENABLE_CAN
// 使用 X-macro 生成 CAN 参数映射表
const param_map_t can_param_map[] = {
    #define X(name, struct_type, field, field_type) \
        {name, offsetof(struct_type, field), sizeof(field_type)},
    CAN_PARAM_MAP_X
    #undef X
};
const size_t can_param_map_size = sizeof(can_param_map) / sizeof(param_map_t);

dev_operations can_ops = {
                        can_open,
                        can_close,
                        can_read,
                        can_write,
                        NULL,
                        can_irq};
#endif
/* Lists--- -----------------------------------------------------------*/
/* Lists--- -----------------------------------------------------------*/
// 设备类型映射表
const device_type_map_t device_type_maps[] = {
#ifdef DRIVERS_ENABLE_UART
    {"uart_t", uart_param_map, sizeof(uart_param_map) / sizeof(param_map_t),&uart_ops},
#endif
#ifdef DRIVERS_ENABLE_GPIO
    {"io_t", io_param_map, sizeof(io_param_map) / sizeof(param_map_t),&io_ops},
#endif
#ifdef DRIVERS_ENABLE_SPI
    {"spi_t", spi_param_map, sizeof(spi_param_map) / sizeof(param_map_t),&spi_ops},
#endif
#ifdef DRIVERS_ENABLE_FLASH
    {"flash_t", flash_param_map, sizeof(flash_param_map) / sizeof(param_map_t),&flash_ops},
#endif
#ifdef DRIVERS_ENABLE_IWDG
    {"iwdg_t", iwdg_param_map, sizeof(iwdg_param_map) / sizeof(param_map_t),&iwdg_ops},
#endif
#ifdef DRIVERS_ENABLE_RTC
    {"rtc_t", rtc_param_map, sizeof(rtc_param_map) / sizeof(param_map_t),&rtc_ops},
#endif
#ifdef DRIVERS_ENABLE_ADC
    {"adc_t", adc_param_map, sizeof(adc_param_map) / sizeof(param_map_t),&adc_ops},
#endif
#ifdef DRIVERS_ENABLE_IICSOF
    {"iicSof_t", iicsof_param_map, sizeof(iicsof_param_map) / sizeof(param_map_t),&iicsof_ops},
#endif
#ifdef DRIVERS_ENABLE_RNG
    {"rng_t", rng_param_map, sizeof(rng_param_map) / sizeof(param_map_t),&rng_ops},
#endif
#ifdef DRIVERS_ENABLE_SDMMC
    {"sdmmc_t", sdmmc_param_map, sizeof(sdmmc_param_map) / sizeof(param_map_t),&sdmmc_ops},
#endif
#ifdef DRIVERS_ENABLE_TIM
    {"tim_t", tim_param_map, sizeof(tim_param_map) / sizeof(param_map_t),&tim_ops},
#endif
#ifdef DRIVERS_ENABLE_DELAY
    {"delay_t",delay_param_map, sizeof(delay_param_map) / sizeof(param_map_t),&delay_ops},
#endif
#ifdef DRIVERS_ENABLE_LTDC
    {"ltdc_t",ltdc_param_map, sizeof(ltdc_param_map) / sizeof(param_map_t),&ltdc_ops},
#endif
#ifdef DRIVERS_ENABLE_DMA2D
    {"dma2d_t",dma2d_param_map, sizeof(dma2d_param_map) / sizeof(param_map_t),&dma2d_ops},
#endif
#ifdef DRIVERS_ENABLE_SDRAM
    {"sdram_t",sdram_param_map, sizeof(sdram_param_map) / sizeof(param_map_t),&sdram_ops},
#endif
#ifdef DRIVERS_ENABLE_CAN
    {"can_t", can_param_map, sizeof(can_param_map) / sizeof(param_map_t),&can_ops},
#endif
    // 可以添加更多设备类型
};
const size_t device_type_maps_size = sizeof(device_type_maps) / sizeof(device_type_map_t);
