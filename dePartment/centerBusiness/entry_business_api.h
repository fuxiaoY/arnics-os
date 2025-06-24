#ifndef ENTRY_BUSINESS_API_H_
#define ENTRY_BUSINESS_API_H_
#include "../../Inc/typedef.h"
#include "../../Inc/ProjDefine.h"


// 函数指针类型的定义
typedef void (T_TYPEDEF(business_process))(void);


// 输出函数接口结构体
typedef struct
{

    T_STRUCT_MEMBER(business_process);
/*-----------------------------------*/
} tBUSINESSEntry;

// 作为一个指向结构体的指针
extern const tBUSINESSEntry entry_business_list;
//  指向 函数指针
#define business_api (&entry_business_list)
#define BUSINESS_MICRODEF(name) (business_api->t_##name)
/*-----------------------------------*/


#define business_process          BUSINESS_MICRODEF(business_process)
#endif // BUSINESS_API_H_
