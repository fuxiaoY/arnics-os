#include "usr_assert.h"

#ifdef _USE_USERASSERT_ 

void critical_enter(void)
{
#if PLATFORM_MCU
    void     __disable_irq();
    __disable_irq();
#elif PLATFORM_LINUX

#elif PLATFORM_WIN

#else
    #error "Please select a platform"
#endif
}




unsigned int Error_id = 0;

void user_port_assert(const char * funcname,const char * file,unsigned int error_id)
{
    Error_id = error_id;
    printf("------------------------------------\r\n");
    printf("ASSERT >>> User, func: %s  file:%s ErrorId: %d.\r\n",funcname,file, Error_id);
    printf("------------------------------------\r\n");
    
    while (1) 
    {
    }
}
#endif
