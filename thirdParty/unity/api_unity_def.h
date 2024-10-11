#ifndef _API_UNITYDEF_H
#define _API_UNITYDEF_H


#include "unity.h"




// 回调函数类型声明
typedef void (*SetUpCallback)(void);
typedef void (*TearDownCallback)(void);

// 初始化回调函数的接口
void setSetUpCallback(SetUpCallback callback);
void setTearDownCallback(TearDownCallback callback);


void setUp(void) ;
void tearDown(void) ;


#endif
