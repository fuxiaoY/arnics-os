#ifndef __MCTADAPTER_H__
#define __MCTADAPTER_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "mctProcesser.h"

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
 * @fn mct_init
 * @brief 将mct数据重置
 * 
 */
extern void mct_data_reset(MctInstance* pInstance);
#ifdef __cplusplus
}
#endif
#endif
