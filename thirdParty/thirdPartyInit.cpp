#include "thirdPartyInclude.h"
#include "../core/coreInclude.h"


void unity_test(void)
{
   /**设置资源管理函数 **/
   //setSetUpCallback(appcheckSetUp);
   //setTearDownCallback(appcheckTearDown);
   // Unity测试开始
	UNITY_BEGIN();							 
   //RUN_TEST(test_get_voltage_percentage); 
	UNITY_END();

}



void thirdPartyInit(void)
{
   usr_ulog_init();
   unity_test();
   fal_init();
   
}
