#ifndef __MCTDRIVER_H__
#define __MCTDRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "../dataPlat/mctDefinition.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/* define ------------------------------------------------------------*/

#define WAIT_SCHEDULE_TIME_MS 10


#define MCT_PAYLOAD_SIZE  512
/* typedef -----------------------------------------------------------*/

/* function prototypes -----------------------------------------------*/



/**
 * @fn MCT_DELAY
 * @brief 精准延时
 * 
 * 此函数用于精准延时。
 */
extern void MCT_DELAY(uint32_t time);
/**
 * @fn mct_init
 * @brief 初始化mct模块
 * 
 * 此函数用于初始化mct模块，以便进行后续的读写操作。
 */
extern void mct_init(MctInstance* pInstance,uint8_t *Cmd_cache,size_t cmd_max_size,uint8_t* Payload_cahe,size_t payload_max_size);
/**
 * @fn mct_init
 * @brief 将mct数据重置
 * 
 */
extern void mct_data_reset(MctInstance* pInstance);
/**
 * @fn mct_write
 * @brief 向mct模块写入数据
 * 
 * 此函数用于向mct模块写入数据。
 *
 * @param [in] file - 指向要写入的数据的指针
 * @param [in] len - 要写入的数据的长度
 * @retval 写入操作的结果。通常为0表示成功，非0值表示错误代码。
 */
extern int mct_write(void *file, uint16_t len);            

/**
 * @fn mct_read
 * @brief 从mct模块读取数据
 * 
 * 此函数用于从mct模块读取数据到缓冲区中。
 *
 * @param [in] buf - 存储读取数据的缓冲区
 * @param [in] maxlen - 缓冲区的最大长度
 * @retval 实际读取的字节数。如果返回值小于或等于0，则表示读取操作失败或没有数据可读。
 */
extern uint32_t mct_read(uint8_t *buf, uint16_t maxlen);



#ifdef __cplusplus
}
#endif
#endif
