#ifndef _ARNICS_TASK_MANAGER_H_
#define _ARNICS_TASK_MANAGER_H_


#include "../Inc/projDefine.h"
#include "../Inc/typedef.h"



#define TASK_FUNC(func)   {func}
/* \brief Function pointer define */
typedef void (*funcPointer) (void);

typedef struct
{
    funcPointer func;

} tTaskFunc;

/* public fun  -----------------------------------------------------------*/
extern const tTaskFunc initTaskList[];
extern const tTaskFunc preloadTaskList[]; 
extern const tTaskFunc deviceTaskList[]; 


#endif // _ARNICS_TASK_MANAGER_H_   
