/**
  ******************************************************************************
  * @file           : mctAdapter.h
  * @brief          : modContact adapter layer header file
  * 
  * 
  * 
  * @version        : 1.0.2
  * @date           : 2025-05-26
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 arong.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#ifndef __MCTADAPTER_H__
#define __MCTADAPTER_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "../core/mctCore.h"
#include "../list/mctList.h"
#include "../port/mctDriver.h"
/**
 * @fn mct_reg
 * @brief 接口注册
 * 
 * 
 */
extern bool mct_reg(const char* name,MctInstance *inst,bool ModemAtuoRecogniton);
/**
 * @fn mctApiExecute
 * @brief 执行接口
 * 
 * 
 */
extern bool mctApiExecute(MctInstance *inst,uint16_t id, void *para);
/**
 * @fn mct_init
 * @brief 初始化mct模块
 * 
 * 此函数用于初始化mct模块，以便进行后续的读写操作。
 */
extern void mct_init(MctInstance* pInstance,uint8_t *Cmd_cache,size_t cmd_max_size, \
                                     uint8_t* Payload_cahe,size_t payload_max_size, \
                                   int (*write_callback)(void *file, uint16_t len), \
                           uint32_t (*read_callback)(uint8_t *buf, uint16_t maxlen));

/**
 * @brief 设置粘帧处理状态 \ setting sticky frame status
 */
extern void mct_sticky_frame_action(MctInstance* pInstance,bool enable);


#ifdef __cplusplus
}
#endif
#endif
