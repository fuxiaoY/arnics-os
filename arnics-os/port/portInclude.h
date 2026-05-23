#ifndef _PORTINCLUDE_H
#define _PORTINCLUDE_H


#ifdef __cplusplus
extern "C" {
#endif

#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#include "port/arnicsPort.h"
#include "port/arnicsMalloc.h"
#if PLATFORM_MCU
#include "port/portMcu/arnicsMcu.h"
#endif


#ifdef __cplusplus
}
#endif
#endif
