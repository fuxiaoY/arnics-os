#include "drivers_var.h"
#include "drivers/common/drivers_list.h"
// 使用 X-macro 定义设备映射表
const device_map_t device_map[] = {
    #define X(name, device, type) {name, device, type},
    DEVICE_MAP_X
    #undef X
};
const size_t device_map_size = sizeof(device_map) / sizeof(device_map_t);

// 环形缓冲值
unsigned char debug_rxbuf[DEBUG_UART_RXBUF_SIZE]; /*接收缓冲区 ------------*/
unsigned char debug_txbuf[DEBUG_UART_TXBUF_SIZE]; /*发送缓冲区 ------------*/
ring_buf_t debug_rbsend, debug_rbrecv;            /*收发缓冲区管理 --------*/

unsigned char modem_rxbuf[MODEM_UART_RXBUF_SIZE]; /*接收缓冲区 ------------*/
unsigned char modem_txbuf[MODEM_UART_TXBUF_SIZE]; /*发送缓冲区 ------------*/
ring_buf_t modem_rbsend, modem_rbrecv;            /*收发缓冲区管理 --------*/
// 驱动映射
device_t delay_ds;
device_t debug_ds;
device_t mcuflash_ds;       // mcuflash
device_t iwdg_ds;           // 独立看门狗
device_t rtc_ds;            // rtc
device_t rng_ds;

device_t wan_uart_ds;
device_t w25q_cs_ds;        // w25q cs
device_t w25q_spi_ds;       // w25q

device_t lcd_ds;
device_t dma2d_ds;
device_t sdram_ds;

device_t gt9xxx_i2c_ds;
device_t gt9xxx_io_int_ds;
device_t gt9xxx_io_rst_ds;
/* instance --------------------------------------------------------*/

// delay
delay_t delay = 
{
    .use_systick = false
};

// debug
uart_t uart1 = 
{
    .huart.Instance          = USART1,
    .huart.Init.BaudRate     = 115200,
    .huart.Init.WordLength   = UART_WORDLENGTH_8B,
    .huart.Init.StopBits     = UART_STOPBITS_1,
    .huart.Init.Parity       = UART_PARITY_NONE,
    .huart.Init.Mode         = UART_MODE_TX_RX,
    .huart.Init.HwFlowCtl    = UART_HWCONTROL_NONE,
    .huart.Init.OverSampling = UART_OVERSAMPLING_16,
    .dma_mode                = 0,
    .ring_rx                 = &debug_rbrecv,
    .ring_tx                 = &debug_rbsend,
    .rx_buf                  = debug_rxbuf,
    .tx_buf                  = debug_txbuf,
    .rx_buf_size             = DEBUG_UART_RXBUF_SIZE,
    .tx_buf_size             = DEBUG_UART_TXBUF_SIZE
};

// iwdg
iwdg_t iwdg = 
{
    .hiwdg.Instance         = IWDG,
    .hiwdg.Init.Prescaler   = IWDG_PRESCALER_256,
    .hiwdg.Init.Reload      = 4095,
};

// rng
rng_t hrng = 
{
    .hrng.Instance = RNG,
};

// rtc
rtc_t rtc = 
{
    .hrtc.Instance              = RTC,
    .hrtc.Init.HourFormat       = RTC_HOURFORMAT_24,
    .hrtc.Init.AsynchPrediv     = 127,
    .hrtc.Init.SynchPrediv      = 255,
    .hrtc.Init.OutPut           = RTC_OUTPUT_DISABLE,
    .hrtc.Init.OutPutPolarity   = RTC_OUTPUT_POLARITY_HIGH,
    .hrtc.Init.OutPutType       = RTC_OUTPUT_TYPE_OPENDRAIN
};

// flash
flash_t mcu_flash;

// w25q
io_t io_f6 = 
{
    .GPIOx                    = GPIOF,
    .GPIO_InitStruct.Pin      = GPIO_PIN_6,
    .GPIO_InitStruct.Mode     = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull     = GPIO_NOPULL,
    .GPIO_InitStruct.Speed    = GPIO_SPEED_FREQ_HIGH,
    .PinState                 = GPIO_PIN_SET
};

