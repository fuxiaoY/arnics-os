#ifndef _THIRDPARTYINCLUDE_H_
#define _THIRDPARTYINCLUDE_H_  
#ifdef __cplusplus
extern "C" {
#endif
#include "ulog/ulogDef.h"
#include "unity/api_unity_def.h"
#include "Assert/usr_assert.h"
#include "ring/ring.h"
#include "struct2json/inc/s2j.h"
#include "sfud/inc/sfud.h"
#include "fal/inc/fal.h"
#include "modContact/mctInclude.h"
#include "Runonce/Runonce.h"
#include "upgrade/upgradeManager.h"
#include "rollDB/rolldbPort.h"

extern void thirdPartyInit(void);
#ifdef __cplusplus
}
#endif
#endif
