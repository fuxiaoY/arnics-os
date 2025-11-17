#include "drivers_var.h"
#include "../common/drivers_list.h"
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

unsigned char modem_rxbuf[MODEM_UART_RXBUF_SIZE]; /*接收缓冲区 ------------*/
unsigned char modem_txbuf[MODEM_UART_TXBUF_SIZE]; /*发送缓冲区 ------------*/
ring_buf_t modem_rbsend, modem_rbrecv;            /*收发缓冲区管理 --------*/
// 驱动映射
device_t led0_ds; // led0
device_t led1_ds; // led1
device_t led2_ds; // led2
device_t debug_ds; // debug串口
device_t w25q_cs_ds; // w25q cs
device_t w25q_spi_ds; // w25q
device_t mcuflash_ds; // mcuflash
device_t iwdg_ds; // 独立看门狗
device_t rtc_ds; // rtc
device_t adc1_ds;
device_t iicsof1_ds;
device_t rng_ds;
device_t sd_card_ds;


device_t wan_uart_ds;// wan uart 
device_t cat1_pen_ds; // cat1 pen
device_t cat1_pwrkey_ds; // cat1 pwr 硬件已逻辑反向
device_t cat1_dtr_ds; // cat1 dtr 硬件已逻辑反向
device_t cat1_rst_ds; // cat1 rst

device_t nb_pen_ds; 
device_t nb_dtr_ds;  //  硬件已逻辑反向
device_t nb_rst_ds; 
// 驱动实例默认值
uart_t lpuart1 = 
{
    .huart.Instance = LPUART1,
    .huart.Init.BaudRate = 115200,
    .huart.Init.WordLength = UART_WORDLENGTH_8B,
    .huart.Init.StopBits = UART_STOPBITS_1,
    .huart.Init.Parity = UART_PARITY_NONE,
    .huart.Init.Mode = UART_MODE_TX_RX,
    .huart.Init.HwFlowCtl = UART_HWCONTROL_NONE,
    .huart.Init.OverSampling = UART_OVERSAMPLING_16,
    .dma_mode = 0,
    .ring_rx = &debug_rbrecv,
    .ring_tx = &debug_rbsend,
    .rx_buf = debug_rxbuf,
    .tx_buf = debug_txbuf,
    .rx_buf_size = DEBUG_UART_RXBUF_SIZE,
    .tx_buf_size = DEBUG_UART_TXBUF_SIZE
};

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
    .ring_rx = &debug_rbrecv,
    .ring_tx = &debug_rbsend,
    .rx_buf = debug_rxbuf,
    .tx_buf = debug_txbuf,
    .rx_buf_size = DEBUG_UART_RXBUF_SIZE,
    .tx_buf_size = DEBUG_UART_TXBUF_SIZE
};
uart_t uart2 = 
{
    .huart.Instance = USART2,
    .huart.Init.BaudRate = 9600,
    .huart.Init.WordLength = UART_WORDLENGTH_8B,
    .huart.Init.StopBits = UART_STOPBITS_1,
    .huart.Init.Parity = UART_PARITY_NONE,
    .huart.Init.Mode = UART_MODE_TX_RX,
    .huart.Init.HwFlowCtl = UART_HWCONTROL_NONE,
    .huart.Init.OverSampling = UART_OVERSAMPLING_16,
    .dma_mode = 0,
    .ring_rx = &modem_rbrecv,
    .ring_tx = &modem_rbsend,
    .rx_buf = modem_rxbuf,
    .tx_buf = modem_txbuf,
    .rx_buf_size = MODEM_UART_RXBUF_SIZE,
    .tx_buf_size = DEBUG_UART_TXBUF_SIZE
};
io_t led0 = 
{
    .GPIOx = GPIOC,
    .GPIO_InitStruct.Pin = GPIO_PIN_0,
    .GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull = GPIO_NOPULL,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_RESET
};
io_t led1 = 
{
    .GPIOx = GPIOC,
    .GPIO_InitStruct.Pin = GPIO_PIN_1,
    .GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull = GPIO_NOPULL,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_RESET
};
io_t led2 = 
{
    .GPIOx = GPIOC,
    .GPIO_InitStruct.Pin = GPIO_PIN_2,
    .GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull = GPIO_NOPULL,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_RESET
};

