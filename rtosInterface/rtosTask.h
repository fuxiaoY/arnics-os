#ifndef _RTOSTASK_H_
#define _RTOSTASK_H_ 

#ifdef __cplusplus
extern "C" {
#endif

#include "../Inc/ProjDefine.h"
#include "../Inc/typedef.h"

#ifdef _USE_FREERTOS_
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

//消息中心队列  
extern QueueHandle_t eventosReceiveQueue;
extern QueueHandle_t eventosSendQueue;
//媒体中心队列
extern QueueHandle_t MediaReceiveQueue;
extern QueueHandle_t MediaSendQueue;

//休眠中心队列
extern QueueHandle_t SleepReceiveQueue;

// 消息中心信号量
extern SemaphoreHandle_t eventosSendQueue_xSemaphore; //读消息中心队列锁
extern SemaphoreHandle_t eventosID_mutex;   //消息ID锁

//sfud锁
extern SemaphoreHandle_t flashDB_mutex;  


#endif // _USE_FREERTOS_

extern void rtosThreadDelay(uint32_t ms);

/**
 * @func StartConsleTask
 * @brief 休眠专用线程
 * @note 无
 */
extern void StartSleepTask(void const *argument);

/**
 * @func StartConsleTask
 * @brief 控制台专用线程
 * @param 无
 * @retval 无
 * @note 无
 * @attention
 */
extern void StartConsleTask(void const *argument);

/**
 * @func StartEventTask
 * @brief eventOS事件线程
 * @param 无
 * @retval 无
 * @note 无
 * @attention
 */
extern void StartEventTask(void const *argument);


/**
 * @func StartMaintTask
 * @brief 主功能线程
 * @param 无
 * @retval 无
 * @note 无
 * @attention
 */
extern void StartMaintTask(void const *argument);


/**
 * @func StartGuardTask
 * 
 * @brief 守护线程
 * @param 无
 * @retval 无
 * @note 用于喂狗操作
 * @attention
 */
extern void StartGuardTask(void const *argument);



#ifdef __cplusplus
}
#endif
#endif
