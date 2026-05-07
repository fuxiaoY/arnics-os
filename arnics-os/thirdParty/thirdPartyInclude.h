#ifndef _THIRDPARTYINCLUDE_H_
#define _THIRDPARTYINCLUDE_H_  
#ifdef __cplusplus
extern "C" {
#endif

#include "unity/api_unity_def.h"
#include "Assert/usr_assert.h"
#include "ring/ring.h"
#include "struct2json/inc/s2j.h"
#include "modContact/mctInclude.h"
#include "Runonce/Runonce.h"
#include "rollDB/rolldbPort.h"
#include "softMutex/soft_mutex.h"
#include "uflog/uflog.h"
// #include "sfud/inc/sfud.h"
// #include "fal/inc/fal.h"
// #include "upgrade/upgradeManager.h"

extern void thirdPartyInit(void);
#ifdef __cplusplus
}
#endif
#endif
