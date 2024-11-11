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
device_t w25q_cs_ds; // w25q cs
device_t w25q_spi_ds; // w25q
device_t mcuflash_ds; // mcuflash
device_t iwdg_ds; // 独立看门狗
device_t rtc_ds; // rtc
device_t mct_ds;
// 驱动实例默认值
uart_t uart1 = 
{
    .huart.Instance = USART1,
    .huart.Init.BaudRate = 115200,
    .huart.Init.WordLength = UART_WORDLENGTH_8B,
    .huart.Init.StopBits = UART_STOPBITS_1,
    .huart.Init.Parity = UART_PARITY_NONE,
    .huart.Init.Mode = UART_MODE_TX_RX,
    .huart.Init.HwFlowCtl = UART_HWCONTROL_NONE,
    .huart.Init.OverSampling = UART_OVERSAMPLING_16,
    .dma_mode = 0,
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
    .GPIO_InitStruct.Pin = GPIO_PIN_8,
    .GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull = GPIO_NOPULL,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_RESET
};
io_t led1 = 
{
    .GPIOx = GPIOD,
    .GPIO_InitStruct.Pin = GPIO_PIN_2,
    .GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull = GPIO_NOPULL,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_RESET
};
io_t w25q_cs = 
{
    .GPIOx = GPIOA,
    .GPIO_InitStruct.Pin = GPIO_PIN_2,
    .GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull  = GPIO_NOPULL,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_SET
};
spi_t w25q_spi = 
{
    .hspi.Instance = SPI1,
    .hspi.Init.Mode = SPI_MODE_MASTER,
    .hspi.Init.Direction = SPI_DIRECTION_2LINES,
    .hspi.Init.DataSize = SPI_DATASIZE_8BIT,
    .hspi.Init.CLKPolarity = SPI_POLARITY_LOW,
    .hspi.Init.CLKPhase = SPI_PHASE_1EDGE,
    .hspi.Init.NSS = SPI_NSS_SOFT,
    .hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2,
    .hspi.Init.FirstBit = SPI_FIRSTBIT_MSB,
    .hspi.Init.TIMode = SPI_TIMODE_DISABLE,
    .hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE,
    .hspi.Init.CRCPolynomial = 7
};

flash_t mcu_flash = 
{
    .EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES,
    .EraseInitStruct.Banks = FLASH_BANK_1,
    .EraseInitStruct.NbPages = 1
};
iwdg_t iwdg = 
{
  .hiwdg.Instance = IWDG,
  .hiwdg.Init.Prescaler = IWDG_PRESCALER_256,
  .hiwdg.Init.Reload = 4095
};


rtc_t rtc = 
{
    .hrtc.Instance = RTC,
    .hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND,
    .hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE
};