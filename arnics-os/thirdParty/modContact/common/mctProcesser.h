#ifndef __MCTPROCESSER_H__
#define __MCTPROCESSER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "../dataPlat/mctDefinition.h"
#include "../port/mctDriver.h"

extern void initStaticFrameList(StaticFrameList *list);

extern void addFrameToExpectedFrameList(StaticFrameList *list, uint16_t startOffset, uint16_t endOffset,uint16_t id);

extern void addFrameToFrameList(StaticFrameList *list, uint16_t startOffset, uint16_t endOffset,uint16_t id);

extern bool CMD_Execute(MctInstance *inst, \
                bool is_expected,int32_t expected_id, \
                tCmd const *List,uint16_t cmdNum,void *para);

#ifdef __cplusplus
}
#endif
#endif
