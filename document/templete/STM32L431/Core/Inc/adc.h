// .h
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "../../arnics-os/drivers/bsp/bsp_def.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */



/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_ADC1_Init(void);
extern void bsp_adc_init(adc_t *adcx);
extern void bsp_adc_close(adc_t *adcx);
extern int bsp_adc_read(adc_t *adcx,uint16_t *buf, size_t count);
/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */
