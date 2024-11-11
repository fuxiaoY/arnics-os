#ifndef _COMMAND_CONSOLEADAPTER_H
#define _COMMAND_CONSOLEADAPTER_H

#ifdef __cplusplus
extern "C" {
#endif
#include "command_console.h"
#include "../../common/mctProcesser.h"
#include "../../dataplat/mctDefinition.h"
#include "../mctList.h"



extern tCmdApi const *mctConsoleApiGet(void);
#ifdef __cplusplus
}
#endif
#endif
