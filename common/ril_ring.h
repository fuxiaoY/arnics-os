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
#ifndef  RIL_RING_H
#define  RIL_RING_H
/* Includes ----------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#include "../Inc/projDefine.h"      ///< 项目定义头文件
#include "../Inc/typedef.h"         ///< 类型定义头文件
#include "../thirdParty/thirdPartyInclude.h"  ///< 第三方库头文件

/* typedef -----------------------------------------------------------*/

/* define ------------------------------------------------------------*/
/**
 * @def _USE_RING_
 * @brief 是否启用环形缓冲区
 * 
 * 如果定义了该宏，则启用环形缓冲区功能。
 */
#ifdef _USE_RING_

/**
 * @def DEBUG_UART_RXBUF_SIZE
 * @brief 接收缓冲区大小
 * 
 * 定义调试串口接收缓冲区的大小为512字节。
 */
#define DEBUG_UART_RXBUF_SIZE  512

/**
 * @def DEBUG_UART_TXBUF_SIZE
 * @brief 发送缓冲区大小
 * 
 * 定义调试串口发送缓冲区的大小为512字节。
 */
#define DEBUG_UART_TXBUF_SIZE  512
/* macro -------------------------------------------------------------*/

/* variables ---------------------------------------------------------*/
/**
 * @var debug_rxbuf
 * @brief 接收缓冲区
 * 
 * 用于存储从调试串口接收到的数据。
 */
extern unsigned char debug_rxbuf[DEBUG_UART_RXBUF_SIZE];

/**
 * @var debug_txbuf
 * @brief 发送缓冲区
 * 
 * 用于存储待发送到调试串口的数据。
 */
extern unsigned char debug_txbuf[DEBUG_UART_TXBUF_SIZE];

/**
 * @var debug_rbsend
 * @brief 发送缓冲区管理结构体
 * 
 * 用于管理发送缓冲区的环形缓冲区结构体。
 */
extern ring_buf_t debug_rbsend;

/**
 * @var debug_rbrecv
 * @brief 接收缓冲区管理结构体
 * 
 * 用于管理接收缓冲区的环形缓冲区结构体。
 */
extern ring_buf_t debug_rbrecv;

/* function prototypes -----------------------------------------------*/
/**
 * @fn user_debug_uart_is_len
 * @brief 获取调试串口缓冲区中的数据长度
 * 
 * @return 返回缓冲区中的数据长度
 */
extern unsigned int user_debug_uart_is_len(void);

/**
 * @fn user_debug_uart_write
 * @brief 向调试串口写入数据
 * 
 * @param buf 要写入的数据缓冲区
 * @param len 要写入的数据长度
 * @return 实际写入的数据长度
 */
extern unsigned int user_debug_uart_write(const void *buf, unsigned int len);

/**
 * @fn user_debug_uart_read
 * @brief 从调试串口读取数据
 * 
 * @param buf 存储读取数据的缓冲区
 * @param len 要读取的数据长度
 * @return 实际读取的数据长度
 */
extern unsigned int user_debug_uart_read(void *buf, unsigned int len);

/**
 * @fn debug_uart_write_byte
 * @brief 向调试串口写入一个字节
 * 
 * @param byteData 要写入的字节数据
 * @return 写入成功返回1，失败返回0
 */
extern unsigned int debug_uart_write_byte(unsigned char* byteData);

/**
 * @fn debug_uart_read_byte
 * @brief 从调试串口读取一个字节
 * 
 * @param byteData 存储读取字节数据的指针
 * @return 读取成功返回1，失败返回0
 */
extern unsigned int debug_uart_read_byte(unsigned char *byteData);

/**
 * @fn debug_uart_send_String
 * @brief 向调试串口发送字符串
 * 
 * @param Str 要发送的字符串
 */
extern void debug_uart_send_String(const char *Str);

/**
 * @fn ring_init
 * @brief 初始化环形缓冲区
 * 
 * 初始化发送和接收的环形缓冲区。
 */
extern void ring_init(void);

#endif // _USE_RING_

#ifdef __cplusplus
}
#endif

#endif // ! RIL_RING_H
/************************ (C) COPYRIGHT ARSTUDIO *****END OF FILE***************************/
