#include "dataPlat/dataCore/dataStruct.h"
#include "dataObj.h"

#define DATA_ALL_STRUCT
#include "dataPlat/dataObj/dataClass.h"
#undef DATA_ALL_STRUCT
#undef X 
#define X(auth,index,type,subtype,var,len,key) {auth,index,type,subtype,var,len,key},
/* data link register --------------------------------------------------*/
/**
 * @brief  data register.
 *       
 */
#define DATA_CLASS
static dataParaList_t para_table[] = 
{
    #include "dataPlat/dataObj/dataClass.h"
}; 
#undef DATA_CLASS

const dataPlatInstance_t dataplat_instance = 
{
    .para_table = para_table,
    .max_count = MAX_COUNT
};



