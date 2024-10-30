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
#ifndef  _RING_H
#define  _RING_H
/* Includes ----------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#include "../../Inc/projDefine.h"      ///< ��Ŀ����ͷ�ļ�
#include "../../Inc/typedef.h"         ///< ���Ͷ���ͷ�ļ�
#include "ringbuffer.h"  ///< ��������ͷ�ļ�

/* typedef -----------------------------------------------------------*/


/* function prototypes -----------------------------------------------*/
extern unsigned int ring_write(ring_buf_t *rbsend, void *buf, unsigned int len);

extern unsigned int ring_read(ring_buf_t *rbrev, void *buf, unsigned int len);

extern void ring_rxclear(ring_buf_t *rbrev);

extern void ring_txclear(ring_buf_t *rbsend);


/**
 * @fn ring_init
 * @brief ��ʼ�����λ�����
 * 
 * ��ʼ�����ͺͽ��յĻ��λ�������
 */
extern void ring_init(ring_buf_t *rbsend,unsigned char *bufsend,ring_buf_t *rbrev,unsigned char *bufrev);



#ifdef __cplusplus
}
#endif

#endif // ! RIL_RING_H
/************************ (C) COPYRIGHT ARSTUDIO *****END OF FILE***************************/
