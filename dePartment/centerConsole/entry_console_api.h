#ifndef ENTRY_CONSOLE_API_H_
#define ENTRY_CONSOLE_API_H_
#include "Inc/typedef.h"
#include "Inc/ProjDefine.h"



// 函数指针类型的定义
typedef void (T_TYPEDEF(console_process))(void);


// 输出函数接口结构体
typedef struct
{

    T_STRUCT_MEMBER(console_process);
/*-----------------------------------*/
} tconsoleEntry;

// 作为一个指向结构体的指针
extern const tconsoleEntry entry_console_list;
//  指向 函数指针
#define console_api (&entry_console_list)
#define console_MICRODEF(name) (console_api->t_##name)
/*-----------------------------------*/


#define console_process          console_MICRODEF(console_process)
#endif // console_API_H_
