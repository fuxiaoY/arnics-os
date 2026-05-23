/**
 * @file arnicsCore.h
 * @brief Arnics 表驱动执行内核
 * @details
 * 提供“函数表 + 检索执行”的通用框架：表项包含 department/name/level 元信息与回调函数。
 * arnicsKernel_t 仅保存表指针与长度，不分配内存、不拷贝表内容。
 */
#ifndef _ARNICS_CORE_H_
#define _ARNICS_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Inc/projDefine.h"
#include "Inc/typedef.h"
/*----------------------------------------------------------------------
 * MACROS AND DEFINITIONS
 *--------------------------------------------------------------------*/

 #define ARNICS_CORE_VERSION_NUMBER                           "V2.0.1"
 #define ARNICS_CORE_VERSION_MAJOR                                   2
 #define ARNICS_CORE_VERSION_MINOR                                   0
 #define ARNICS_CORE_VERSION_BUILD                                   1

/* typedef -----------------------------------------------------------*/
#define TASK_FUNC(func)                                          {func}
/* \brief Function pointer define */
typedef                                      void (*funcPointer) (void);

typedef struct
{
    funcPointer        func;

} tTaskFunc;

/**
 * @brief 函数表表项
 */
typedef struct
{
    const char* department;
    const char* name;
    void (*func)(void* argv);
    int level;
} arnicsFuncItem_t;

/**
 * @brief 内核实例
 * @details
 * 仅绑定函数表指针与表长度；不分配内存，不管理表生命周期。
 */
typedef struct
{
    const arnicsFuncItem_t* table;
    int table_size;
} arnicsKernel_t;
/* define ------------------------------------------------------------*/

/**
 * @brief X-macro：声明表项函数原型
 */
#define ARNICS_KERNEL_DECLARE_ITEM(name, func, department, level) extern void func(void* argv);

/**
 * @brief X-macro：生成表项
 */
#define ARNICS_KERNEL_TABLE_ITEM(name, func, department, level) { department, name, func, level },

/**
 * @brief 声明 REGISTRY 中所有表项函数原型
 */
#define ARNICS_KERNEL_DECLARE_FUNCS(REGISTRY) REGISTRY(ARNICS_KERNEL_DECLARE_ITEM)

/**
 * @brief 使用 REGISTRY 生成静态表与表长度常量
 */
#define ARNICS_KERNEL_BUILD_TABLE(table_name, REGISTRY)                                         \
    static const arnicsFuncItem_t table_name##_table[] = { REGISTRY(ARNICS_KERNEL_TABLE_ITEM) }; \
    enum { table_name##_size = (int)(sizeof(table_name##_table) / sizeof((table_name##_table)[0])) };

/**
 * @brief 一次性完成：声明函数原型、建表、生成内核句柄
 */
#define ARNICS_KERNEL_DECLARE_AND_BUILD_TABLE(table_name, REGISTRY)                              \
    ARNICS_KERNEL_DECLARE_FUNCS(REGISTRY)                                                        \
    ARNICS_KERNEL_BUILD_TABLE(table_name, REGISTRY)                                              \
    static const arnicsKernel_t table_name##_kernel = {                                          \
        table_name##_table,                                                                      \
        table_name##_size                                                                        \
    };                                                                                           \
    const arnicsKernel_t* table_name(void)                                                       \
    {                                                                                            \
        return &table_name##_kernel;                                                             \
    }

/* function declaration ---------------------------------------------*/
/**
 * @brief 初始化/绑定内核实例
 * @param[out] kernel     内核实例
 * @param[in]  table      函数表
 * @param[in]  table_size 表项数量
 */
extern void arnics_kernel_init(arnicsKernel_t* kernel, const arnicsFuncItem_t* table, int table_size);

/**
 * @brief 按“部门”执行表中所有匹配项
 * @param[in] kernel 内核实例
 * @param[in] dept   部门标识
 * @param[in] argv   透传参数
 */
extern void arnics_kernel_execute_by_dept(const arnicsKernel_t* kernel, const char* dept, void* argv);

/**
 * @brief 按“部门 + 名称”执行单个匹配项（找到后执行一次并停止）
 * @param[in] kernel 内核实例
 * @param[in] dept   部门标识
 * @param[in] name   条目名称
 * @param[in] argv   透传参数
 */
extern void arnics_kernel_execute_by_name(const arnicsKernel_t* kernel, const char* dept, const char* name, void* argv);

/**
 * @brief 按“部门 + level”执行单个匹配项（找到后执行一次并停止）
 * @param[in] kernel 内核实例
 * @param[in] dept   部门标识
 * @param[in] level  等级
 * @param[in] argv   透传参数
 */
extern void arnics_kernel_execute_by_level(const arnicsKernel_t* kernel, const char* dept, int level, void* argv);



/**
 * @brief 便捷宏：按部门执行（kernel 为句柄函数名）
 */
#define ARNICS_EXECUTE_BY_DEPT(kernel, dept, argv)                \
extern const arnicsKernel_t* (kernel)(void);                      \
arnics_kernel_execute_by_dept((kernel()), (dept), (argv))

/**
 * @brief 便捷宏：按部门+名称执行（kernel 为句柄函数名）
 */
#define ARNICS_EXECUTE_BY_NAME(kernel, dept, name, argv)          \
extern const arnicsKernel_t* (kernel)(void);                      \
arnics_kernel_execute_by_name((kernel()), (dept), (name), (argv))

/**
 * @brief 便捷宏：按部门+等级执行（kernel 为句柄函数名）
 */
#define ARNICS_EXECUTE_BY_LEVEL(kernel, dept, level, argv)        \
extern const arnicsKernel_t* (kernel)(void);                      \
arnics_kernel_execute_by_level((kernel()), (dept), (level), (argv))

/* function declaration ---------------------------------------------*/
/**
 * @brief 打印 Arnics Logo（含版本号）
 */
extern                         void  logo_print(void);
/**
 * @brief 全局启动入口：执行 INIT_TAG 部门的初始化条目，并打印 logo
 */
extern                   void  arnics_core_init(void);

#ifdef __cplusplus
}
#endif
#endif
/************************ (C) COPYRIGHT ARSTUDIO *****END OF FILE***************************/
