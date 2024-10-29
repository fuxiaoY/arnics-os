#include "drivers_var.h"

// 使用 X-macro 定义设备映射表
const device_map_t device_map[] = {
    #define X(name, device, type) {name, device, type},
    DEVICE_MAP_X
    #undef X
};
const size_t device_map_size = sizeof(device_map) / sizeof(device_map_t);
/* define --------------------------------------------------------*/
// 驱动映射
device_t led0_ds; // led0
device_t led1_ds; // led1
device_t debug_ds; // debug串口


// 驱动实例默认值
uart_t uart1 = 
{
    .huart = &huart1,
    .baudrate = 115200,
    .data_bits = 8,
    .stop_bits = 1,
    .parity = 0,
    .dma_mode = 0
};
io_t led0 = 
{
    .GPIOx = GPIOA,
    .GPIO_Pin = GPIO_PIN_8,
    .GPIO_Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_Pull = GPIO_NOPULL,
    .GPIO_Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_RESET
};
io_t led1 = 
{
    .GPIOx = GPIOD,
    .GPIO_Pin = GPIO_PIN_2,
    .GPIO_Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_Pull = GPIO_NOPULL,
    .GPIO_Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_RESET
};




