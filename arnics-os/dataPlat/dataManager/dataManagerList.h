#ifndef _DATA_MANAGER_LIST_H_
#define _DATA_MANAGER_LIST_H_
#include "Inc/projDefine.h"
#include "Inc/typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "dataPlat/dataCore/dataCore.h"
#include "dataPlat/dataCore/dataStruct.h"
#include "dataPlat/dataObj/dataObj.h"
#define DATA_ALL_STRUCT
#include "dataPlat/dataObj/dataClass.h"
#undef DATA_ALL_STRUCT

#define DATA_LIST
#include "dataPlat/dataObj/dataClass.h"
#undef DATA_LIST


#ifdef __cplusplus
}
#endif

#endif
