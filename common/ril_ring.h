/* information */
/**
  ******************************************************************************
  * @file           : RIL_RING_H
  * @brief          : ���λ�������ص�ͷ�ļ�
  * 
  * ���ļ������˻��λ���������غ궨�塢�����ͺ���������
  * ��Ҫ���ڵ��Դ��ڵ��շ����ݹ���
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

#include "../Inc/projDefine.h"      ///< ��Ŀ����ͷ�ļ�
#include "../Inc/typedef.h"         ///< ���Ͷ���ͷ�ļ�
#include "../thirdParty/thirdPartyInclude.h"  ///< ��������ͷ�ļ�

/* typedef -----------------------------------------------------------*/

/* define ------------------------------------------------------------*/
/**
 * @def _USE_RING_
 * @brief �Ƿ����û��λ�����
 * 
 * ��������˸ú꣬�����û��λ��������ܡ�
 */
#ifdef _USE_RING_

/**
 * @def DEBUG_UART_RXBUF_SIZE
 * @brief ���ջ�������С
 * 
 * ������Դ��ڽ��ջ������Ĵ�СΪ512�ֽڡ�
 */
#define DEBUG_UART_RXBUF_SIZE  512

/**
 * @def DEBUG_UART_TXBUF_SIZE
 * @brief ���ͻ�������С
 * 
 * ������Դ��ڷ��ͻ������Ĵ�СΪ512�ֽڡ�
 */
#define DEBUG_UART_TXBUF_SIZE  512
/* macro -------------------------------------------------------------*/

/* variables ---------------------------------------------------------*/
/**
 * @var debug_rxbuf
 * @brief ���ջ�����
 * 
 * ���ڴ洢�ӵ��Դ��ڽ��յ������ݡ�
 */
extern unsigned char debug_rxbuf[DEBUG_UART_RXBUF_SIZE];

/**
 * @var debug_txbuf
 * @brief ���ͻ�����
 * 
 * ���ڴ洢�����͵����Դ��ڵ����ݡ�
 */
extern unsigned char debug_txbuf[DEBUG_UART_TXBUF_SIZE];

/**
 * @var debug_rbsend
 * @brief ���ͻ���������ṹ��
 * 
 * ���ڹ����ͻ������Ļ��λ������ṹ�塣
 */
extern ring_buf_t debug_rbsend;

/**
 * @var debug_rbrecv
 * @brief ���ջ���������ṹ��
 * 
 * ���ڹ�����ջ������Ļ��λ������ṹ�塣
 */
extern ring_buf_t debug_rbrecv;

/* function prototypes -----------------------------------------------*/
/**
 * @fn user_debug_uart_is_len
 * @brief ��ȡ���Դ��ڻ������е����ݳ���
 * 
 * @return ���ػ������е����ݳ���
 */
extern unsigned int user_debug_uart_is_len(void);

/**
 * @fn user_debug_uart_write
 * @brief ����Դ���д������
 * 
 * @param buf Ҫд������ݻ�����
 * @param len Ҫд������ݳ���
 * @return ʵ��д������ݳ���
 */
extern unsigned int user_debug_uart_write(const void *buf, unsigned int len);

/**
 * @fn user_debug_uart_read
 * @brief �ӵ��Դ��ڶ�ȡ����
 * 
 * @param buf �洢��ȡ���ݵĻ�����
 * @param len Ҫ��ȡ�����ݳ���
 * @return ʵ�ʶ�ȡ�����ݳ���
 */
extern unsigned int user_debug_uart_read(void *buf, unsigned int len);

/**
 * @fn debug_uart_write_byte
 * @brief ����Դ���д��һ���ֽ�
 * 
 * @param byteData Ҫд����ֽ�����
 * @return д��ɹ�����1��ʧ�ܷ���0
 */
extern unsigned int debug_uart_write_byte(unsigned char* byteData);

/**
 * @fn debug_uart_read_byte
 * @brief �ӵ��Դ��ڶ�ȡһ���ֽ�
 * 
 * @param byteData �洢��ȡ�ֽ����ݵ�ָ��
 * @return ��ȡ�ɹ�����1��ʧ�ܷ���0
 */
extern unsigned int debug_uart_read_byte(unsigned char *byteData);

/**
 * @fn debug_uart_send_String
 * @brief ����Դ��ڷ����ַ���
 * 
 * @param Str Ҫ���͵��ַ���
 */
extern void debug_uart_send_String(const char *Str);

/**
 * @fn ring_init
 * @brief ��ʼ�����λ�����
 * 
 * ��ʼ�����ͺͽ��յĻ��λ�������
 */
extern void ring_init(void);

#endif // _USE_RING_

#ifdef __cplusplus
}
#endif

#endif // ! RIL_RING_H
/************************ (C) COPYRIGHT ARSTUDIO *****END OF FILE***************************/
