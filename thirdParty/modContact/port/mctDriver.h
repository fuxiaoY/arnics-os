#ifndef __MCTDRIVER_H__
#define __MCTDRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "../../../Inc/typedef.h"

/* define ------------------------------------------------------------*/

#define WAIT_SCHEDULE_TIME_MS 10
#define TIMEOUT_MS 100
/* typedef -----------------------------------------------------------*/

/* function prototypes -----------------------------------------------*/



/**
 * @fn MCT_DELAY
 * @brief 精准延时
 * 
 * 此函数用于精准延时。
 */
extern void MCT_DELAY(uint32_t time);
extern void MCT_MALLOC(void **buf, uint32_t size);
extern void MCT_FREE(void **buf);
extern int command_mct_write(void *file, uint16_t len);            
extern uint32_t command_mct_read(uint8_t *buf, uint16_t maxlen);
extern int modem_mct_write(void *file, uint16_t len);            
extern uint32_t modem_mct_read(uint8_t *buf, uint16_t maxlen);


#ifdef __cplusplus
}
#endif
#endif
