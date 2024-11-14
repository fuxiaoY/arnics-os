
#ifndef ENTRY_MEDIA_API_H_
#define ENTRY_MEDIA_API_H_
#include "../../Inc/ProjDefine.h"
#include "../../Inc/typedef.h"


// 函数指针类型的定义
typedef void (T_TYPEDEF(media_process))(void);
// 输出函数接口结构体
typedef struct
{

    T_STRUCT_MEMBER(media_process);

/*-----------------------------------*/
} tMEDIAEntry;

// 作为一个指向结构体的指针
extern const tMEDIAEntry entry_media_list;
//  指向 函数指针
#define media_api (&entry_media_list)
#define MEDIA_MICRODEF(name) (media_api->t_##name)
/*-----------------------------------*/

#define media_process                          MEDIA_MICRODEF(media_process)


#endif // API_H_
