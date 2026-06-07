#include "thirdParty/rollDB/rolldbPort.h"
#include "uflog.h"
#if PLATFORM_MCU
#include "drivers/driversInclude.h"
#elif PLATFORM_LINUX
#include <time.h>
#include <stdio.h>
#elif PLATFORM_WIN
#include <windows.h>
#include <stdio.h>
#else
    #error "Please select a platform"
#endif



/*port --------------------------------------------------------------*/
void uflog_get_timestamp(char *timestamp, int timestamp_len)
{
#if PLATFORM_MCU
    //TODO:rtcTimeDateTypeDef_t DT;
    //TODO:dev_ctl(&rtc_ds,RTC_GETDATETIME,&DT);
    //TODO:snprintf(timestamp, timestamp_len, "%02d-%02d-%02d %02d:%02d:%02d", DT.Year, DT.Month, DT.Date,DT.Hour, DT.Minute, DT.Second);
#elif PLATFORM_LINUX
    time_t rawtime;
    struct tm tm_info;
    time(&rawtime);
    localtime_r(&rawtime, &tm_info);
    snprintf(timestamp, timestamp_len, "%04d-%02d-%02d %02d:%02d:%02d", 
             tm_info.tm_year + 1900, tm_info.tm_mon + 1, tm_info.tm_mday,
             tm_info.tm_hour, tm_info.tm_min, tm_info.tm_sec);
#elif PLATFORM_WIN
    SYSTEMTIME st;
    GetLocalTime(&st);
    snprintf(timestamp, timestamp_len, "%04d-%02d-%02d %02d:%02d:%02d", 
             st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

#else
    #error "Please select a platform"   
#endif
}

void log_printf(char *out_str)
{
    printf("%s", out_str);
}

void log_auto_store(char * str)
{
    ts_record((uint8_t*)str,strlen(str));
}
