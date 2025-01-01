#ifndef __MCTPROCESSER_H__
#define __MCTPROCESSER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "../dataPlat/mctDefinition.h"
#include "../dataplat/mctStruct.h"
#include "../port/mctDriver.h"

extern void initStaticFrameList(StaticFrameList *list);

extern bool CMD_Execute(MctInstance *inst, \
                int32_t expected_id, \
                tCmd const *List,uint16_t cmdNum,void *para);

#ifdef __cplusplus
}
#endif
#endif
