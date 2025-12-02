#ifndef _RTOSINCLUDE_H
#define _RTOSINCLUDE_H
#include "Inc/projDefine.h"
#include "Inc/typedef.h"

#include "rtosInterface/rtosTask.h"
#ifdef _USE_FREERTOS_
#include "rtosInterface/freertos/FreeRTOSConfig.h"
#include "rtosInterface/freertos/freertosDef.h"
#endif

#include "rtosInterface/rtosPort.h"
#endif
