#include "api_unity_def.h"
#include "../../Inc/typedef.h"

// 全局函数指针变量，用于保存用户定义的回调函数
static SetUpCallback globalSetUpCallback = NULL;
static TearDownCallback globalTearDownCallback = NULL;

// 设置setUp阶段的回调函数
void setSetUpCallback(SetUpCallback callback)
{
    globalSetUpCallback = callback;
}

// 设置tearDown阶段的回调函数
void setTearDownCallback(TearDownCallback callback)
{
    globalTearDownCallback = callback;
}

void setUp(void)
{
    if (globalSetUpCallback != NULL)
    {
        globalSetUpCallback();
    }
}

void tearDown(void)
{
    if (globalTearDownCallback != NULL)
    {
        globalTearDownCallback();
    }
}


void testUnity(void)
{
	UNITY_BEGIN();
   //RUN_TEST(func); 
	UNITY_END();
}
