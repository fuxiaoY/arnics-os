#ifndef __FREERTOS_H__
#define __FREERTOS_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "../../Inc/projDefine.h"
#include "../../Inc/typedef.h"

#include "../../dataPlat/globalDef.h"
#include "../rtosTask.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

extern void MX_FREERTOS_Init(void);

#ifdef __cplusplus
}
#endif

#endif 

