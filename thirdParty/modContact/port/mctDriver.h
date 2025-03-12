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

extern int command_mct_write(void *file, uint16_t len);            
extern uint32_t command_mct_read(uint8_t *buf, uint16_t maxlen);



#ifdef __cplusplus
}
#endif
#endif
