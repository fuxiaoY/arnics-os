#include "command_console.h"
#include "../../../ulog/ulogDef.h"
#include "../../../../port/portInclude.h"
#include "../../../../dataPlat/dataPlatInclude.h"
#include "../../../../rtosInterface/entry_rtos_api.h"
#include "../../../upgrade/upgradeManager.h"
#include "../../../rollDB/rolldbPort.h"
#include "../../../../drivers/driversInclude.h"
#include "../../dataPlat/dataLink.h"
static void CurrentTimeToString(rtcTimeDateTypeDef_t* DT, char* buffer, size_t bufferSize)
{
  snprintf(buffer, bufferSize, "%02d-%02d-%02d %02d:%02d:%02d", DT->Year, DT->Month, DT->Date,DT->Hour, DT->Minute, DT->Second);
}

static void handleCmd_02(const uint8_t *buf, size_t len,uint8_t *packbuf, size_t* packlen)
{
  if(len  == 0)
  {
    static uint32_t add_num = 0;
    add_num++;
    rtcTimeDateTypeDef_t DT;
    dev_ctl(&rtc_ds,RTC_GETDATETIME,&DT);
    char timeStr[32] ={0};
    CurrentTimeToString(&DT, timeStr, sizeof(timeStr));
    char tsdb[60] = {0};
    snprintf(tsdb,sizeof(tsdb) ,"[%s]add test record count: %d",timeStr,add_num);
    ts_record((uint8_t*)tsdb, strlen(tsdb));
    ULOG_ALWAYS("add test record: %d", add_num);
  }
    switch (buf[1])
    {
      case 'R':
      {
        if (len >= 4 && buf[3] == 'A' && buf[4] == 'L' && buf[5] == 'L')
        {
          ts_read_all();
        }
        else if (len >= 4 && buf[3] == 'N' && buf[4] == 'U' && buf[5] == 'M')
        {
          ts_read_sysinfo(); // 打印系统信息
          ULOG_ALWAYS("---------------");
          uint32_t log_num = ts_read_num();
          ULOG_ALWAYS("Log number: %d", log_num); // 打印日志数量
          uint32_t capacity = ts_read_capacity();
          ULOG_ALWAYS("Capacity: %d%%", capacity); // 打印日志容量
          uint32_t  capacity_size = ts_read_capacity_size(); 
          ULOG_ALWAYS("Capacity size: %d", capacity_size); // 打印日志容量大小
          
        }
        else if (len >= 6 && buf[2] == ',')
        {
          uint32_t start_num = 0; 
          uint32_t end_num = 0;
          if (sscanf((const char *)(buf + 3), "%u,%u", &start_num, &end_num) != 2)
          {
            return;
          }
          ts_read_pick(start_num, end_num);
        }
        else
        {
          return;
        }
        break;
      }
      case 'C':
      {
        ts_clear();
        ULOG_ALWAYS("Log cleared");
      }
      case 'I':
      {
        ts_repair();
      }
      break;
      default:
      break;
    }
}
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
static void handleCmd_E3(const uint8_t *buf, size_t len,uint8_t *packbuf, size_t* packlen)
{
  rtosTaskSuspendAll();
  {
    UnityGlobalStatInterface((const char*)buf);
  }
  rtosTaskResumeAll();

}
static void handleCmd_E4(const uint8_t *buf, size_t len,uint8_t *packbuf, size_t* packlen)
{
  rtosTaskSuspendAll();
  {
    UnityGlobalCfgInterface((const char*)buf);
  }
  rtosTaskResumeAll();

}
static void handleCmd_E5(const uint8_t *buf, size_t len,uint8_t *packbuf, size_t* packlen)
{
  rtosTaskSuspendAll();
  {
    ymodem_upgrade();
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

    // 数据库测试
    {0x02, handleCmd_02},
    // 软复位
    {0x7F, handleCmd_7F},
    // 读系统参数
    {0xE0, handleCmd_E0},
    // 读运行参数
    {0xE3, handleCmd_E3},
    // 读配置参数
    {0xE4, handleCmd_E4},
    // 读配置参数
    {0xE5, handleCmd_E5},
    // 系统堆栈情况
    {0x80, handleCmd_80},
    // 版本号
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



static uint8_t header = 0x7e;
static uint8_t tail = 0x7e;

static const tCmd cmdList[] =
{
  CMD_ADD(CMD_CONSOLE_ID_REV,             2,  "$$COMX$$",                      "*#*#",                     NULL,         RecvSend,        RevFlow),
  CMD_HEX_ADD(CMD_CONSOLE_ID_REV2,        2,  &header,sizeof(header),          &tail,sizeof(header),       NULL,0,       RecvSend,        RevHexFlow),
};
tCmd const *CMD_ConsoleCmdGet(void)
{
    return cmdList;
}

uint16_t CMD_ConsoleCmdNumGet(void)
{
    return (sizeof(cmdList) / sizeof(tCmd));
}
