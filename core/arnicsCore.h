/* information */
/**
  ******************************************************************************
  * @file           : ARNICS_CORE_H
  * @brief          : 核心功能头文件
  * 
  * 该文件定义了核心功能的宏定义、数据类型和函数声明。
  * 主要用于初始化和注册各种功能模块。
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
#ifndef _ARNICS_CORE_H_
#define _ARNICS_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../Inc/projDefine.h"
#include "../Inc/typedef.h"

/* typedef -----------------------------------------------------------*/
#define TASK_FUNC(func)   {func}
/* \brief Function pointer define */
typedef void (*funcPointer) (void);

typedef struct
{
    funcPointer func;

} tTaskFunc;

typedef struct 
{
    const char *name;               
    void (*func)(void* argv);             
    int level;  
} ArnicsFuncItem;

/* define ------------------------------------------------------------*/
#define  STRCAT(a, b ,c)                 #a "." b "."  #c

#define  ARNICS_CONN(type, funcname, line)  type  funcname##_##line
#define  ARNICS_DEF(type,funcname,line)     ARNICS_CONN(type, funcname, line)
#define  ARNICS_TYPE(type,funcname)         ARNICS_DEF(type, funcname, __LINE__)
/**
 * @def ARNICS_REGISTER
 * @brief 注册功能项宏
 * 
 * @param [in] name - 功能项名称
 * @param [in] func - 功能项函数指针
 * @param [in] department - 部门名称
 * @param [in] level - 段级别
 */
#define ARNICS_REGISTER(name, func, department, level) \
    _USED ARNICS_TYPE(const ArnicsFuncItem, funCb_##func) \
    _SECTION(STRCAT(arnics, department, level)) = {name, func, level}
#define INIT_TAG "init"
#define DRIVER_INIT(name,func)      ARNICS_REGISTER(name,func,INIT_TAG,1)
#define SYSTEM_INIT(name,func)      ARNICS_REGISTER(name,func,INIT_TAG,2)
#define COMPONENT_INIT(name,func)   ARNICS_REGISTER(name,func,INIT_TAG,3)
#define DEPARTMENT_INIT(name,func)  ARNICS_REGISTER(name,func,INIT_TAG,4)


/* macro -------------------------------------------------------------*/

static inline void nop_process(void* argv) {}
#define DEFINE_ARNICS_FUNC_ITEM(name, department, level) \
    const ArnicsFuncItem name _SECTION(STRCAT(arnics, department, level)) = \
    { \
        "", nop_process \
    }
/**
 * @def DEFINE_ARNICS_FUNC_ITEM_RANGE
 * @brief 定义功能项段范围宏
 * 
 * @param [in] prefix - 前缀
 * @param [in] department - 部门名称
 * @param [in] start_level - 开始级别
 * @param [in] end_level - 结束级别
 */
#define DEFINE_ARNICS_FUNC_ITEM_RANGE(prefix, department, start_level, end_level) \
    DEFINE_ARNICS_FUNC_ITEM(prefix##_start, department, start_level); \
    DEFINE_ARNICS_FUNC_ITEM(prefix##_end, department, end_level)

/**
 * @def EXECUTE_FUNC_ALLSECTION
 * @brief 执行所有功能项宏
 * 
 * @param [in] prefix - 前缀
 */
#define EXECUTE_FUNC_ALLSECTION(prefix,argv) \
    do { \
        const ArnicsFuncItem* it = &prefix##_start; \
        const ArnicsFuncItem* end = &prefix##_end; \
        while (it < end) { \
            if (it->func != NULL) { \
                it->func(argv); \
            } \
            it++; \
        } \
    } while (0)
/**
 * @def EXECUTE_FUNC_BY_NAME
 * @brief 根据名称执行功能项宏
 * 
 * @param [in] prefix - 前缀
 * @param [in] Name - 功能项名称
 */
#define EXECUTE_FUNC_BY_NAME(prefix, Name, argv) \
    do { \
        const ArnicsFuncItem* it = &prefix##_start; \
        const ArnicsFuncItem* end = &prefix##_end; \
        while (it < end) { \
            if (it->name != NULL && strcmp(it->name, Name) == 0) { \
                if (it->func != NULL) { \
                    it->func(argv); \
                } \
                break; \
            } \
            it++; \
        } \
    } while (0)
/**
 * @def EXECUTE_FUNC_BY_NAME_AT_LEVEL
 * @brief 根据名称和级别执行功能项宏
 * 
 * @param [in] prefix - 前缀
 * @param [in] Name - 功能项名称
 * @param [in] Level - 段级别
 */
#define EXECUTE_FUNC_BY_NAME_AT_LEVEL(prefix, Name, Level,argv) \
    do { \
        const ArnicsFuncItem* it = &prefix##_start; \
        const ArnicsFuncItem* end = &prefix##_end; \
        while (it < end) { \
            if (it->level == Level && it->name != NULL && strcmp(it->name, Name) == 0) { \
                if (it->func != NULL) { \
                    it->func(argv); \
                } \
                break; \
            } \
            it++; \
        } \
    } while (0)


/* function declaration ---------------------------------------------*/
/**
 * @fn void logo_print(void)
 * @brief logo函数
 * 
 * logo
 */
extern void logo_print(void);

/**
 * @fn void arnics_core_init(void)
 * @brief 核心初始化函数
 * 
 * 该函数用于初始化核心功能模块。
 */
extern void arnics_core_init(void);



#ifdef __cplusplus
}
#endif
#endif
/************************ (C) COPYRIGHT ARSTUDIO *****END OF FILE***************************/
