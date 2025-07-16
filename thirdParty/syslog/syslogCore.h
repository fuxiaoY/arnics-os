#ifndef SYSLOG_CORE_H
#define SYSLOG_CORE_H
#include "Inc/ProjDefine.h"
#include "Inc/typedef.h"
#include "syslogFormat.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifndef SYSLOG_MAX_MSG_LEN
#define SYSLOG_MAX_MSG_LEN 512
#endif



// RFC5424日志API
extern void syslog_log(syslog_facility_t facility, syslog_severity_t severity,
    const char *app_name, const char *procid, const char *msgid, 
    const char *function, int line,
    const char *fmt, ...);

/**
 * SYSLOG日志宏，生成并输出一条符合RFC5424格式的syslog日志。
 *
 * 参数说明:
 *   facility   - 日志Facility（如SYSLOG_FACILITY_LOCAL0等）
 *   severity   - 日志Severity（如SYSLOG_SEVERITY_INFO等）
 *   app_name   - 应用名
 *   procid     - 进程ID
 *   msgid      - 消息ID
 *   fmt        - printf格式字符串
 *   ...        - 可选的printf参数
 *
 *   例子:
 *   <7>1 2024-05-01T13:45:30Z - business - - - sending message to eventos:rfid_action
 */

 /**
  * @brief     STRUCTURED_DATA(buffer, sizeof(buffer), "exampleSDID@32473",
                    "function=\"%s\" line=\"%d\"", __FUNCTION__, __LINE__);
  * 
  */
#define STRUCTURED_DATA(buffer, size, sd_id, ...) \
    snprintf(buffer, size, "[%s " __VA_ARGS__ "]", sd_id)

#define SYSLOG(facility, severity, app_name, msgid, fmt, ...) \
syslog_log(              \
    facility,            \
    severity,            \
    app_name,            \
    __FUNCTION__,        \
    msgid,               \
    __FILE__,            \
    __LINE__,            \
    fmt,                 \
    ##__VA_ARGS__        \
)



#ifdef __cplusplus
}
#endif

#endif // SYSLOG_CORE_H
