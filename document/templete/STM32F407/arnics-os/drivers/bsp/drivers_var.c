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

device_t w25q_cs_ds;        // w25q cs
device_t w25q_spi_ds;       // w25q
device_t mcuflash_ds;       // mcuflash
device_t iwdg_ds;           // 独立看门狗
device_t rtc_ds;            // rtc
device_t rng_ds;
device_t tim4_ds;

device_t debug_ds;          // debug串口
// wan模块
device_t wan_uart_ds;       // wan串口
device_t cat1_pen_ds;       // cat1 pen
device_t cat1_pwrkey_ds;    // cat1 pwr 硬件已逻辑反向
device_t cat1_dtr_ds;       // cat1 dtr 硬件已逻辑反向
device_t cat1_rst_ds;       // cat1 rst

device_t nb_pen_ds; 
device_t nb_dtr_ds;         //  硬件已逻辑反向
device_t nb_rst_ds; 
// MPU6050
device_t mpu6050_i2c_ds;    // MPU6050 I2C
device_t mpu6050_pow_ds;    // MPU6050电源控制引脚
// rfid
device_t rfidpow_ds;        // A12   RFID读卡器的POW线
device_t rfidirq_ds;        // A15   RFID读卡器的IRQ线
device_t rfidrst_ds;        // B6    RFID读卡器的RST线
device_t rfidncs_ds;        // C12   RFID读卡器的NCS线
device_t rc522_spi_ds;      // C12   RFID读卡器的NCS线
// VL53L0
device_t vl53l0x_i2c_ds;     // 红外I2C
device_t vl53l0x_pow_ds;     // A11    红外供电控制
device_t vl53l0x_cs1_ds;     // C7     红外1片选
device_t vl53l0x_cs2_ds;     // C6     红外2片选
// 北斗
device_t bd_power_ds;
device_t bd_hot_power_ds;
device_t bd_uart_ds;        // 北斗串口
// 蜂鸣器
device_t buzzer_ds;
// 震动开关
device_t exit_xxw_ds; 
// 电压采集
device_t adc_battery_ds;

io_t io_c9 = 
{
    .GPIOx = GPIOC,
    .GPIO_InitStruct.Pin = GPIO_PIN_9,
    .GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull  = GPIO_NOPULL,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_SET
};


io_t io_a8 =
{
    .GPIOx = GPIOA,
    .GPIO_InitStruct.Pin = GPIO_PIN_8,
    .GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull  = GPIO_NOPULL,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_SET
};
io_t io_b15 =
{
    .GPIOx = GPIOB,
    .GPIO_InitStruct.Pin = GPIO_PIN_15,
    .GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull  = GPIO_NOPULL,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_RESET
};
io_t io_b11 = 
{
    .GPIOx = GPIOB,
    .GPIO_InitStruct.Pin = GPIO_PIN_11,
    .GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING,
    .GPIO_InitStruct.Pull  = GPIO_PULLUP,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_SET
};


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
// W25Q
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
// mcu flash
flash_t mcu_flash;
// iwdg
iwdg_t iwdg = 
{
  .hiwdg.Instance = IWDG,
  .hiwdg.Init.Prescaler = IWDG_PRESCALER_256,
  .hiwdg.Init.Reload = 4095,
};
// rtc
rtc_t rtc = 
{
    .hrtc.Instance = RTC,
    .hrtc.Init.HourFormat = RTC_HOURFORMAT_24,
    .hrtc.Init.AsynchPrediv = 127,
    .hrtc.Init.SynchPrediv = 255,
    .hrtc.Init.OutPut = RTC_OUTPUT_DISABLE,
    .hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH,
    .hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN
};
// rng
rng_t hrng = 
{
    .hrng.Instance = RNG,
};


//4G
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

