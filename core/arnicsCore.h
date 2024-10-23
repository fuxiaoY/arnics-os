#ifndef _ARNICS_CORE_H_
#define _ARNICS_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../Inc/projDefine.h"
#include "../Inc/typedef.h"

#if defined(__CC_ARM) || defined(__GNUC__) /* ARM,GCC*/ 
    #define _SECTION(x)                  __attribute__((section(x)))
    #define _UNUSED                      __attribute__((unused))
    #define _USED                        __attribute__((used))
    #define _ALIGN(n)                    __attribute__((aligned(n)))
    #define _WEAK                        __attribute__((weak))
#elif defined (__ICCARM__)                 /*IAR */
    #define _SECTION(x)                  @ x
    #define _UNUSED                      
    #define _USED                        __root
    #define _WEAK                        __weak
#else
    #error "do not supported!"
#endif

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
    void (*func)(void);             
    int level;  
} ArnicsFuncItem;

/*---------------------------------------------------------------*/
#define  STRCAT(a, b ,c)                 #a "." b "."  #c

#define  ARNICS_CONN(type, funcname, line)  type  funcname##_##line
#define  ARNICS_DEF(type,funcname,line)     ARNICS_CONN(type, funcname, line)
#define  ARNICS_TYPE(type,funcname)         ARNICS_DEF(type, funcname, __LINE__)

#define ARNICS_REGISTER(name, func, department, level) \
    _USED ARNICS_TYPE(const ArnicsFuncItem, funCb_##func) \
    _SECTION(STRCAT(arnics, department, level)) = {name, func, level}
#define INIT_TAG "init"
#define DRIVER_INIT(name,func)      ARNICS_REGISTER(name,func,INIT_TAG,1)
#define SYSTEM_INIT(name,func)      ARNICS_REGISTER(name,func,INIT_TAG,2)
#define COMPONENT_INIT(name,func)   ARNICS_REGISTER(name,func,INIT_TAG,3)
#define DEPARTMENT_INIT(name,func)  ARNICS_REGISTER(name,func,INIT_TAG,4)


/*---------------------------------------------------------------*/


// 定义 nop_process 函数
static inline void nop_process(void) {}
#define DEFINE_ARNICS_FUNC_ITEM(name, department, level) \
    const ArnicsFuncItem name _SECTION(STRCAT(arnics, department, level)) = \
    { \
        "", nop_process \
    }

#define DEFINE_ARNICS_FUNC_ITEM_RANGE(prefix, department, start_level, end_level) \
    DEFINE_ARNICS_FUNC_ITEM(prefix##_start, department, start_level); \
    DEFINE_ARNICS_FUNC_ITEM(prefix##_end, department, end_level)


#define EXECUTE_FUNC_ALLSECTION(prefix) \
    do { \
        const ArnicsFuncItem* it = &prefix##_start; \
        const ArnicsFuncItem* end = &prefix##_end; \
        while (it < end) { \
            if (it->func != NULL) { \
                it->func(); \
            } \
            it++; \
        } \
    } while (0)
// 根据名称执行函数的宏
#define EXECUTE_FUNC_BY_NAME(prefix, Name) \
    do { \
        const ArnicsFuncItem* it = &prefix##_start; \
        const ArnicsFuncItem* end = &prefix##_end; \
        while (it < end) { \
            if (it->name != NULL && strcmp(it->name, Name) == 0) { \
                if (it->func != NULL) { \
                    it->func(); \
                } \
                break; \
            } \
            it++; \
        } \
    } while (0)
#define EXECUTE_FUNC_BY_NAME_AT_LEVEL(prefix, Name, level) \
    do { \
        const ArnicsFuncItem* it = &prefix##_start; \
        const ArnicsFuncItem* end = &prefix##_end; \
        while (it < end) { \
            if (it->level == level && it->name != NULL && strcmp(it->name, Name) == 0) { \
                if (it->func != NULL) { \
                    it->func(); \
                } \
                break; \
            } \
            it++; \
        } \
    } while (0)



extern void logo_print();
extern void arnics_core_init();



#ifdef __cplusplus
}
#endif
#endif
