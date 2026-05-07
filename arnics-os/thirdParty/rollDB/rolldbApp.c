#include "rolldbPort.h"

// #define TS_RECORD_MAX_LENGTH    200
// static char s_message[TS_RECORD_MAX_LENGTH];


void ts_record_timestamp(const char *fmt, ...) 
{
    // memset(s_message, 0, TS_RECORD_MAX_LENGTH);
    // rtcTimeDateTypeDef_t DT;
    // dev_ctl(&rtc_ds, RTC_GETDATETIME, &DT);
    // snprintf(s_message, sizeof(s_message), "[%02d-%02d-%02d %02d:%02d:%02d] [TSDB]: ", DT.Year, DT.Month, DT.Date,DT.Hour, DT.Minute, DT.Second);
    // uint16_t tim_len = strlen(s_message);

    // va_list ap;
    // va_start(ap, fmt);
    // vsnprintf(s_message + tim_len, TS_RECORD_MAX_LENGTH - tim_len, fmt, ap);
    // va_end(ap);

    // printf("%s\r\n", s_message);
    // ts_record((uint8_t *)s_message, strlen(s_message));
}


void iwdg_reload_counter(void)
{
}