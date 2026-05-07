
#ifndef __MCTLIST_H__
#define __MCTLIST_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "thirdParty/modContact/core/mctCore.h"
#include "thirdParty/modContact/common/mctStruct.h"
#include "thirdParty/modContact/list/console/command_consoleAdapter.h"



extern uint16_t mctModemLisNumGet(void);
extern const tModemList *mctModemListGet(void);

#ifdef __cplusplus
}
#endif

#endif
