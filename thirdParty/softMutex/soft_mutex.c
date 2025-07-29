#include "soft_mutex.h"
#include "rtosInterface/entry_rtos_api.h"



/**
  * @brief  初始化软件互斥锁
  * @param  mutex: 指向互斥锁的指针
  * @retval None
  */
void soft_mutex_init(soft_mutex_t *mutex) 
{
    mutex->is_init = 1;
    mutex->lock = 0;
}

/**
  * @brief  获取软件互斥锁
  * @param  mutex: 指向互斥锁的指针
  * @retval None
  */
void soft_mutex_lock(soft_mutex_t *mutex) 
{
    if(0 == mutex->is_init)
    {
        return;
    }
    // 确保原子操作
    rtosTaskSuspendAll();
    while (mutex->lock) 
    {
        rtosTaskResumeAll();
        rtosThreadDelay(10);
        rtosTaskSuspendAll();
    }
    mutex->lock = 1;
    rtosTaskResumeAll();
}

/**
  * @brief  释放软件互斥锁
  * @param  mutex: 指向互斥锁的指针
  * @retval None
  */
void soft_mutex_unlock(soft_mutex_t *mutex) 
{
    if(0 == mutex->is_init)
    {
        return;
    }
    rtosTaskSuspendAll();
    mutex->lock = 0;
    rtosTaskResumeAll();
}

