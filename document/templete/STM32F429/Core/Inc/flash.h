#ifndef __FLASH_H__
#define __FLASH_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "../../arnics-os/drivers/bsp/bsp_def.h"



extern int bsp_flash_read(flash_t *dev, uint8_t *buf, size_t size);

extern int bsp_flash_write(flash_t *dev, const uint8_t *buf, size_t size);

extern int flash_erase(flash_t *dev,flash_ctl_t *ctl);

#ifdef __cplusplus
}
#endif
#endif /*__ FLASH_H__ */
