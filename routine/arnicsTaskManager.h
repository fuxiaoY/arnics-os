﻿#ifndef _AENICSTASKMANAGER_H
#define _AENICSTASKMANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../Inc/projDefine.h"
#include "../Inc/typedef.h"
#include "../core/coreInclude.h"


extern const tTaskFunc initTaskList[];
extern const tTaskFunc preloadTaskList[]; 
extern const tTaskFunc deviceTaskList[]; 



extern void driver_init(void);
extern void third_init(void);

#ifdef __cplusplus
}
#endif
#endif
