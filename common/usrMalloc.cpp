#include "usrMalloc.h"

#ifdef _USE_FREERTOS_
#include "FreeRTOS.h"
#define arnicsFree vPortFree
#define arnicsMalloc pvPortMalloc
#else
#define arnicsFree free
#define arnicsMalloc malloc
#endif

