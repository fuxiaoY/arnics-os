#ifndef _COMMAND_CONSOLE_H
#define _COMMAND_CONSOLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../common/mctProcesser.h"



#define CMD_CONSOLE_ID_BASE                           (int32_t)0
#define CMD_CONSOLE_ID_REV                            (1+CMD_CONSOLE_ID_BASE)
#define CMD_CONSOLE_ID_REV2                           (1+CMD_CONSOLE_ID_REV)

typedef struct
{
  const uint8_t command;
  void (*handler)(const uint8_t *buf, size_t len,uint8_t *packbuf, size_t* packlen);
} CommandHandler;

typedef struct
{
  uint8_t cmd;
  uint8_t *buf;
  uint32_t len;

}command_rev_t;

typedef union 
{
  command_rev_t commad_rev;

} command_t;

extern tCmd const *CMD_ConsoleCmdGet(void);
extern uint16_t CMD_ConsoleCmdNumGet(void);
/**
 * @fn mct_console_execute
 * @brief 
 * 
 * @param [in] inst - mct实例

 * @param [in] expected_tcmd_id   - 期望的命令id   NULL_CMD_SEEK：无需匹配
 * @param [in] para - 回调参数
 * @retval None
 */
#define mct_console_execute(inst,expected_tcmd_id, para)                  CMD_Execute(inst,expected_tcmd_id,CMD_ConsoleCmdGet(), CMD_ConsoleCmdNumGet(), para)


#ifdef __cplusplus
}
#endif
#endif
