#include "ulogDef.h"
#include "../../dataPlat/entry_dataPlat_api.h"

static void CurrentTimeToString(rtcTimeDateTypeDef_t* DT, char* buffer, size_t bufferSize)
{
  snprintf(buffer, bufferSize, "%02d-%02d-%02d %02d:%02d:%02d", DT->Year, DT->Month, DT->Date,DT->Hour, DT->Minute, DT->Second);
}
static void DEBUG_logger(ulog_level_t severity, char *msg)
{
  rtcTimeDateTypeDef_t DT;
  char timeStr[32] ={0};
  CurrentTimeToString(&DT, timeStr, sizeof(timeStr));
  if(VirtualEnvironment)
  {
    printf("[Vir] [%s] [%s]: %s\r\n",
          timeStr, 
          ulog_level_name(severity),                           
          msg);
  }
  else
  {
    printf("[%s] [%s]: %s\r\n",
          timeStr, 
          ulog_level_name(severity),                           
          msg);
  }
}

void usr_ulog_init()
{
  ULOG_INIT();
  ULOG_SUBSCRIBE(DEBUG_logger, ULOG_INFO_LEVEL);
}
void usr_ulog_deinit()
{
  ULOG_UNSUBSCRIBE(DEBUG_logger);
}
void usr_ulog_level_change(ulog_level_t level)
{
  ULOG_UNSUBSCRIBE(DEBUG_logger);
  ULOG_SUBSCRIBE(DEBUG_logger, level);
}
