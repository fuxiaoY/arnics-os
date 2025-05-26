
#ifndef __MCTLIST_H__
#define __MCTLIST_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "../core/mctCore.h"
#include "../common/mctStruct.h"
#include "modbus/command_modbusAdapter.h"
#include "console/command_consoleAdapter.h"
#include "Y7025/command_Y7025Adapter.h"
#include "A7680C/command_A7680CAdapter.h"


extern uint16_t mctModemLisNumGet(void);
extern const tModemList *mctModemListGet(void);

#ifdef __cplusplus
}
#endif

#endif
