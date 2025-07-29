#ifndef SOFT_MUTEX_H
#define SOFT_MUTEX_H 
#include "Inc/typedef.h"
#include "Inc/ProjDefine.h"

#ifdef __cplusplus
extern "C" {
#endif
/*软件互斥锁*/
typedef struct 
{
    volatile uint8_t is_init;// 0 ->未初始化, 1 ->已初始化
    volatile uint8_t lock;   // 0 ->未锁定,   1 ->已锁定
}soft_mutex_t;

/**
  * @brief  初始化软件互斥锁
  * @param  mutex: 指向互斥锁的指针
  * @retval None
  */
extern void soft_mutex_init(soft_mutex_t *mutex);

/**
  * @brief  获取软件互斥锁
  * @param  mutex: 指向互斥锁的指针
  * @retval None
  */
extern void soft_mutex_lock(soft_mutex_t *mutex);


/**
  * @brief  释放软件互斥锁
  * @param  mutex: 指向互斥锁的指针
  * @retval None
  */
extern void soft_mutex_unlock(soft_mutex_t *mutex); 

#ifdef __cplusplus
}
#endif
#endif
