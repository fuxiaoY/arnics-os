// #include "drivers/driversInclude.h"
#include "uflog.h"

/*port --------------------------------------------------------------*/
// void uflog_get_timestamp(char *timestamp, int timestamp_len)
// {
//     if (timestamp && timestamp_len > 0) 
//     {
//         snprintf(timestamp, timestamp_len, "9999-01-01 00:00:00");
//     }
// }
void uflog_get_timestamp(char *timestamp, int timestamp_len)
{
    //TODO:rtcTimeDateTypeDef_t DT;
    //TODO:dev_ctl(&rtc_ds,RTC_GETDATETIME,&DT);
    //TODO:snprintf(timestamp, timestamp_len, "%02d-%02d-%02d %02d:%02d:%02d", DT.Year, DT.Month, DT.Date,DT.Hour, DT.Minute, DT.Second);
}

void log_printf(char *out_str)
{
    printf("%s", out_str);
}

void log_auto_store(char * str)
{

}
