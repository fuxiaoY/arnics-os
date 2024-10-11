#include "usr_assert.h"
#include "../../Inc/typedef.h"
#ifndef _USE_USERASSERT_
void     __disable_irq()
{}
void     __enable_irq()
{}
#endif

void critical_enter(void)
{
    __disable_irq();
}


void critical_exit(void)
{
    __enable_irq();
}

unsigned int Error_id = 0;

void user_port_assert(const char * funcname,unsigned int error_id)
{
    Error_id = error_id;
    printf("------------------------------------\r\n");
    printf("ASSERT >>> User, func: %s  ErrorId: %d.\r\n",funcname, Error_id);
    printf("------------------------------------\r\n");
    
    while (1) 
    {
    }
}


