
#ifndef __PROCESSTASK_H__
#define __PROCESSTASK_H__
#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*process_Init)(void);
typedef void (*process_handle)(uint16_t id, void *para,void *argv);
struct process
{
    process_Init                        init;
    process_handle                      fun;
    struct process                      *next;
};

extern void PROCESS_INIT(void);
extern bool PROCESS_ADD(struct process *handle);
extern bool PROCESS_REMOVE(struct process *handle);
extern void PROCESS_TASK(uint16_t id, void *para,void *argv);

#ifdef __cplusplus
}
#endif
#endif
