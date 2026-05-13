#ifndef _THIRDPARTYINCLUDE_H_
#define _THIRDPARTYINCLUDE_H_  
#ifdef __cplusplus
extern "C" {
#endif

#include "thirdParty/unity/api_unity_def.h"
#include "thirdParty/Assert/usr_assert.h"
#include "thirdParty/ring/ring.h"
#include "thirdParty/struct2json/inc/s2j.h"
#include "thirdParty/modContact/mctInclude.h"
#include "thirdParty/Runonce/Runonce.h"
#include "thirdParty/rollDB/rolldbPort.h"
#include "thirdParty/softMutex/soft_mutex.h"
#include "thirdParty/uflog/uflog.h"
// #include "thirdParty/sfud/inc/sfud.h"
// #include "thirdParty/fal/inc/fal.h"
// #include "thirdParty/upgrade/upgradeManager.h"

extern void thirdPartyInit(void);
#ifdef __cplusplus
}
#endif
#endif
