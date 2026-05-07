#ifndef _DATA_API_H_
#define _DATA_API_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "dataPlat/dataCore/dataCore.h"
#include "dataPlat/dataObj/dataObj.h"
/* include struct ------------------------------------------------------*/



#define DATA_PTR(ID)                      dataParaGetPtrEx(&dataplat_instance, DATA(ID))
#define DATA_GET(ID,OUT_BUF,BUF_LEN)      dataParaGetEx(&dataplat_instance, DATA(ID), OUT_BUF, BUF_LEN)
#define DATA_SET(ID,IN_DATA,DATA_LEN)     dataParaSetEx(&dataplat_instance, DATA(ID), IN_DATA, DATA_LEN)

#ifdef __cplusplus
}
#endif
#endif
