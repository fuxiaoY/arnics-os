#include "syslogPort.h"
#include "drivers/driversInclude.h"


void syslog_port_print(const char *msg)
{
    dev_write(&debug_ds, (void*)msg, strlen(msg));
}
// example: 2024-05-01T13:45:30Z
static void CurrentTimeToString(rtcTimeDateTypeDef_t* DT, char* buffer, size_t bufferSize)
{
    snprintf(buffer, bufferSize, "%02d-%02d-%02dT%02d:%02d:%02dZ", DT->Year, DT->Month, DT->Date,DT->Hour, DT->Minute, DT->Second);
}

void syslog_port_get_timestamp(char *buf, unsigned int len)
{
    rtcTimeDateTypeDef_t DT;
    dev_ctl(&rtc_ds,RTC_GETDATETIME,&DT);
    CurrentTimeToString(&DT, buf, len);
}