// mpu6050
io_t mpu6050_pow = 
{
    .GPIOx = GPIOC,
    .GPIO_InitStruct.Pin = GPIO_PIN_8,
    .GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull = GPIO_NOPULL,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_SET
};
iicSof_t mpu6050_i2c = 
{
    .SCL.GPIOx = GPIOB,
    .SCL.GPIO_InitStruct.Pin = GPIO_PIN_13,
    .SCL.GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .SCL.GPIO_InitStruct.Pull  = GPIO_NOPULL,
    .SCL.GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    
    .SDA_OUT.GPIOx = GPIOB,
    .SDA_OUT.GPIO_InitStruct.Pin = GPIO_PIN_12,
    .SDA_OUT.GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD,
    .SDA_OUT.GPIO_InitStruct.Pull  = GPIO_NOPULL,
    .SDA_OUT.GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
	
    .SDA_IN.GPIOx = GPIOB,
    .SDA_IN.GPIO_InitStruct.Pin = GPIO_PIN_12,
    .SDA_IN.GPIO_InitStruct.Mode = GPIO_MODE_INPUT,
    .SDA_IN.GPIO_InitStruct.Pull  = GPIO_NOPULL,
    .SDA_IN.GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
};

// vl53l0x
io_t vl53l0x_cs1 = 
{
    .GPIOx = GPIOC,
    .GPIO_InitStruct.Pin = GPIO_PIN_7,
    .GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull = GPIO_NOPULL,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_SET
};
io_t vl53l0x_cs2 = 
{
    .GPIOx = GPIOC,
    .GPIO_InitStruct.Pin = GPIO_PIN_6,
    .GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull = GPIO_NOPULL,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_SET
};
io_t vl53l0x_pow = 
{
    .GPIOx = GPIOA,
    .GPIO_InitStruct.Pin = GPIO_PIN_11,
    .GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull = GPIO_NOPULL,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_RESET
};
iicSof_t vl53l0x_i2c = 
{
    .SCL.GPIOx = GPIOC,
    .SCL.GPIO_InitStruct.Pin = GPIO_PIN_10,
    .SCL.GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .SCL.GPIO_InitStruct.Pull  = GPIO_NOPULL,
    .SCL.GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    
    .SDA_OUT.GPIOx = GPIOC,
    .SDA_OUT.GPIO_InitStruct.Pin = GPIO_PIN_11,
    .SDA_OUT.GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD,
    .SDA_OUT.GPIO_InitStruct.Pull  = GPIO_NOPULL,
    .SDA_OUT.GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
	
    .SDA_IN.GPIOx = GPIOC,
    .SDA_IN.GPIO_InitStruct.Pin = GPIO_PIN_11,
    .SDA_IN.GPIO_InitStruct.Mode = GPIO_MODE_INPUT,
    .SDA_IN.GPIO_InitStruct.Pull  = GPIO_NOPULL,
    .SDA_IN.GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
};
// rc522
io_t rfidpow = 
{
    .GPIOx = GPIOA,
    .GPIO_InitStruct.Pin = GPIO_PIN_12,
    .GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull = GPIO_NOPULL,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_RESET
};
io_t rfidirq = 
{
    .GPIOx = GPIOA,
    .GPIO_InitStruct.Pin = GPIO_PIN_15,
    .GPIO_InitStruct.Mode = GPIO_MODE_INPUT,
    .GPIO_InitStruct.Pull = GPIO_PULLUP,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_SET
};
io_t rfidrst = 
{
    .GPIOx = GPIOB,
    .GPIO_InitStruct.Pin = GPIO_PIN_6,
    .GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull = GPIO_NOPULL,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_SET
};
io_t rfidncs = 
{
    .GPIOx = GPIOC,
    .GPIO_InitStruct.Pin = GPIO_PIN_12,
    .GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull = GPIO_NOPULL,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH,
    .PinState = GPIO_PIN_SET
};
spi_t rc522_spi = 
{
    .hspi.Instance = SPI3,
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


tim_t tim4 = 
{
    .period = 10,
    .htim.Instance = TIM4,
    .htim.Init.CounterMode = TIM_COUNTERMODE_UP,
    .htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,
    .htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE,
    .sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL,
    .sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET,
    .sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE
};
