#include "command_console.h"
#include "../../common/mctLib.h"
#include "../../../ulog/ulogDef.h"
static bool parse()
{
return true;
}

static bool cmd_Packtest1(MctInstance *cb, void *para)
{
    ULOG_INFO("cmd_Packtest1");
    return true;
}

static bool cmd_Analyzetest1(MctInstance *cb, void *para)
{
    ULOG_INFO("cmd_Analyzetest1");
    return true;
}

static bool cmd_Packtest2(MctInstance *cb, void *para)
{
    ULOG_INFO("cmd_Packtest2");
    return true;
}

static bool cmd_Analyzetest2(MctInstance *cb, void *para)
{
    ULOG_INFO("cmd_Analyzetest2");
    return true;
}

static bool cmd_Packtest3(MctInstance *cb, void *para)
{
    ULOG_INFO("cmd_Packtest3");
    return true;
}

static bool cmd_Analyzetest3(MctInstance *cb, void *para)
{
    ULOG_INFO("cmd_Analyzetest3");
    return true;
}

static const tCmd cmdList[] =
{
    CMD_ADD(CMD_CONSOLE_ID_TEST1,             5,  "$$COMX$$",          "*#*#",         NULL,         SendRev,        test1),
    CMD_ADD(CMD_CONSOLE_ID_TEST2,             5,  "$$test2",            "*#*#",         NULL,        RecvSend,       test2),
    CMD_ADD(CMD_CONSOLE_ID_TEST3,             5,  "$$test3",            "*#*#",         NULL,        RecvSend,       test3)  
};
tCmd const *CMD_ConsoleCmdGet(void)
{
    return cmdList;
}

uint16_t CMD_ConsoleCmdNumGet(void)
{
    return (sizeof(cmdList) / sizeof(tCmd));
}
