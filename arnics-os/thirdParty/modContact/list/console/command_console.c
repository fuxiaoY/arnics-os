#include "command_console.h"
#include "../../common/mctLib.h"
#include "../../../ulog/ulogDef.h"
#include "../../../../port/portInclude.h"
static void handleCmd_7F(const uint8_t *buf, size_t len)
{
    NVIC_SystemReset();
}

CommandHandler commandHandlers[] = {

    {0x7F, handleCmd_7F},
};
// 查表的大小
const size_t numHandlers = sizeof(commandHandlers) / sizeof(commandHandlers[0]);

static void processCommand(const uint8_t *cmd, const uint8_t  *arg, size_t argLen)
{
  // 查找命令
  for (size_t i = 0; i < numHandlers; ++i)
  {
    if (memcmp(cmd, &commandHandlers[i].command, 1) == 0)
    {
      commandHandlers[i].handler(arg, argLen);
      return;
    }
  }
}




static bool cmd_PackRevFlow(uint8_t* buf, size_t len, void *para)
{


    return true;
}

static bool cmd_AnalyzeRevFlow(uint8_t* buf, size_t len, void *para)
{
    uint8_t Cmd = 0;
    int remainLen = (int)len;

    uint8_t *ptr = buf;
    // 找到 "$$COMX$$" 的位置
    ptr = hexhex(buf, (uint8_t *)"$$COMX$$", remainLen, strlen("$$COMX$$"));
    // 移动指针到 "$$COMX$$" 之后的位置
    ptr += strlen("$$COMX$$");

    // 解析命令字，两个字节组成的HEX，
    Cmd = hexChrToAscii((const char*)ptr);
    ptr += 2;
    remainLen -= (ptr - buf);
    remainLen -= strlen("*#*#");
    if(remainLen >= 0)
    {
        processCommand(&Cmd, ptr, remainLen);
    }


    return true;
}





static const tCmd cmdList[] =
{
    CMD_ADD(CMD_CONSOLE_ID_REV,             2,  "$$COMX$$",          "*#*#",         NULL,         RecvSend,        RevFlow),

};
tCmd const *CMD_ConsoleCmdGet(void)
{
    return cmdList;
}

uint16_t CMD_ConsoleCmdNumGet(void)
{
    return (sizeof(cmdList) / sizeof(tCmd));
}
