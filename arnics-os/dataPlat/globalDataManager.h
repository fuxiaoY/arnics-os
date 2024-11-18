/* information */
/**
  ******************************************************************************
  * @file           : GlobalDataManager.h
  * @brief          : 全局数据管理相关的头文件
  * 
  * 该文件定义了全局数据管理的相关宏定义、枚举、结构体和函数声明。
  * 主要用于系统的参数管理和配置。
  * 
  * @version        : 1.0.0
  * @date           : 2023-10-01
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 ARSTUDIO.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* information */
#ifndef _GLOBALDATAMANAGER_H_
#define _GLOBALDATAMANAGER_H_
#include "../Inc/projDefine.h"
#include "../Inc/typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*---------------------------------------------------------------------*/
/*-系统参数------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
#define Unity_System_START 0
#define Unity_System_usr_systick (1 + Unity_System_START) // 用户系统滴答计数

/*---------------------------------------------------------------------*/
/*-global_state--------------------------------------------------------*/
/*---------------------------------------------------------------------*/
#define Unity_global_state_START 0
#define Unity_global_state_SaveTs (1 + Unity_global_state_START) // 保存时间戳
#define Unity_global_cfg_WorkStat (1 + Unity_global_state_SaveTs)
#define Unity_global_cfg_PreWorkStat (1 + Unity_global_cfg_WorkStat)
#define Unity_global_state_MAX_ID (1 + Unity_global_cfg_PreWorkStat) // 最大ID
/*---------------------------------------------------------------------*/
/*-global_cfg----------------------------------------------------------*/
/*---------------------------------------------------------------------*/
#define Unity_global_cfg_START 0
#define Unity_global_cfg_SaveTs (1 + Unity_global_cfg_START) // 保存时间戳

#define Unity_global_cfg_MAX_ID (1 + Unity_global_cfg_SaveTs) // 最大ID

/* define ------------------------------------------------------------*/
#define RW_MASK(x) ((uint32_t)0x00000001 << (x))
#define NA ((uint32_t)0x00000000)
#define R RW_MASK(0)
#define W RW_MASK(1)
#define S RW_MASK(2)
/* typedef -----------------------------------------------------------*/
typedef enum
{
    FORMAT = 0,
    UNFORMAT,
} JsonFomat;
typedef enum
{
    NULL_TYPE = 0,
    UINT8_TYPE,
    INT_TYPE,
    INT16_TYPE,
    UINT16_TYPE,
    UINT32_TYPE,
    FLOAT_TYPE,
    STRING_TYPE,
    ARRAY_TYPE
} FieldType;

typedef struct
{
    uint32_t access;
    uint16_t id;
    FieldType type;
    FieldType subType;
    void *addr;
    uint16_t len;
    const char *key;
} unityParaList_t;
extern void UnitySystemInterface(const char *ArgReq);
extern void UnityGlobalStatInterface(const char *ArgReq);
extern void UnityGlobalCfgInterface(const char *ArgReq);



#ifdef __cplusplus
}
#endif

#endif
