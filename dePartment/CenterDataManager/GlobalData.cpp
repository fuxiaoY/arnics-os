#include "ProjDefine.h"
#include "GlobalData.h"

// 系统是否允许休眠标志位

BOOL ALLOWSLEEP = FALSE;
#ifdef _CENTER_MEDIA_
BOOL MediaWantSleep = FALSE;//媒体中心休眠申请
#else
BOOL MediaWantSleep = TRUE;//媒体中心休眠申请
#endif

BOOL eventosWantSleep = FALSE;//事件OS休眠申请
BOOL ConsoleWantSleep = FALSE;//控制台休眠申请
BOOL StartGuardWantSleep = FALSE;



// 系统参数
uint8_t systemAllowSleep = 1; //是否允许休眠
uint8_t VirtualEnvironment = 0;  // 虚拟环境标志
volatile uint8_t isFirstPowerOn = 0; // 系统是否第一次开机
uint32_t system_reset_flag = 0;
uint32_t system_reset_NUM = 0;
BOOL SHELL_ACTIVE = FALSE; // 系统是否处于shell状态
BOOL FREERTOS_INIT_ISDONE = FALSE; //freertos是否初始化完毕
BOOL EVENTOS_INIT_ISDONE = FALSE; // EVENOS是否初始化完毕
time_t g_InitTs = 0;          // 系统启动时的时间戳
//数据库
uint32_t TSDBForceCleanCount = 0;


//Flash 操作对象
Module_Flash g_dataFlash;
//看门狗
Module_IWDG g_IWDG;

// RTC外设对象
Module_RTC g_RTC; // 创建Module_RTC实例

// 定时器对象
Module_TIM g_TIM15;

// GPIO外设对象
Module_GPIO gpioModule; //gpio全局初始化对象

Module_GPIO g_SDNCS(GPIOA, GPIO_PIN_12); // A12  = SD卡片选

#ifdef _DEVELOPE_BOARD_
Module_GPIO g_W25QNCS(GPIOB, GPIO_PIN_12); // B12  = W25Q片选-开发版
#else
Module_GPIO g_W25QNCS(GPIOA, GPIO_PIN_11); // A11  = W25Q片选
#endif
Module_GPIO g_TouchNCS(GPIOB, GPIO_PIN_12); // B12  = 触摸片选
Module_GPIO g_LCDNCS(GPIOA, GPIO_PIN_15); // A15  = spi屏片选



Module_GPIO g_LCD_DCRS(GPIOB,GPIO_PIN_5);// 液晶屏数据/命令控制信号
Module_GPIO g_LCDRST(GPIOB,GPIO_PIN_6);     //液晶屏复位控制信号

// 串口外设对象
/**LPUART1 GPIO Configuration
PC0     ------> LPUART1_RX
PC1     ------> LPUART1_TX
*/
Module_USART g_lpcom1(LPUART1); 




// SPI外设对象
Module_SPI g_spi1(SPI1);
Module_SPI g_spi2(SPI2);
Module_SPI g_spi3(SPI3);
// ADC外设对象
Module_ADC g_BateryCheck;

// 设备模块对象
DeviceCPP_VL53L0 g_VLDT01; // 红外测距1号板
DeviceCPP_VL53L0 g_VLDT02; // 红外测距2号板
DeviceCPP_RC522 g_RFID_RC522; // RFID读卡器
DeviceCPP_MPU6050 g_MPU6050;  //MPU6050芯片
// 全局参数

 ST_RTConfig g_RTCfg __attribute__((aligned(4))) = {0};       // 设备运行参数
 ST_RTState g_RTStat __attribute__((aligned(4))) = {0};       // 全局运行状态数据集


uint16_t DefaultBatteryCurve[25] = {2840, 2850, 2860, 3110, 3340,  // 0% - 4%
                     3400, 3480, 3540, 3560, 3640,  // 8% - 16%
                     3690, 3710, 3730, 3740, 3750,  // 20% - 28%
                     3850, 3860, 3870, 3880, 4039,  // 32% - 40%
                     4040, 4120, 4140, 4170, 4190};       // 44% - 100%



