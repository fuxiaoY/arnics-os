
#include "drivers_list.h"
#include "../component/inc/interface_Include.h"
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
                        NULL};
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
    // 可以添加更多设备类型
};
const size_t device_type_maps_size = sizeof(device_type_maps) / sizeof(device_type_map_t);
