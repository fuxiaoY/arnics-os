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
#ifndef _DATA_JSON_H_
#define _DATA_JSON_H_
#include "Inc/projDefine.h"
#include "Inc/typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* typedef -----------------------------------------------------------*/
typedef enum
{
    FORMAT = 0,
    UNFORMAT,
} jsonFomat_e;

typedef bool (*unityCb)(char *str);
extern bool print_str_callback(char *str);
extern void UnitySystemInterface(const char *ArgReq,unityCb cb);
extern void UnityGlobalStatInterface(const char *ArgReq,unityCb cb);
extern void UnityGlobalCfgInterface(const char *ArgReq,unityCb cb);



#ifdef __cplusplus
}
#endif

#endif
