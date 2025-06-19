#ifndef RTOSINTERFACE_H_
#define RTOSINTERFACE_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "../Inc/ProjDefine.h"
#include "../Inc/typedef.h"

#ifdef _USE_FREERTOS_
typedef enum  
{
    rtosPriorityIdle          = -3,          ///< priority: idle (lowest)
    rtosPriorityLow           = -2,          ///< priority: low
    rtosPriorityBelowNormal   = -1,          ///< priority: below normal
    rtosPriorityNormal        =  0,          ///< priority: normal (default)
    rtosPriorityAboveNormal   = +1,          ///< priority: above normal
    rtosPriorityHigh          = +2,          ///< priority: high
    rtosPriorityRealtime      = +3,          ///< priority: realtime (highest)
    rtosPriorityError         =  0x84        ///< system cannot determine priority or thread has illegal priority
} rtosPriority_e;

typedef void (*rtos_pthread) (void const *argument);

#endif


#ifdef __cplusplus
}
#endif
#endif
