#ifndef _COMMAND_CONSOLE_H
#define _COMMAND_CONSOLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../common/mctProcesser.h"



#define CMD_CONSOLE_ID_BASE                           (uint16_t)0
#define CMD_CONSOLE_ID_REV                            (1+CMD_CONSOLE_ID_BASE)


typedef struct
{
  const uint8_t command;
  void (*handler)(const uint8_t *buf, size_t len);
} CommandHandler;


extern tCmd const *CMD_ConsoleCmdGet(void);
extern uint16_t CMD_ConsoleCmdNumGet(void);

#define mct_console_execute(inst,is_expected,expected_tcmd_id, para)                  CMD_Execute(inst,is_expected,expected_tcmd_id,CMD_ConsoleCmdGet(), CMD_ConsoleCmdNumGet(), para)


#ifdef __cplusplus
}
#endif
#endif
