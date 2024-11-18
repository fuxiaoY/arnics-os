#include "command_console.h"
#include "../../common/mctLib.h"
#include "../../../ulog/ulogDef.h"
#include "../../../../port/portInclude.h"
#include "../../../../dataPlat/dataPlatInclude.h"
#include "../../../../rtosInterface/entry_rtos_api.h"

static void handleCmd_7F(const uint8_t *buf, size_t len,uint8_t *packbuf, size_t* packlen)
{
    NVIC_SystemReset();
}
static void handleCmd_80(const uint8_t *buf, size_t len,uint8_t *packbuf, size_t* packlen)
{
    cpuInfo();
}

static void handleCmd_E0(const uint8_t *buf, size_t len,uint8_t *packbuf, size_t* packlen)
{
  rtosTaskSuspendAll();
  {
    UnitySystemInterface((const char*)buf);
  }
  rtosTaskResumeAll();

}
static void handleCmd_FF(const uint8_t *buf, size_t len,uint8_t *packbuf, size_t* packlen)
{
    const char version[] = "V1.0.0\n";
    *packlen = strlen(version);
    memcpy(packbuf, version, *packlen);

}


CommandHandler commandHandlers[] = {
    {0x7F, handleCmd_7F},
    {0x80, handleCmd_80},
    {0xE0, handleCmd_E0},
    {0xFF, handleCmd_FF},
};
// 查表的大小
const size_t numHandlers = sizeof(commandHandlers) / sizeof(commandHandlers[0]);

static void processCommand(const uint8_t *cmd, const uint8_t  *arg, size_t argLen,uint8_t *buf, size_t* len)
{
  // 查找命令
  for (size_t i = 0; i < numHandlers; ++i)
  {
    if (memcmp(cmd, &commandHandlers[i].command, 1) == 0)
    {
      commandHandlers[i].handler(arg, argLen,buf,len);
      return;
    }
  }
}




static bool cmd_PackRevFlow(uint8_t* buf, size_t* len, void *para)
{
    command_t *console_cmd = (command_t *)para;
    processCommand(&console_cmd->commad_rev.cmd, console_cmd->commad_rev.buf, console_cmd->commad_rev.len, \
                        buf,len);
    

    return true;
}

static bool cmd_AnalyzeRevFlow(uint8_t* buf, size_t len, void *para)
{
    command_t *console_cmd = (command_t *)para;
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
    if(remainLen < strlen("*#*#"))
    {
        return false;
    }
    remainLen -= strlen("*#*#");

    console_cmd->commad_rev.cmd = Cmd;
    console_cmd->commad_rev.buf =  ptr;
    console_cmd->commad_rev.len = remainLen;

    return true;
}
static bool cmd_PackRevHexFlow(uint8_t* buf, size_t* len, void *para)
{

    printf("cmd_PackRevHexFlow\r\n");
    return true;
}
//7e len cmd 7e
static bool cmd_AnalyzeRevHexFlow(uint8_t* buf, size_t len, void *para)
{
    printf("cmd_AnalyzeRevHexFlow\r\n");
    return true;
}



static const char header[2] = {0x7e,0x00};
static const char tail[2] = {0x7e,0x00};

static const tCmd cmdList[] =
{
    CMD_ADD(CMD_CONSOLE_ID_REV,             2,  "$$COMX$$",          "*#*#",         NULL,         RecvSend,        RevFlow),
    CMD_ADD(CMD_CONSOLE_ID_REV2,            2,  (char*)&header,     (char*)&tail,    NULL,        RecvSend,        RevHexFlow),
};
tCmd const *CMD_ConsoleCmdGet(void)
{
    return cmdList;
}

uint16_t CMD_ConsoleCmdNumGet(void)
{
    return (sizeof(cmdList) / sizeof(tCmd));
}
