/* USER CODE BEGIN Header */
/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */
 /* USER CODE END Header */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H
#include "../../Inc/projDefine.h"
/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * These parameters and more are described within the 'configuration' section of the
 * FreeRTOS API documentation available on the FreeRTOS.org web site.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* USER CODE BEGIN Includes */   	      
/* Section where include file can be added */
/* USER CODE END Includes */ 

/* Ensure definitions are only used by the compiler, and not by the assembler. */
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
  #include <stdint.h>
  extern uint32_t SystemCoreClock;
  void xPortSysTickHandler(void);
#endif
#define configUSE_PREEMPTION                     1
//#define configUSE_RECURSIVE_MUTEXES              1
#define configSUPPORT_STATIC_ALLOCATION          0
#define configSUPPORT_DYNAMIC_ALLOCATION         1
#define configUSE_IDLE_HOOK                      0
#define configUSE_TICK_HOOK                      0
#define configCPU_CLOCK_HZ                       ( SystemCoreClock )
#define configTICK_RATE_HZ                       ((TickType_t)1000)
#define configMAX_PRIORITIES                     ( 7 )
#define configMINIMAL_STACK_SIZE                 ((uint16_t)128)
#define configTOTAL_HEAP_SIZE                    ((size_t)20 * 1024)  //freertos的堆大小（字节数为单位）20KB
#define configMAX_TASK_NAME_LEN                  ( 16 )
#define configUSE_16_BIT_TICKS                   0
#define configUSE_MUTEXES                        1
#define configQUEUE_REGISTRY_SIZE                8
#define configUSE_PORT_OPTIMISED_TASK_SELECTION  1
#define configUSE_QUEUE_SETS                     1 //开启消息队列
#define configUSE_COUNTING_SEMAPHORES            1 //使能信号量
#define configUSE_TASK_NOTIFICATIONS             1  //开启任务通知           
#define rtosScheduling                     100 // 单位：ms  系统调度的最小延时时间
#ifdef _USE_FREERTOS_MONITOR_
/* 
该函数返回 所在任务自启动以来 的最小任务栈空间大小，单位为 字（求字节数 再 ×4 ）
在一个任务中调用该API：
printf(" the min free stack size is %d \r\n",(int32_t)uxTaskGetStackHighWaterMark(NULL));  
 */
#define INCLUDE_uxTaskGetStackHighWaterMark       1 //使能获取任务栈使用情况
//启用运行时间统计功能 
#define configGENERATE_RUN_TIME_STATS 1 //使能获取任务运行时间 
#define configUSE_STATS_FORMATTING_FUNCTIONS 1 //使能获取任务运行时间
//启用可视化跟踪调试 
#define configUSE_TRACE_FACILITY 1 
/* 与宏 configUSE_TRACE_FACILITY 同时为 1 时会编译下面 3 个函数
* prvWriteNameToBuffer()
* vTaskList(),
* vTaskGetRunTimeStats()
*/
extern volatile uint32_t CPU_RunTime; 

#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() (CPU_RunTime = 0ul) 
#define portGET_RUN_TIME_COUNTER_VALUE() CPU_RunTime
#else
#define INCLUDE_uxTaskGetStackHighWaterMark       1 //使能获取任务栈使用情况
#define configUSE_STATS_FORMATTING_FUNCTIONS 1 //使能获取任务运行时间
//启用可视化跟踪调试 
#define configUSE_TRACE_FACILITY 1 
/* 与宏 configUSE_TRACE_FACILITY 同时为 1 时会编译下面 3 个函数
* prvWriteNameToBuffer()
* vTaskList(),
* vTaskGetRunTimeStats()
*/
#endif
//其他监测手段
#ifdef _USE_FREERTOS_SYSTEM_MONITOR_
#define configCHECK_FOR_STACK_OVERFLOW  1
#define configUSE_MALLOC_FAILED_HOOK 1
#endif

#ifdef NOT_ALLOWED_SLEEP
#define configUSE_TICKLESS_IDLE                     0 //使能低功耗模式
#else
#define configUSE_TICKLESS_IDLE                     1 //使能低功耗模式
#endif

#define configEXPECTED_IDLE_TIME_BEFORE_SLEEP       2  // 空闲时间阈值（Tick 数）该值将衡量所有任务的最低延时，来判断是否允许进入休眠
//#define configCHECK_FOR_STACK_OVERFLOW            1 //使能获取任务栈溢出情况

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES                    0
#define configMAX_CO_ROUTINE_PRIORITIES          ( 2 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet            1
#define INCLUDE_uxTaskPriorityGet           1
#define INCLUDE_vTaskDelete                 1
#define INCLUDE_vTaskCleanUpResources       0
#define INCLUDE_vTaskSuspend                1
#define INCLUDE_vTaskDelayUntil             1
#define INCLUDE_vTaskDelay                  1
#define INCLUDE_xTaskGetSchedulerState      1

/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
 /* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
 #define configPRIO_BITS         __NVIC_PRIO_BITS
#else
 #define configPRIO_BITS         4
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY   15

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
/* USER CODE BEGIN 1 */
#define configASSERT( x ) if ((x) == 0) {taskDISABLE_INTERRUPTS(); for( ;; );} 
/* USER CODE END 1 */

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler    SVC_Handler
#define xPortPendSVHandler PendSV_Handler

/* IMPORTANT: This define is commented when used with STM32Cube firmware, when the timebase source is SysTick,
              to prevent overwriting SysTick_Handler defined within STM32Cube HAL */
/* #define xPortSysTickHandler SysTick_Handler */

/* USER CODE BEGIN Defines */   	      
/* Section where parameter definitions can be added (for instance, to override default ones in FreeRTOS.h) */
/* USER CODE END Defines */ 

#endif /* FREERTOS_CONFIG_H */
