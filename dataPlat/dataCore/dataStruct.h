#ifndef _DATA_ALL_STRUCT_H_
#define _DATA_ALL_STRUCT_H_ 
#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/typedef.h"
#include "Inc/ProjDefine.h"
#define RW_MASK(x) ((uint32_t)0x00000001 << (x))
#define NA ((uint32_t)0x00000000)
#define R RW_MASK(0)
#define W RW_MASK(1)
#define S RW_MASK(2)

typedef enum
{
    TYPE_NULL = 0,
    TYPE_INT8,
    TYPE_UINT8,
    TYPE_INT16,
    TYPE_UINT16,
    TYPE_INT32,
    TYPE_UINT32,
    TYPE_INT64,        
    TYPE_UINT64,    
    TYPE_FLOAT,
    TYPE_DOUBLE,

    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_INT,
    TYPE_STRING,
    TYPE_ARRAY,
    TYPE_SIZE_T,       
    TYPE_PTR,     
    TYPE_STRUCT,
    TYPE_ENUM,
    TYPE_UNION,
} dataType_e;

typedef struct 
{
    uint32_t    auth;         
    uint32_t    index; 
    dataType_e  type;
    dataType_e  sub_type;
    void*       pVar;
    uint32_t    len;
    const char *key;
} dataParaList_t;

struct dataPlatInstance_t
{
    dataParaList_t* para_table;
    uint32_t max_count;
};


#ifdef __cplusplus
}
#endif
#endif
