#include "delay.h"
#include "drivers/driversInclude.h"
void Delay_us(uint32_t us)
{
    dev_ctl(&delay_ds,DELAY_US,us);
}
void Delay_ms(uint32_t ms)
{
    dev_ctl(&delay_ds,DELAY_MS,ms);
}
#include "../common/TaskTimer.h"
void test_delay_check(void) {
    uint32_t start_tick = arnics_getTick();
    Delay_ms(10); // 延时10ms
    uint32_t end_tick = arnics_getTick();
    
    uint32_t elapsed_time = end_tick - start_tick;
    printf("Delay test. Expected: 10ms, Actual: %d ms\n", elapsed_time);
    
}


