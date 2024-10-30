
#include "drivers_list.h"
#include "../component/inc/interface_Include.h"
/* Lists--- -----------------------------------------------------------*/
// 使用 X-macro 生成 UART 参数映射表
const param_map_t uart_param_map[] = {
    #define X(name, struct_type, field, field_type) \
        {name, offsetof(struct_type, field), sizeof(field_type)},
    UART_PARAM_MAP_X
    #undef X
};
const size_t uart_param_map_size = sizeof(uart_param_map) / sizeof(param_map_t);
const dev_operations uart_ops = {
                        uart_open,
                        uart_close,
                        uart_read,
                        uart_write,
                        uart_ctl,
                        uart_irq};
               
/* Lists--- -----------------------------------------------------------*/
// 使用 X-macro 生成 IO 参数映射表
const param_map_t io_param_map[] = {
    #define X(name, struct_type, field, field_type) \
        {name, offsetof(struct_type, field), sizeof(field_type)},
    IO_PARAM_MAP_X
    #undef X
};
const size_t io_param_map_size = sizeof(io_param_map) / sizeof(param_map_t);

const dev_operations io_ops = {
                        gpio_open,
                        gpio_close,
                        NULL,
                        NULL,
                        gpio_ctl,
                        gpio_irq};

/* Lists--- -----------------------------------------------------------*/
// 设备类型映射表
const device_type_map_t device_type_maps[] = {
    {"uart_t", uart_param_map, sizeof(uart_param_map) / sizeof(param_map_t),&uart_ops},
    {"io_t", io_param_map, sizeof(io_param_map) / sizeof(param_map_t),&io_ops},
    // 可以添加更多设备类型
};
const size_t device_type_maps_size = sizeof(device_type_maps) / sizeof(device_type_map_t);
