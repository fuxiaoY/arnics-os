#include "dataStruct.h"
#include "dataCore.h"
#if defined(__CC_ARM) || defined(__GNUC__) 
#pragma arm section code = "._entry_dataplat_api"
#endif


void* dataParaGetPtrEx(const dataPlatInstance_t* instance, uint32_t id) 
{
    USER_ASSERT(id < instance->max_count);
    return instance->para_table[id].pVar;
}

bool dataParaGetEx(const dataPlatInstance_t* instance, uint32_t id, void* out_buf, uint32_t buf_len) 
{
    USER_ASSERT((out_buf != NULL && buf_len != 0 && id < instance->max_count));
    const dataParaList_t* pPara = &instance->para_table[id];
    uint32_t copy_len = (buf_len <= pPara->len) ? buf_len : pPara->len;
    memcpy(out_buf, pPara->pVar, copy_len);
    if(buf_len <= pPara->len)
    {

        return true;
    }
    else
    {
        return false;
    }


}

bool dataParaSetEx(const dataPlatInstance_t* instance, uint32_t id, const void* data, uint32_t len) 
{
    USER_ASSERT(data != NULL && len != 0 && id < instance->max_count);
    const dataParaList_t* pPara = &((dataParaList_t*)instance->para_table)[id]; 
    uint32_t copy_len = (len <= pPara->len) ? len : pPara->len;
    memcpy(pPara->pVar, data, copy_len);
    if(len <= pPara->len)
    {

        return true;
    }
    else
    {
        return false;
    }
}


bool dataAuthCheck()
{
    return true;
}