io_t w25q_cs = 
{
    .GPIOx = GPIOB,
    .GPIO_InitStruct.Pin = GPIO_PIN_12,
    .GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull  = GPIO_NOPULL,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_SET
};
spi_t w25q_spi = 
{
    .hspi.Instance = SPI2,
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
  .hiwdg.Init.Reload = 4095,
  .hiwdg.Init.Window = 4095
};


rtc_t rtc = 
{
    .hrtc.Instance = RTC,
    .hrtc.Init.HourFormat = RTC_HOURFORMAT_24,
    .hrtc.Init.AsynchPrediv = 127,
    .hrtc.Init.SynchPrediv = 255,
    .hrtc.Init.OutPut = RTC_OUTPUT_DISABLE,
    .hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE,
    .hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH,
    .hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN
};

// vbat
ADC_ChannelConfTypeDef sSconfig[2] = 
{
    {
        .Channel = ADC_CHANNEL_3,
        .Rank = ADC_REGULAR_RANK_1,
        .SamplingTime = ADC_SAMPLETIME_247CYCLES_5,
        .SingleDiff = ADC_SINGLE_ENDED,
        .OffsetNumber = ADC_OFFSET_NONE,
        .Offset = 0,
    },
    {
        .Channel = ADC_CHANNEL_VREFINT,
        .Rank = ADC_REGULAR_RANK_2,
        .SamplingTime = ADC_SAMPLETIME_247CYCLES_5,
        .SingleDiff = ADC_SINGLE_ENDED,
        .OffsetNumber = ADC_OFFSET_NONE,
        .Offset = 0,
    }
};

adc_t adc1 = 
{
    .hadc.Instance = ADC1,
    .hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1,
    .hadc.Init.Resolution = ADC_RESOLUTION_12B,
    .hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT,
    .hadc.Init.ScanConvMode = ADC_SCAN_ENABLE,
    .hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV,
    .hadc.Init.LowPowerAutoWait = DISABLE,
    .hadc.Init.ContinuousConvMode = DISABLE,
    .hadc.Init.NbrOfConversion = 2,
    .hadc.Init.DiscontinuousConvMode = DISABLE,
    .hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START,
    .hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE,
    .hadc.Init.DMAContinuousRequests = DISABLE,
    .hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED,
    .hadc.Init.OversamplingMode = DISABLE,
    /** Configure Regular Channel*/
    .channel_num = 2,
    .sConfig_p = sSconfig

};

iicSof_t iicsof1 = 
{
    .SCL.GPIOx = GPIOC,
    .SCL.GPIO_InitStruct.Pin = GPIO_PIN_6,
    .SCL.GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .SCL.GPIO_InitStruct.Pull  = GPIO_NOPULL,
    .SCL.GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,

    .SDA_OUT.GPIOx = GPIOC,
    .SDA_OUT.GPIO_InitStruct.Pin = GPIO_PIN_7,
    .SDA_OUT.GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD,
    .SDA_OUT.GPIO_InitStruct.Pull  = GPIO_NOPULL,
    .SDA_OUT.GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
	
    .SDA_IN.GPIOx = GPIOC,
    .SDA_IN.GPIO_InitStruct.Pin = GPIO_PIN_7,
    .SDA_IN.GPIO_InitStruct.Mode = GPIO_MODE_INPUT,
    .SDA_IN.GPIO_InitStruct.Pull  = GPIO_NOPULL,
    .SDA_IN.GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
	
	
	
};

rng_t hrng = 
{
    .hrng.Instance = RNG,
};

sdmmc_t sd_card = 
{
    .hsd.Instance = SDMMC1,
    .hsd.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING,
    .hsd.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE,
    .hsd.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE,
    .hsd.Init.BusWide = SDMMC_BUS_WIDE_1B,
    .hsd.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE,
    .hsd.Init.ClockDiv = 2
};


io_t cat1_pen = 
{
    .GPIOx = GPIOC,
    .GPIO_InitStruct.Pin = GPIO_PIN_3,
    .GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull  = GPIO_NOPULL,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_SET
};

io_t cat1_pwrkey = 
{
    .GPIOx = GPIOA,
    .GPIO_InitStruct.Pin = GPIO_PIN_0,
    .GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull  = GPIO_NOPULL,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_SET
};
io_t cat1_dtr = 
{
    .GPIOx = GPIOC,
    .GPIO_InitStruct.Pin = GPIO_PIN_4,
    .GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull  = GPIO_NOPULL,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_SET
};


