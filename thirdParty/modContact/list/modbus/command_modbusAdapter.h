
#ifndef _COMMAND_MODBUSADAPTER_H
#define _COMMAND_MODBUSADAPTER_H

#ifdef __cplusplus
extern "C" {
#endif
#include "command_modbus.h"
#include "../../common/mctProcesser.h"
#include "../../dataplat/mctDefinition.h"
#include "../mctList.h"



extern tCmdApi const *mctModbusApiGet(void);
#ifdef __cplusplus
}
#endif
#endif
