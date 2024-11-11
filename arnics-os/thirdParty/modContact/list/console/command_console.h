#ifndef _COMMAND_CONSOLE_H
#define _COMMAND_CONSOLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../common/mctProcesser.h"

#define CMD_CONSOLE_ID_BASE                           (uint16_t)0

#define CMD_CONSOLE_ID_TEST1                          (1+CMD_CONSOLE_ID_BASE)
#define CMD_CONSOLE_ID_TEST2                          (1+CMD_CONSOLE_ID_TEST1)
#define CMD_CONSOLE_ID_TEST3                          (1+CMD_CONSOLE_ID_TEST2)




extern tCmd const *CMD_ConsoleCmdGet(void);
extern uint16_t CMD_ConsoleCmdNumGet(void);

#define mct_console_execute(inst,is_expected,expected_tcmd_id, para)                  CMD_Execute(inst,is_expected,expected_tcmd_id,CMD_ConsoleCmdGet(), CMD_ConsoleCmdNumGet(), para)


#ifdef __cplusplus
}
#endif
#endif
