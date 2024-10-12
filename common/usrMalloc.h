#ifndef  _USRMALLOC_H_
#define  _USRMALLOC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../Inc/projDefine.h"
#include "../Inc/typedef.h"

#ifdef _USE_FREERTOS_
#include "FreeRTOS.h"
#define arnicsFree vPortFree
#define arnicsMalloc pvPortMalloc
#else
#define arnicsFree free
#define arnicsMalloc malloc
#endif



#ifdef __cplusplus
}
#endif

#endif
