#ifndef _DATA_OBJ_H_
#define _DATA_OBJ_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "../dataCore/dataCore.h"


/* include -------------------------------------------------------------*/
#define DATA_PUBLIC_STRUCT
#include "dataPlat/dataObj/dataClass.h"
#undef DATA_PUBLIC_STRUCT
/* data link register --------------------------------------------------*/
#undef X
#define X(auth,index,type,subtype,var,len,key) index,
#define DATA_CLASS
typedef enum 
{
  #include "dataPlat/dataObj/dataClass.h"
  MAX_COUNT,
}dataPlatParaIndex_e;
#undef X
#undef DATA_CLASS
/* function prototypes -----------------------------------------------*/
extern const dataPlatInstance_t dataplat_instance;

#ifdef __cplusplus
}
#endif
#endif
