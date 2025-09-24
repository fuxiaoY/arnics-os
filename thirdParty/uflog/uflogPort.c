#include "drivers/driversInclude.h"
#include "uflog.h"


void uflog_get_timestamp(char *timestamp, int timestamp_len)
{
    rtcTimeDateTypeDef_t DT;
    dev_ctl(&rtc_ds,RTC_GETDATETIME,&DT);
    snprintf(timestamp, timestamp_len, "%02d-%02d-%02d %02d:%02d:%02d", DT.Year, DT.Month, DT.Date,DT.Hour, DT.Minute, DT.Second);
}

