#include "dataLink.h"
#ifdef USE_MCT_DATA_LINK
#include "../../../dataPlat/dataPlatInclude.h"
#undef X 
#define X(name, var, len) {var, len, name},


// 参数表结构体
typedef struct 
{
    void* pVar;
    uint32_t len;
    mctParaIndex index; // 添加索引字段
} mctParaList;


// 用户事件
static mctParaList paraTable[] = 
{
    #include "dataLinkDef.h" 
}; 

void *mctParaGetPtr(mctParaIndex id)
{
    if (id >= MCT_DATA_MAX_COUNT ) 
    {
        return NULL;
    }

    return paraTable[id].pVar;
}

uint32_t mctParaGet(mctParaIndex id, void* outBuf, uint32_t bufLen)
{
    if (id >= MCT_DATA_MAX_COUNT || outBuf == NULL || bufLen == 0) 
    {
        return 0;
    }

    const mctParaList* pPara = &paraTable[id];
    uint32_t copyLen = (bufLen < pPara->len) ? bufLen : pPara->len;
    memcpy(outBuf, pPara->pVar, copyLen);
    return copyLen;
}

uint32_t mctParaSet(mctParaIndex id, const void* inData, uint32_t dataLen)
{
    if (id >= MCT_DATA_MAX_COUNT || inData == NULL || dataLen == 0) 
    {
        return 0;
    }

    mctParaList* pPara = &paraTable[id];
    uint32_t copyLen = (dataLen < pPara->len) ? dataLen : pPara->len;
    memcpy(pPara->pVar, inData, copyLen);
    return copyLen;
}
#endif
