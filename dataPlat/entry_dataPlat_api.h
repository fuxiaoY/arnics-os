
#ifndef ENTRY_DATAPLAT_API_H_
#define ENTRY_DATAPLAT_API_H_
#include "../Inc/projDefine.h"
#include "../Inc/typedef.h"
#include "globalDef.h"
#ifdef __cplusplus
extern "C" {
#endif
// 函数指针类型的定义
typedef void (T_TYPEDEF(UnitySystemInterface))(const char*);

// 输出函数接口结构体
typedef struct
{
    T_STRUCT_VAR(arnics_systick,volatile uint32_t);
    T_STRUCT_VAR(arnics_start,bool);
    T_STRUCT_VAR(global_cfg,SytemCfg);
    T_STRUCT_VAR(global_state,SytemState);
    T_STRUCT_VAR(eventosWantSleep,bool);
    
    T_STRUCT_MEMBER(UnitySystemInterface);
/*-----------------------------------*/
} tDATAPLATEntry;

// 作为一个指向结构体的指针
extern const tDATAPLATEntry entry_dataplat_list;
//  指向 函数指针
#define DATAPLAT_API (&entry_dataplat_list)
#define DATAPLAT_MICRODEF(name) (DATAPLAT_API->t_##name)
#define DATAPLAT_MICRODEF_VAR(name, TYPE) (*(TYPE *)DATAPLAT_MICRODEF(name))
/*-----------------------------------*/

#define arnics_systick          DATAPLAT_MICRODEF_VAR(arnics_systick, volatile uint32_t)
#define arnics_start            DATAPLAT_MICRODEF_VAR(arnics_start, bool)
#define global_cfg            DATAPLAT_MICRODEF_VAR(global_cfg, SytemCfg)
#define global_state          DATAPLAT_MICRODEF_VAR(global_state, SytemState)
#define eventosWantSleep          DATAPLAT_MICRODEF_VAR(eventosWantSleep, bool)
#define UnitySystemInterface    MICRODEF(UnitySystemInterface)

#ifdef __cplusplus
}
#endif
#endif // ENTRY_DATAPLAT_API_H_
