#ifndef _LTDC_H
#define _LTDC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Inc/typedef.h"
#include "main.h"
#include "../../arnics-os/drivers/bsp/bsp_def.h"

extern int bsp_ltdc_init(ltdc_t *lcd_ltdc);
extern int bsp_ltdc_close(ltdc_t *lcd_ltdc);
#ifdef __cplusplus
}
#endif

#endif
