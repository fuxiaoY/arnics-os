#include "thirdPartyInclude.h"
#include "Inc/ProjDefine.h"
#include "../core/coreInclude.h"


void unity_test(void)
{
#ifdef _USE_UNITYTEST_
   /**设置资源管理函数 **/
   setSetUpCallback(rolldbSetUp);
   setTearDownCallback(rolldbTearDown);

   // Unity测试开始
	UNITY_BEGIN();							 
   // RUN_TEST(testRollDB); 
	UNITY_END();
#endif
}



void thirdPartyInit(void)
{
   usr_ulog_init();
   fal_init();
   unity_test();
   ts_init();
   uflog_init(UFLOG_LEVEL_ALL,UFLOG_SHOW_FULL);
}