spi_t w25q_spi = 
{
    .hspi.Instance               = SPI5,
    .hspi.Init.Mode              = SPI_MODE_MASTER,
    .hspi.Init.Direction         = SPI_DIRECTION_2LINES,
    .hspi.Init.DataSize          = SPI_DATASIZE_8BIT,
    .hspi.Init.CLKPolarity       = SPI_POLARITY_LOW,
    .hspi.Init.CLKPhase          = SPI_PHASE_1EDGE,
    .hspi.Init.NSS               = SPI_NSS_SOFT,
    .hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2,
    .hspi.Init.FirstBit          = SPI_FIRSTBIT_MSB,
    .hspi.Init.TIMode            = SPI_TIMODE_DISABLE,
    .hspi.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE,
    .hspi.Init.CRCPolynomial     = 7
};

uint16_t ltdc_lcd_framebuf[272][480]        __attribute__((at(0xC0000000)));
uint16_t ltdc_lcd_framebuf_cache[272][480]  __attribute__((at(0xC0040000)));

ltdc_t lcd_ltdc = 
{
    // clock
   .pllsain                      = 360,
   .pllsair                      = 3,
   .pllsaidivr                   = RCC_PLLSAIDIVR_8,
    // init
   .backcolor_red                = 0,
   .backcolor_green              = 0,
   .backcolor_blue               = 0,     
   .HSPolarity                   = LTDC_HSPOLARITY_AL,
   .VSPolarity                   = LTDC_VSPOLARITY_AL,
   .DEPolarity                   = LTDC_DEPOLARITY_AL,
   .PCPolarity                   = LTDC_PCPOLARITY_IPC,
    // config
   .ltdc_config_num              = 1,
   .ltdc_configer[0].layerx      =  0,
   .ltdc_configer[0].bufaddr     =  (uint32_t)ltdc_lcd_framebuf,
   .ltdc_configer[0].pixformat   =  LTDC_PIXEL_FORMAT_RGB565,
   .ltdc_configer[0].alpha       =  255,
   .ltdc_configer[0].alpha0      =  0,
   .ltdc_configer[0].bfac1       =  6,
   .ltdc_configer[0].bfac2       =  7,
   .ltdc_configer[0].bkcolor     =  0x000000,
    // lcd config
   .ltdc_dev.pwidth             =   480,      
   .ltdc_dev.pheight            =   272,      
   .ltdc_dev.hsw                =   1,       
   .ltdc_dev.vsw                =   1,       
   .ltdc_dev.hbp                =   40,     
   .ltdc_dev.vbp                =   8,    
   .ltdc_dev.hfp                =   5,     
   .ltdc_dev.vfp                =   8      
};

/**
 * 
 * Alpha值是一个8位数值（0-255），用来控制图像或图层的透明程度：
 * Alpha = 0：完全透明，不可见
 * Alpha = 255：完全不透明，完全可见
 * Alpha = 1-254：半透明，可以与其他图层混合显示
 */
dma2d_t dma2d = 
{
  .width                             = 480,
  .height                            = 272,
  .pixel_size                        = 2,
   // 配置DMA2D基本参数
  .hdma2d.Instance                   = DMA2D,
  .hdma2d.Init.Mode                  = DMA2D_M2M_BLEND,          // 内存到内存模式-混合渲染
  .hdma2d.Init.ColorMode             = DMA2D_OUTPUT_RGB565,      // 输出颜色格式为RGB565
  .hdma2d.Init.OutputOffset          = 0,                        // 输出偏移量为0

  .layer_num                         = 2,
  // 配置图层1参数
  .hdma2d.LayerCfg[0].InputOffset    = 0,                        // 输入偏移量为0
  .hdma2d.LayerCfg[0].InputColorMode = DMA2D_INPUT_RGB565,       // 输入颜色格式为RGB565
  .hdma2d.LayerCfg[0].AlphaMode      = DMA2D_REPLACE_ALPHA,      // Alpha模式
  .hdma2d.LayerCfg[0].InputAlpha     = 80,                       // 输入Alpha值为0
  // 配置图层1参数
  .hdma2d.LayerCfg[1].InputOffset    = 0,                        // 输入偏移量为0
  .hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565,       // 输入颜色格式为RGB565
  .hdma2d.LayerCfg[1].AlphaMode      = DMA2D_REPLACE_ALPHA,      // Alpha模式
  .hdma2d.LayerCfg[1].InputAlpha     = 90,                       // 输入Alpha值为0

  .dst_addr                          = (uint16_t *)ltdc_lcd_framebuf
};

