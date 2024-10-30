#include "drivers_var.h"

// 使用 X-macro 定义设备映射表
const device_map_t device_map[] = {
    #define X(name, device, type) {name, device, type},
    DEVICE_MAP_X
    #undef X
};
const size_t device_map_size = sizeof(device_map) / sizeof(device_map_t);
/* define --------------------------------------------------------*/

// 环形缓冲值
unsigned char debug_rxbuf[DEBUG_UART_RXBUF_SIZE]; /*接收缓冲区 ------------*/
unsigned char debug_txbuf[DEBUG_UART_TXBUF_SIZE]; /*发送缓冲区 ------------*/
ring_buf_t debug_rbsend, debug_rbrecv;            /*收发缓冲区管理 --------*/

// 驱动映射
device_t led0_ds; // led0
device_t led1_ds; // led1
device_t debug_ds; // debug串口

// 驱动实例默认值
uart_t uart1 = 
{
    .baudrate = 115200,
    .data_bits = UART_WORDLENGTH_8B,
    .stop_bits = UART_STOPBITS_1,
    .parity = UART_PARITY_NONE,
    .dma_mode = 0,
    .function = UART_DEBUG,
    .ring_rx = &debug_rbsend,
    .ring_tx = &debug_rbrecv,
    .rx_buf = debug_rxbuf,
    .tx_buf = debug_txbuf,
    .rx_buf_size = DEBUG_UART_RXBUF_SIZE,
    .tx_buf_size = DEBUG_UART_TXBUF_SIZE
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



