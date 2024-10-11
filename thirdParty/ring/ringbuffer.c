/******************************************************************************
 * @brief    环形缓冲区管理(参考linux/kfifo)
 *
 * Copyright (c) 2016~2021, <morro_luo@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2016-05-30     Morro        Initial version
 * 2021-02-05     Morro        增加空闲空间获取接口.
 ******************************************************************************/
#include "ringbuffer.h"
#include <string.h>
#include <stddef.h>

#define min(a,b) ( (a) < (b) )? (a):(b)     
     
/*
 *@brief      构造一个空环形缓冲区
 *@param[in]  r    - 环形缓冲区管理器
 *@param[in]  buf  - 数据缓冲区
 *@param[in]  len  - buf长度(必须是2的N次幂)
 *@retval     bool
 */
bool ring_buf_init(ring_buf_t *r,unsigned char *buf, unsigned int len)
{
    r->buf    = buf;
    r->size   = len;
    r->front  = r->rear = 0;
    return buf != NULL && (len & len -1) == 0;
}

/*
 *@brief      清空环形缓冲区 
 *@param[in]  r - 待清空的环形缓冲区
 *@retval     none
 */
void ring_buf_clr(ring_buf_t *r)
{
    r->front = r->rear = 0;
}

/*
 *@brief      获取环形缓冲区数据长度
 *@retval     环形缓冲区中有效字节数 
 */
unsigned int ring_buf_len(ring_buf_t *r)
{
    return r->rear - r->front;
}

/*
 *@brief      获取环形缓冲空闲空间
 *@retval     空闲空间
 */
unsigned int ring_buf_free_space(ring_buf_t *r)
{
    return r->size - (unsigned int)(r->rear - r->front);
}

/*
 *@brief       将指定长度的数据放到环形缓冲区中 
 *@param[in]   buf - 数据缓冲区
 *             len - 缓冲区长度 
 *@retval      实际放到中的数据 
 */
unsigned int ring_buf_put(ring_buf_t *r,unsigned char *buf,unsigned int len)
{
    unsigned int i;
    unsigned int left;
    left = r->size + r->front - r->rear;
    len  = min(len , left);
    i    = min(len, r->size - (r->rear & r->size - 1));   
    memcpy(r->buf + (r->rear & r->size - 1), buf, i); 
    memcpy(r->buf, buf + i, len - i);
    r->rear += len;     
    return len;
    
}

/*
 *@brief       从环形缓冲区中读取指定长度的数据 
 *@param[in]   len - 读取长度 
 *@param[out]  buf - 输出数据缓冲区
 *@retval      实际读取长度 
 */
unsigned int ring_buf_get(ring_buf_t *r,unsigned char *buf,unsigned int len)
{
    unsigned int i;
    unsigned int left;    
    left = r->rear - r->front;
    len  = min(len , left);                                
    i    = min(len, r->size - (r->front & r->size - 1));
    memcpy(buf, r->buf + (r->front & r->size - 1), i);    
    memcpy(buf + i, r->buf, len - i);   
    r->front += len;
    return len;
}
