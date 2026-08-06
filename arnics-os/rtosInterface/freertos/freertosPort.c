#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#if PLATFORM_MCU
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "rtosInterface/rtosInterfacePublic.h"
#include "rtosInterface/queue/queue_port.h"
#include "dePartment/centerEvent/entry_event_api.h"
#include "dePartment/centerMedia/entry_media_api.h"
#include "dePartment/centerAdministrative/entry_ad_api.h"



/* Private define ------------------------------------------------------------*/
osThreadId                    initTaskHandle;
osThreadId                  consleTaskHandle;
osThreadId                   GuardTaskHandle;
osThreadId                    mainTaskHandle;
osThreadId                   eventTaskHandle;
osThreadId                   sleepTaskHandle;
osThreadId                   MediaTaskHandle;
/*---------------------------------------------------------------------------------------*/

/*-系统监控-------------------------------------------------------------------------------*/
void cpuInfo(void)
{
    uint8_t CPU_RunInfo[400]; // 保存任务运行时间信息
    char CPU_left_stack[100]= {0}; // 剩余堆栈
    memset(CPU_RunInfo, 0, 400); // 信息缓冲区清零
    vTaskList((char *)&CPU_RunInfo); // 获取任务运行时间信息

    printf("---------------------------------------------\r\n");
    printf("任务名      任务状态 优先级   剩余栈 任务序号 \r\n");
    printf("%s", CPU_RunInfo);
    printf("---------------------------------------------\r\n");
    memset(CPU_left_stack, 0, 100); // 信息缓冲区清零
    snprintf(CPU_left_stack, sizeof(CPU_left_stack), "当前剩余动态内存大小                  %u Bytes\r\n ",xPortGetFreeHeapSize());
    printf("%s",CPU_left_stack);

    memset(CPU_left_stack, 0, 100); // 信息缓冲区清零
	snprintf(CPU_left_stack, sizeof(CPU_left_stack), "系统启动至当前时刻的动态内存最小剩余 %u Bytes\r\n ",xPortGetMinimumEverFreeHeapSize());
    printf("%s",CPU_left_stack);
    printf("---------------------------------------------\r\n\n");

  
}
/*---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/

// 系统函数
void rtosTaskSuspendAll(void)
{
    vTaskSuspendAll();
}
void rtosTaskResumeAll(void)
{
    xTaskResumeAll();
}
void rtosThreadDelayUntil(uint32_t time)
{
    TickType_t xLastWakeTime;
    TickType_t xFrequency = pdMS_TO_TICKS(time); // 时间

    // 初始化时刻
    xLastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil(&xLastWakeTime, xFrequency); // 唤醒
}

void rtosThreadDelay(uint32_t ms)
{
    osDelay(ms);
}
void rtosTaskCreate(char* name, \
                            rtosPriority_e priority, \
                            void* func, \
                            uint32_t stackSize, \
                            void* arg)
{
    if (func == NULL || stackSize == 0) 
    {
        return;
    }
    const osThreadDef_t os_thread_def = 
    {(name), (os_pthread)(func),(osPriority)(priority),0,(stackSize)};
    osThreadCreate(&os_thread_def, arg);
}
void rtosTaskSelfDelete(void)
{
    vTaskDelete(NULL);
}

/* 钩子函数，当内存分配失败时被调用 */
void vApplicationMallocFailedHook(void)
{
    /* 这是一个错误情况，通常意味着系统没有足够的内存来继续运行 */
    /* 打印错误消息 */
    printf("Malloc Failed Hook called\n");

    /* 此时系统可能处于不稳定状态 */
    cpuInfo();
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    /* 停止所有任务调度，以便安全地处理错误 */
    vTaskSuspendAll();

    /* 记录栈溢出错误到某个存储区域或打印到控制台 */
    printf("Stack overflow detected in task: %s", pcTaskName);
    /* 你可以在这里添加其他的错误处理代码，比如重启系统、进入安全模式等 */
    /* 此时系统可能处于不稳定状态 */
    cpuInfo();
    /* 停止系统，因为栈溢出通常是严重的错误，需要干预 */
    for (;;)
    {
        /* 循环等待，或者执行一些简单的错误处理任务 */
    }
}




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
  char CPU_left_stack[100]= {0}; // 剩余堆栈
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
    memset(CPU_left_stack, 0, 100); // 信息缓冲区清零
    snprintf(CPU_left_stack, sizeof(CPU_left_stack), "当前剩余动态内存大小                  %u Bytes\r\n ",xPortGetFreeHeapSize());
    printf("%s",CPU_left_stack);

    memset(CPU_left_stack, 0, 100); // 信息缓冲区清零
	  snprintf(CPU_left_stack, sizeof(CPU_left_stack), "系统启动至当前时刻的动态内存最小剩余 %u Bytes\r\n ",xPortGetMinimumEverFreeHeapSize());
    printf("%s",CPU_left_stack);
    printf("---------------------------------------------\r\n\n");
    vTaskDelay(1000); /* 延时1000个tick */
  }
}
#endif
void os_task_create(void)
{
    /* 统一队列初始化 */
    queue_init_all();
    /* 统一互斥锁初始化 */
    compat_init_mutexes();
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
    osThreadDef(AdTask, StartAdTask, osPriorityRealtime, 0, 128);
    sleepTaskHandle = osThreadCreate(osThread(AdTask), NULL);
}
/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void freertos_task_init(void) 
{
    osThreadDef(initTask, StartInitTask, osPriorityRealtime, 0, 500);
    initTaskHandle = osThreadCreate(osThread(initTask), NULL);
    osKernelStart();
}




#endif 
