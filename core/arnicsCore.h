#ifndef _ARNICS_CORE_H_
#define _ARNICS_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../Inc/projDefine.h"
#include "../Inc/typedef.h"

#define TASK_FUNC(func)   {func}
/* \brief Function pointer define */
typedef void (*funcPointer) (void);

typedef struct
{
    funcPointer func;

} tTaskFunc;


/*匿名类型定义 -----------------------------------------------------------*/
#define  ARNICS_ANONY_CONN(type, var, line)  type  var##line
#define  ARNICS_ANONY_DEF(type,prefix,line)  ARNICS_ANONY_CONN(type, prefix, line)
#define  ARNICS_ANONY_TYPE(type,prefix)      ARNICS_ANONY_DEF(type, prefix, __LINE__)

#if defined(__CC_ARM) || defined(__GNUC__) /* ARM,GCC*/ 
    #define _SECTION(x)                  __attribute__((section(x)))
    #define _UNUSED                      __attribute__((unused))
    #define _USED                        __attribute__((used))
    #define _ALIGN(n)                    __attribute__((aligned(n)))
    #define _WEAK                        __attribute__((weak))
#elif defined (__ICCARM__)              /*IAR */
    #define _SECTION(x)                  @ x
    #define _UNUSED                      
    #define _USED                        __root
    #define _WEAK                        __weak
#else
    #error "Current tool chain haven't supported yet!"
#endif

/*---------------------------------------------------------------*/
#define ARRAY_COUNT(Array) (sizeof(Array) / sizeof(Array[0]))


#define ARNICS_REGISTER(name,func,level)           \
    _USED ARNICS_ANONY_TYPE(const init_item_t, init_tbl_##func)\
    _SECTION("arnics.init."level) = {name,func}

#define DRIVER_INIT(name,func)  ARNICS_REGISTER(name,func,"1")
#define SYSTEM_INIT(name,func)  ARNICS_REGISTER(name,func,"2")
#define COMPONENT_INIT(name,func)  ARNICS_REGISTER(name,func,"3")
#define DEPARTMENT_INIT(name,func)  ARNICS_REGISTER(name,func,"4")


typedef struct 
{
    const char *name;               
    void (*init)(void);             
}ArnicsInitItem,*pArnicsInitItem;;



struct ArnicsCoreData
{
    volatile uint32_t arnics_systick;
};
extern ArnicsCoreData arnics_core_data;
extern void arnics_addTick(uint32_t addTime);
extern void arnics_systick_handler();
extern void arnics_core_init();

/* public fun  -----------------------------------------------------------*/
extern const tTaskFunc initTaskList[];
extern const tTaskFunc preloadTaskList[]; 
extern const tTaskFunc deviceTaskList[]; 

#ifdef __cplusplus
}
#endif
#endif
