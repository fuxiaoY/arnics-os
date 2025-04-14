#include "delay.h"
#ifdef STM32CHIP
void Delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, reload, tcnt = 0;


    reload = SysTick->LOAD; // 获取重装载寄存器值
    ticks = nus * (SystemCoreClock / 1000000); // 计算需要的计数值

    told = SysTick->VAL; // 获取当前数值寄存器值（开始时数值）

    while (1) {
        tnow = SysTick->VAL; // 获取当前数值寄存器值

        if (tnow != told) { // 当前值不等于开始值说明已在计数
            if (tnow < told) { // 当前值小于开始数值，说明未计到0
                tcnt += told - tnow; // 计数值=开始值-当前值
            } else { // 当前值大于开始数值，说明已计到0并重新计数
                tcnt += reload - tnow + told; // 计数值=重装载值-当前值+开始值
            }

            told = tnow; // 更新开始值

            if (tcnt >= ticks) break; // 时间超过/等于要延迟的时间,则退出.
        }
    }
}
void Delay_ms(uint32_t ms)
{
    uint32_t ticks;
    uint32_t told, tnow, reload, tcnt = 0;

    reload = SysTick->LOAD; // 获取重装载寄存器值
    ticks = ms * (SystemCoreClock / 1000); // 计算需要的计数值

    told = SysTick->VAL; // 获取当前数值寄存器值（开始时数值）

    while (1) {
        tnow = SysTick->VAL; // 获取当前数值寄存器值

        if (tnow != told) { // 当前值不等于开始值说明已在计数
            if (tnow < told) { // 当前值小于开始数值，说明未计到0
                tcnt += told - tnow; // 计数值=开始值-当前值
            } else { // 当前值大于开始数值，说明已计到0并重新计数
                tcnt += reload - tnow + told; // 计数值=重装载值-当前值+开始值
            }

            told = tnow; // 更新开始值

            if (tcnt >= ticks) break; // 时间超过/等于要延迟的时间,则退出.
        }
    }

}
#else
void Delay1us()     //@80MHz
{
    unsigned char i;

    __asm__("nop");
    __asm__("nop");
    i = 40;  // 调整循环次数以接近1微秒
    while (--i);
}

void Delay1ms()     //@80MHz
{
    unsigned int i, j;

    i = 4000;  // 外层循环次数
    j = 20;    // 内层循环次数

    while (i--) {
        j = 20;
        while (j--);
    }
}
// 毫秒级延时函数
void Delay_ms(uint32_t ms)
{
    while (ms--)
    {
        Delay1ms();
    }
}

// 微秒级延时函数
void Delay_us(uint32_t nus)
{
    while (nus--)
    {
        Delay1us();
    }
}
#endif



#include "../common/TaskTimer.h"
void test_delay_check(void) {
    uint32_t start_tick = arnics_getTick();
    Delay_ms(10); // 延时10ms
    uint32_t end_tick = arnics_getTick();
    
    uint32_t elapsed_time = end_tick - start_tick;

    printf("Delay test. Expected: 10ms, Actual: %d ms\n", elapsed_time);
    
}


