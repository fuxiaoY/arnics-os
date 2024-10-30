/* information */
/**
  ******************************************************************************
  * @file           : RIL_RING_H
  * @brief          : 环形缓冲区相关的头文件
  * 
  * 该文件定义了环形缓冲区的相关宏定义、变量和函数声明。
  * 主要用于调试串口的收发数据管理。
  * 
  * @version        : 1.0.0
  * @date           : 2020-10-01
  ******************************************************************************

  * @attention
  *
  * Copyright (c) 2023 ARSTUDIO.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************

  */
/* information */
#ifndef  _RING_H
#define  _RING_H
/* Includes ----------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#include "../../Inc/projDefine.h"      ///< 项目定义头文件
#include "../../Inc/typedef.h"         ///< 类型定义头文件
#include "ringbuffer.h"  ///< 第三方库头文件

/* typedef -----------------------------------------------------------*/


/* function prototypes -----------------------------------------------*/
extern unsigned int ring_write(ring_buf_t *rbsend, void *buf, unsigned int len);

extern unsigned int ring_read(ring_buf_t *rbrev, void *buf, unsigned int len);

extern void ring_rxclear(ring_buf_t *rbrev);

extern void ring_txclear(ring_buf_t *rbsend);


/**
 * @fn ring_init
 * @brief 初始化环形缓冲区
 * 
 * 初始化发送和接收的环形缓冲区。
 */
extern void ring_init(ring_buf_t *rbsend,unsigned char *bufsend,ring_buf_t *rbrev,unsigned char *bufrev);



#ifdef __cplusplus
}
#endif

#endif // ! RIL_RING_H
/************************ (C) COPYRIGHT ARSTUDIO *****END OF FILE***************************/
