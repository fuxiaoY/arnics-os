
#ifndef ENTRY_AD_API_H_
#define ENTRY_AD_API_H_

#include "../../Inc/ProjDefine.h"
#include "../../Inc/typedef.h"
#include "../../dataPlat/dataPlatInclude.h"

// 函数指针类型的定义
typedef void (T_TYPEDEF(ad_process))(void);
typedef bool (T_TYPEDEF(readAdReq))(adMessage_t *msg,time_t wait);
typedef void (T_TYPEDEF(SendMsgToAdCenter))(adMessage_t *message);

// 输出函数接口结构体
typedef struct
{
    T_STRUCT_MEMBER(ad_process);
    T_STRUCT_MEMBER(readAdReq);
    T_STRUCT_MEMBER(SendMsgToAdCenter);
/*-----------------------------------*/
} tSLEEPEntry;

// 作为一个指向结构体的指针
extern const tSLEEPEntry entry_sleep_list;
//  指向 函数指针
#define sleep_api (&entry_sleep_list)
#define SLEEP_MICRODEF(name) (sleep_api->t_##name)
/*-----------------------------------*/

#define ad_process                             SLEEP_MICRODEF(ad_process)
#define readAdReq                              SLEEP_MICRODEF(readAdReq)
#define SendMsgToAdCenter                      SLEEP_MICRODEF(SendMsgToAdCenter)
#endif // API_H_
