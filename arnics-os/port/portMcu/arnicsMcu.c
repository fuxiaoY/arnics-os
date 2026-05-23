#include "arnicsMcu.h"
#if PLATFORM_MCU
#include "drivers/driversInclude.h"
#ifdef _USE_REPRINTF_
#pragma import(__use_no_semihosting)
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;   
// 重定义 _sys_exit 函数
void _sys_exit(int x) 
{
    (void)x;  // 避免未使用警告
}

// 重定义 _ttywrch 函数
int _ttywrch(int ch) 
{
    // 实现你的输出逻辑，例如通过 UART 发送字符
    uart_dirct_t bufcache = {0};
    bufcache.buf = (uint8_t *)&ch;
    bufcache.size = 1;
    dev_ctl(&debug_ds, UART_DSEND, &bufcache);
    return ch;
}

// 重定义 fputc 函数
int fputc(int ch, FILE *stream) 
{
    return _ttywrch(ch);
}
#endif

#include "drivers/driversInclude.h"
void Delay_us(uint32_t us)
{
    dev_ctl(&delay_ds,DELAY_US,us);
}
void Delay_ms(uint32_t ms)
{
    dev_ctl(&delay_ds,DELAY_MS,ms);
}
#include "common/TaskTimer.h"
void test_delay_check(void) 
{
    uint32_t start_tick = arnics_getTick();
    Delay_ms(10); // 延时10ms
    uint32_t end_tick = arnics_getTick();
    
    uint32_t elapsed_time = end_tick - start_tick;
    printf("Delay test. Expected: 10ms, Actual: %d ms\n", elapsed_time);
    
}




#endif
