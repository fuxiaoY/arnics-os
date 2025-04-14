

#ifndef _CRC16_H_
#define _CRC16_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define DL_CRC16(buf, len)   crc16_update_revtab(0, buf, len)
uint16_t crc16_update_revtab(uint16_t crc, const void* cp, uint16_t size);
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif

