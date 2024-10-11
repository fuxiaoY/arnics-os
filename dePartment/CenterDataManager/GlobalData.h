#ifndef _GLOBAL_DATA_H_
#define _GLOBAL_DATA_H_
#include "ProjDefine.h"
#include "interface_def.h"
#include "device_def.h"
#include "Global_def.h"





// 是否允许休眠标志位
extern BOOL ALLOWSLEEP;
extern BOOL MediaWantSleep;//媒体中心休眠申请
extern BOOL eventosWantSleep;//事件OS休眠申请
extern BOOL ConsoleWantSleep;//控制台休眠申请
extern BOOL StartGuardWantSleep;//督查中心休眠申请
// 系统参数
extern uint8_t systemAllowSleep; //是否允许休眠
extern uint8_t VirtualEnvironment;  // 虚拟环境标志
extern volatile uint8_t isFirstPowerOn; // 系统是否第一次开机
extern uint32_t system_reset_flag;
extern uint32_t system_reset_NUM ;
extern BOOL SHELL_ACTIVE; // 系统是否处于shell状态
extern BOOL FREERTOS_INIT_ISDONE;
extern BOOL EVENTOS_INIT_ISDONE; // EVENOS是否初始化完毕
extern time_t g_InitTs;      // 系统启动时的时间戳
extern uint32_t TSDBForceCleanCount; // TSDB强制清理计数



//Flash 操作对象
extern Module_Flash g_dataFlash;
//看门狗
extern Module_IWDG g_IWDG;
// RTC外设对象
extern Module_RTC g_RTC; // 创建Module_RTC实例

// 定时器对象
extern Module_TIM g_TIM15;

// GPIO外设对象
extern Module_GPIO gpioModule; //gpio全局初始化对象

extern Module_GPIO g_SDNCS; // A12  = SD卡片选
extern Module_GPIO g_W25QNCS; // B0  = W25Q片选
extern Module_GPIO g_TouchNCS; // B12  = 触摸屏片选
extern Module_GPIO g_LCDNCS; // A15  = spi屏片选

extern Module_GPIO g_LCD_DCRS;//液晶屏数据/命令控制信号
extern Module_GPIO g_LCDRST;     //液晶屏复位控制信号


// 串口外设对象
extern Module_USART g_lpcom1; // 调试串口


// 串口外设对象的别名
#define g_ComDBG g_lpcom1
#define g_ComIAP g_lpcom1

#define g_UARTDBG hlpuart1

// 串口外设对象的别名



//I2C外设对象的别名

// SPI外设对象
extern Module_SPI g_spi1;
extern Module_SPI g_spi2;
extern Module_SPI g_spi3;
//SPI外设对象的别名
#ifdef _DEVELOPE_BOARD_
#define g_W25Q g_spi2
#else
#define g_W25Q g_spi1
#endif

#define g_Touch g_spi2
#define g_LCD g_spi3
// ADC外设对象

extern Module_ADC g_BateryCheck;



// 业务逻辑全局变量


 extern ST_RTConfig g_RTCfg;       // 设备运行参数
 extern ST_RTState g_RTStat;       // 全局运行状态数据集


extern uint16_t DefaultBatteryCurve[25];


#endif
