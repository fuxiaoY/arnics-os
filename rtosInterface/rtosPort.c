#include "rtosPort.h"

#ifdef _USE_FREERTOS_


//消息中心队列  
extern QueueHandle_t eventosReqQueue;
extern QueueHandle_t eventosRspQueue;
//媒体中心队列
extern QueueHandle_t MediaReqQueue;
extern QueueHandle_t MediaRspQueue;
//行政中心队列
extern QueueHandle_t adReqQueue;
extern QueueHandle_t adRspQueue;

// 消息中心信号量
extern SemaphoreHandle_t eventosRspQueue_xSemaphore; //读消息中心队列锁
extern SemaphoreHandle_t eventosID_mutex;   //消息ID锁
// 媒体中心信号量
extern SemaphoreHandle_t MediaRspQueue_xSemaphore; //读媒体中心队列锁
// 行政中心信号量
extern SemaphoreHandle_t AdRspQueue_xSemaphore; //读行政中心队列锁
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

/*---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/
// 消息队列

/*---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/
// 行政管理部门队列
bool rtosAdGetMsg(void *msg,uint32_t delay)
{
    return (xQueueReceive(adReqQueue, msg, delay) == pdTRUE);
}
bool rtosAdSendMsg(void *msg,uint32_t delay)
{
    return (xQueueSend(adRspQueue, msg, delay) == pdPASS);
}
bool rtosTakeMsgFromAd(void *msg,uint32_t delay)
{
    return (xQueueReceive(adRspQueue, msg, delay) == pdTRUE);
}
bool rtosDeliverMsgToAd(void *msg,uint32_t delay)
{
    return (xQueueSend(adReqQueue, msg, delay) == pdTRUE);
}
// 获取读队列互斥信号量
bool TakeAdMsgQueueMutex(time_t waitTime)
{
    return (xSemaphoreTake(AdRspQueue_xSemaphore, waitTime) == pdTRUE);
}

// 释放读队列互斥信号量
void ReleaseAdMsgQueueMutex(void)
{
    xSemaphoreGive(AdRspQueue_xSemaphore);
}


// 检查请求队列中是否有可用空间
bool CheckAdqueueSpacesAvailable(void)
{
    return uxQueueSpacesAvailable(adReqQueue);
}
uint32_t  CheckAdRspMesgNum(void)
{
    return (uint32_t)uxQueueMessagesWaiting(adRspQueue);
}

uint32_t  CheckAdReqMesgNum(void)
{
    return (uint32_t)uxQueueMessagesWaiting(adReqQueue);
}
bool  PeekAdRspMesg(void *receivedMsg)
{
    // 使用队列的Peek功能查看消息而不取出
    return  (uint32_t)xQueuePeek(adRspQueue, receivedMsg, 0)== pdTRUE;
}

// 检查请求队列中是否有可用空间
bool CheckAdQueueSpacesAvailable(void)
{
    return uxQueueSpacesAvailable(adReqQueue);
}

/*---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/
// 事件中心队列

bool rtosEventosGetMsg(void *msg,uint32_t delay)
{
    return (xQueueReceive(eventosReqQueue, msg, delay) == pdTRUE);
}

bool rtosEventosSendMsg(void *msg,uint32_t delay)
{
    return (xQueueSend(eventosRspQueue, msg, delay) == pdPASS);
}

bool rtosTakeMsgFromEventos(void *msg,uint32_t delay)
{
    return (xQueueReceive(eventosRspQueue, msg, delay) == pdTRUE);
}
bool rtosDeliverMsgToEventos(void *msg,uint32_t delay)
{
    return (xQueueSend(eventosReqQueue, msg, delay) == pdTRUE);
}

// 获取读队列互斥信号量
bool TakeEventosMsgQueueMutex(time_t waitTime)
{
    return (xSemaphoreTake(eventosRspQueue_xSemaphore, waitTime) == pdTRUE);
}

// 释放读队列互斥信号量
void ReleaseEventosMsgQueueMutex(void)
{
    xSemaphoreGive(eventosRspQueue_xSemaphore);
}


// 获取事件中心id互斥信号量
bool TakeEventosMutex(time_t waitTime)
{
    return (xSemaphoreTake(eventosID_mutex, waitTime) == pdTRUE);
}

// 释放事件中心id互斥信号量
void ReleaseEventosMutex(void)
{
    xSemaphoreGive(eventosID_mutex);
}

uint32_t  CheckEventRspMesgNum(void)
{
    return (uint32_t)uxQueueMessagesWaiting(eventosRspQueue);
}

bool  PeekEventRspMesg(void *receivedMsg)
{
    // 使用队列的Peek功能查看消息而不取出
    return  (uint32_t)xQueuePeek(eventosRspQueue, receivedMsg, 0)== pdTRUE;
}

// 检查请求队列中是否有可用空间
bool CheckEventQueueSpacesAvailable(void)
{
    return uxQueueSpacesAvailable(eventosReqQueue);
}
/*---------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/
// 媒体中心队列
bool rtosMediaGetMsg(void *msg,uint32_t delay)
{
    return (xQueueReceive(MediaReqQueue, msg, delay) == pdTRUE);
}

bool rtosMediaSendMsg(void *msg,uint32_t delay)
{
    return (xQueueSend(MediaRspQueue, msg, delay) == pdPASS);
}

bool rtosTakeMsgFromMedia(void *msg,uint32_t delay)
{
    return (xQueueReceive(MediaRspQueue, msg, delay) == pdTRUE);
}

bool rtosDeliverMsgToMedia(void *msg,uint32_t delay)
{
    return (xQueueSend(MediaReqQueue, msg, delay) == pdTRUE);
}

// 获取读队列互斥信号量
bool TakeMediaMutex(time_t waitTime)
{
    return (xSemaphoreTake(MediaRspQueue_xSemaphore, waitTime) == pdTRUE);
}
// 释放读队列互斥信号量
void ReleaseMediaMsgQueueMutex(void)
{
    xSemaphoreGive(MediaRspQueue_xSemaphore);
}

uint32_t  CheckMediaRspMesgNum(void)
{
    return (uint32_t)uxQueueMessagesWaiting(MediaRspQueue);
}

uint32_t  CheckMediaReqMesgNum(void)
{
    return (uint32_t)uxQueueMessagesWaiting(MediaReqQueue);
}
bool  PeekMediaRspMesg(void *receivedMsg)
{
    // 使用队列的Peek功能查看消息而不取出
    return  (uint32_t)xQueuePeek(MediaRspQueue, receivedMsg, 0)== pdTRUE;
}
// 检查请求队列中是否有可用空间
bool CheckMediaQueueSpacesAvailable(void)
{
    return uxQueueSpacesAvailable(MediaReqQueue);
}


#else
void rtosThreadDelay(uint32_t ms)
{
}

bool rtosNonBlockingEventQueueReceive(void *msg)
{

    return false;

}

bool rtosBlockingEventQueueReceive(void *msg)
{
    while(1);
    return false;
}

#endif // _USE_FREERTOS_
