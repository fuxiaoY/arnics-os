#ifndef __MCTADAPTER_H__
#define __MCTADAPTER_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "mctProcesser.h"


extern bool mct_reg(const char* name,MctInstance *inst,bool ModemAtuoRecogniton);
extern bool mctApiExecute(MctInstance *inst,uint16_t id, void *para);

#ifdef __cplusplus
}
#endif
#endif
