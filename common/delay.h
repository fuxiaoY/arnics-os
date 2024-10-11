#ifndef _DELAY_H_
#define _DELAY_H_   

#ifdef __cplusplus
extern "C" {
#endif
#include "../Inc/typedef.h"
#include "../Inc/ProjDefine.h"

void Delay_us(uint32_t ms); 
void Delay_ms(uint32_t ms);
void test_delay_check();


#ifdef __cplusplus
}
#endif

#endif
