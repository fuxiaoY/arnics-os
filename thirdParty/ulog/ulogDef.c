#include "ulogDef.h"
#include "../../dataPlat/dataPlatInclude.h"
#include "../../drivers/driversInclude.h"
static void CurrentTimeToString(rtcTimeDateTypeDef_t* DT, char* buffer, size_t bufferSize)
{
  snprintf(buffer, bufferSize, "%02d-%02d-%02d %02d:%02d:%02d", DT->Year, DT->Month, DT->Date,DT->Hour, DT->Minute, DT->Second);
}
static void DEBUG_logger(ulog_level_t severity, char *msg)
{
  rtcTimeDateTypeDef_t DT;
  dev_ctl(&rtc_ds,RTC_GETDATETIME,&DT);
  char timeStr[32] ={0};
  CurrentTimeToString(&DT, timeStr, sizeof(timeStr));
  if(virtual_environment)
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

void usr_ulog_init(void)
{
  ULOG_INIT();
  ULOG_SUBSCRIBE(DEBUG_logger, ULOG_INFO_LEVEL);
}
void usr_ulog_deinit(void)
{
  ULOG_UNSUBSCRIBE(DEBUG_logger);
}
void usr_ulog_level_change(ulog_level_t level)
{
  ULOG_UNSUBSCRIBE(DEBUG_logger);
  ULOG_SUBSCRIBE(DEBUG_logger, level);
}
