#ifndef _DELAY_H_
#define _DELAY_H_   

#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#include "port/arnicsPort.h"
extern void Delay_us(uint32_t us); 
extern void Delay_ms(uint32_t ms);
extern void test_delay_check(void);


#ifdef __cplusplus
}
#endif

#endif
