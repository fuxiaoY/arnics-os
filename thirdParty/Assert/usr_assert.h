#ifndef _USR_ASSERT_H_
#define _USR_ASSERT_H_

#include "../../Inc/typedef.h"
#include "../../Inc/projDefine.h"
#ifdef __cplusplus
extern "C" {
#endif

/* assert ------------------------------------------------------------------- */
#ifdef _USE_USERASSERT_ 
#define USER_ASSERT(test_) do { if (!(test_)) {                   \
        critical_enter();                                             \
        user_port_assert(__FUNCTION__, __FILE__,__LINE__);                       \
    } } while (0)

#else
#define USER_ASSERT(test_)   ((void)0)
#endif


/* --------------------------------------------------------------------- */
void critical_enter(void);
void critical_exit(void);

void user_port_assert(const char * funcname,const char * file,unsigned int error_id);

#ifdef __cplusplus
}
#endif

#endif

