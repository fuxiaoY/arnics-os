#ifndef ENTRY_GUARD_API_H_
#define ENTRY_GUARD_API_H_
#include "../../Inc/typedef.h"
#include "../../Inc/ProjDefine.h"



// 函数指针类型的定义
typedef void (T_TYPEDEF(guard_process))(void);


// 输出函数接口结构体
typedef struct
{

    T_STRUCT_MEMBER(guard_process);
/*-----------------------------------*/
} tGUARDEntry;

// 作为一个指向结构体的指针
extern const tGUARDEntry entry_guard_list;
//  指向 函数指针
#define guard_api (&entry_guard_list)
#define GUARD_MICRODEF(name) (guard_api->t_##name)
/*-----------------------------------*/


#define guard_process          GUARD_MICRODEF(guard_process)
#endif // GUARD_API_H_
