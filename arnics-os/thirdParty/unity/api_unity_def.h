#ifndef _API_UNITYDEF_H
#define _API_UNITYDEF_H
#ifdef __cplusplus
extern "C" {
#endif


#include "unity.h"




// 回调函数类型声明
typedef void (*SetUpCallback)(void);
typedef void (*TearDownCallback)(void);

// 初始化回调函数的接口
extern void setSetUpCallback(SetUpCallback callback);
extern void setTearDownCallback(TearDownCallback callback);


extern void setUp(void) ;
extern void tearDown(void) ;
extern void testUnity(void) ;

#ifdef __cplusplus
}
#endif

#endif