sdram_t sdram = 
{
    .SDRAM_Handler.Instance = FMC_SDRAM_DEVICE,                               // SDRAM 在 BANK5,6
    .SDRAM_Handler.Init.SDBank = FMC_SDRAM_BANK1,                             // 第一个 SDRAM BANK
    .SDRAM_Handler.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9,       // 列数
    .SDRAM_Handler.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13,            // 行数
    .SDRAM_Handler.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16,         // 数据宽度 16 位
    .SDRAM_Handler.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4,    // 4 个内部 BANK
    .SDRAM_Handler.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3,                 // CAS = 3
    .SDRAM_Handler.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE, // 关闭写保护
    .SDRAM_Handler.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2,             // SDCLK = HCLK/2
    .SDRAM_Handler.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE,                  // 使能突发读取
    .SDRAM_Handler.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1,              // 读管线延时

    .SDRAM_Timing.LoadToActiveDelay = 2,                                      // 加载模式寄存器到激活的延迟
    .SDRAM_Timing.ExitSelfRefreshDelay = 8,                                   // 退出自刷新延迟
    .SDRAM_Timing.SelfRefreshTime = 6,                                        // 自刷新时间
    .SDRAM_Timing.RowCycleDelay = 6,                                          // 行循环延迟
    .SDRAM_Timing.WriteRecoveryTime = 2,                                      // 写恢复时间
    .SDRAM_Timing.RPDelay = 2,                                                // 行预充电延迟
    .SDRAM_Timing.RCDDelay = 2                                                // 行到列延迟
};


// INT
io_t io_h7 = 
{
    .GPIOx = GPIOH,
    .GPIO_InitStruct.Pin = GPIO_PIN_7,
    .GPIO_InitStruct.Mode = GPIO_MODE_INPUT,
    .GPIO_InitStruct.Pull  = GPIO_PULLUP,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH,
    .PinState = GPIO_PIN_RESET
};
// RST
io_t io_i8 = 
{
    .GPIOx = GPIOI,
    .GPIO_InitStruct.Pin = GPIO_PIN_8,
    .GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .GPIO_InitStruct.Pull  = GPIO_PULLUP,
    .GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH,
    .PinState = GPIO_PIN_RESET
};

iicSof_t gtxx_i2c = 
{
    .SCL.GPIOx = GPIOH,
    .SCL.GPIO_InitStruct.Pin = GPIO_PIN_6,
    .SCL.GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    .SCL.GPIO_InitStruct.Pull  = GPIO_PULLUP,
    .SCL.GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH,
    
    .SDA_OUT.GPIOx = GPIOI,
    .SDA_OUT.GPIO_InitStruct.Pin = GPIO_PIN_3,
    .SDA_OUT.GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD,
    .SDA_OUT.GPIO_InitStruct.Pull  = GPIO_NOPULL,
    .SDA_OUT.GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH,
	
    .SDA_IN.GPIOx = GPIOI,
    .SDA_IN.GPIO_InitStruct.Pin = GPIO_PIN_3,
    .SDA_IN.GPIO_InitStruct.Mode = GPIO_MODE_INPUT,
    .SDA_IN.GPIO_InitStruct.Pull  = GPIO_NOPULL,
    .SDA_IN.GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH,
};
