/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "../../Inc/projDefine.h"
#include "../../Inc/typedef.h"

#include "../../dataPlat/globalDef.h"
#include "../rtosTask.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"


/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
osThreadId consleTaskHandle;
osThreadId GuardTaskHandle;
osThreadId mainTaskHandle;
osThreadId eventTaskHandle;
osThreadId sleepTaskHandle;
osThreadId MediaTaskHandle;
//消息中心队列
QueueHandle_t eventosReqQueue;
QueueHandle_t eventosRspQueue;
//媒体中心队列
QueueHandle_t MediaReqQueue;
QueueHandle_t MediaRspQueue;
//行政中心队列
QueueHandle_t adReqQueue;
QueueHandle_t adRspQueue;

// 消息中心信号量
SemaphoreHandle_t eventosRspQueue_xSemaphore; //读消息中心队列锁
SemaphoreHandle_t eventosID_mutex;   //消息ID锁
// 媒体中心信号量
SemaphoreHandle_t MediaRspQueue_xSemaphore; //读媒体中心队列锁

//sfud锁
SemaphoreHandle_t flashDB_mutex;  


/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* Private variables ---------------------------------------------------------*/

#ifdef _USE_FREERTOS_MONITOR_
osThreadId cpuTaskHandle;
/*
(1)FreeRTOS统计CPU占用时间使用的是一个32bit无符号整形来记录系统时间的，而统计时间时没有对统计时间的变量做溢出保护，当定时器精度为50us时，32bit无符号整形支持的最大计数时间为：
Tmax = 2^32 * 50us/1000100060=59.6分钟
当运行时间超过59.6分钟时，统计结果将不准确。
(2)高精度定时器频繁的进入中断也会对系统性能造成一定的影响。


任务状态：任务的状态，用字母表示。常见的状态有：
R: 运行（Running）
B: 阻塞（Blocked）
S: 挂起（Suspended）
D: 删除（Deleted）
优先级：任务的优先级，数值越大优先级越高。
剩余堆栈：任务堆栈中剩余的空间，单位是字（对于32位系统，1个字等于4个字节）。
任务序号：任务在任务列表中的序号。
*/
void clear_screen()
{
    printf("\033[2J"); // 清屏
    printf("\033[H");  // 将光标移动到屏幕左上角
}
void CPU_Task(void const *argument)
{
  uint8_t CPU_RunInfo[400]; // 保存任务运行时间信息
  char CPU_left_stack[50]= {0}; // 剩余堆栈
  while (1)
  {

    clear_screen(); // 清屏
    memset(CPU_RunInfo, 0, 400); // 信息缓冲区清零

    vTaskList((char *)&CPU_RunInfo); // 获取任务运行时间信息

    printf("---------------------------------------------\r\n");
    printf("任务名      任务状态 优先级   剩余栈 任务序号 \r\n");
    printf("%s", CPU_RunInfo);
    printf("---------------------------------------------\r\n");

    memset(CPU_RunInfo, 0, 400); // 信息缓冲区清零

    vTaskGetRunTimeStats((char *)&CPU_RunInfo);

    printf("任务名       运行计数         利用率  \r\n");
    printf("%s", CPU_RunInfo);
    printf("---------------------------------------------\r\n");
    memset(CPU_left_stack, 0, 50); // 信息缓冲区清零
    sprintf(CPU_left_stack,"当前剩余动态内存大小                  %u Bytes\r\n ",xPortGetFreeHeapSize());
    printf("%s",CPU_left_stack);

    memset(CPU_left_stack, 0, 50); // 信息缓冲区清零
		sprintf(CPU_left_stack,"系统启动至当前时刻的动态内存最小剩余 %u Bytes\r\n ",xPortGetMinimumEverFreeHeapSize());
    printf("%s",CPU_left_stack);
    printf("---------------------------------------------\r\n\n");
    vTaskDelay(1000); /* 延时1000个tick */
  }
}
#endif
/**
  * @brief  信号量初始化
  * @param  None
  * @retval None
  */

void initSemaphore() 
{
    // Create the semaphore
    eventosRspQueue_xSemaphore = xSemaphoreCreateMutex(); // Create a mutex semaphore
    eventosID_mutex = xSemaphoreCreateMutex(); // Create a mutex semaphore
    MediaRspQueue_xSemaphore = xSemaphoreCreateMutex(); // Create a mutex semaphore

    flashDB_mutex = xSemaphoreCreateMutex(); // Create a mutex semaphore
}
/**
  * @brief  队列初始化
  * @param  None
  * @retval None
  */

void initQueue() 
{
    eventosReqQueue = xQueueCreate(3, sizeof(Message_t));  // 创建一个可以存储 3 个 Message_t 类型消息的队列
    eventosRspQueue = xQueueCreate(3, sizeof(Message_t));     // 创建一个可以存储 3 个 Message_t 类型消息的队列

    MediaReqQueue = xQueueCreate(3, sizeof(MediaMessage_t));  // 创建一个可以存储 3 个 MediaMessage_t 类型消息的队列
    MediaRspQueue = xQueueCreate(3, sizeof(MediaMessage_t));     // 创建一个可以存储 3 个 MediaMessage_t 类型消息的队列  

    adReqQueue = xQueueCreate(1, sizeof(adMessage_t));  // 创建一个可以存储 1 个 adMessage_t 类型消息的队列
    adReqQueue = xQueueCreate(1, sizeof(adMessage_t));  // 创建一个可以存储 1 个 adMessage_t 类型消息的队列
}
/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) 
{
  initSemaphore();
  initQueue();
  /* definition and creation of defaultTask */
  osThreadDef(ConsleTask, StartConsleTask, osPriorityNormal, 0, 1024);
  consleTaskHandle = osThreadCreate(osThread(ConsleTask), NULL);

  osThreadDef(eventTask, StartEventTask, osPriorityNormal, 0, 640);
  eventTaskHandle = osThreadCreate(osThread(eventTask), NULL);
  
  osThreadDef(MediaTask, StartMediaTask, osPriorityNormal, 0, 640);
  MediaTaskHandle = osThreadCreate(osThread(MediaTask), NULL);

  osThreadDef(mainTask, StartMaintTask, osPriorityNormal, 0, 640);
  mainTaskHandle = osThreadCreate(osThread(mainTask), NULL);

  osThreadDef(GuardTask, StartGuardTask, osPriorityLow, 0, 512);
  GuardTaskHandle = osThreadCreate(osThread(GuardTask), NULL);
#ifdef _USE_FREERTOS_MONITOR_
  osThreadDef(CPUTask, CPU_Task, osPriorityHigh, 0, 256);
  cpuTaskHandle = osThreadCreate(osThread(CPUTask), NULL);
#endif
  osThreadDef(SleepTask, StartSleepTask, osPriorityRealtime, 0, 128);
  sleepTaskHandle = osThreadCreate(osThread(SleepTask), NULL);
}



