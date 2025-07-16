#ifndef SYSLOG_CORE_H
#define SYSLOG_CORE_H
#include "Inc/ProjDefine.h"
#include "Inc/typedef.h"
#include "syslogFormat.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * 结构化数据项
 */
typedef struct 
{
    char struct_data_element[SYSLOG_MAX_SINGLE_STRUCT_ID_LEN];
    uint16_t len;
} syslog_structured_data_t;

/**
 * syslog_log，支持多个STRUCTURED_DATA
 * @param facility   日志Facility
 * @param severity   日志Severity
 * @param app_name   应用名
 * @param procid     进程ID
 * @param msgid      消息ID
 * @param file       文件名
 * @param line        行号
 * @param structured_data 结构化数据数组指针
 * @param sd_count   结构化数据项数量
 * @param fmt        printf格式字符串
 * @param ...        可选的printf参数
 */
extern void syslog_log(syslog_facility_t facility, syslog_severity_t severity,
    const char *app_name, const char *procid, const char *msgid,
    const char *file, int line,
    const syslog_structured_data_t *structured_data, int sd_count,
    const char *fmt, ...);

/**
 * @brief 生成结构化数据
 */
extern syslog_structured_data_t create_structed_data(const char* sd_id, const char* fmt, ...);


#define SYSLOG(facility, severity, app_name, msgid, structured_data, sd_count, fmt, ...) \
syslog_log(              \
    facility,            \
    severity,            \
    app_name,            \
    __FUNCTION__,        \
    msgid,               \
    __FILE__,            \
    __LINE__,            \
    structured_data,     \
    sd_count,            \
    fmt,                 \
    ##__VA_ARGS__        \
)

extern syslog_control_t *syslog_flags(void);

/**
 * @brief 位操作宏定义
 */
extern syslog_control_t *syslog_flags(void);

/**
 * @brief 位操作宏定义 - C/C++ 兼容
 */
#ifdef __cplusplus
#define SYSLOG_SET_FLAG(flag)    ((*syslog_flags()) = (syslog_control_t)((uint32_t)(*syslog_flags()) | (uint32_t)(flag)))
#define SYSLOG_CLEAR_FLAG(flag)  ((*syslog_flags()) = (syslog_control_t)((uint32_t)(*syslog_flags()) & ~(uint32_t)(flag)))
#define SYSLOG_TOGGLE_FLAG(flag) ((*syslog_flags()) = (syslog_control_t)((uint32_t)(*syslog_flags()) ^ (uint32_t)(flag)))
#define SYSLOG_HAS_FLAG(flag)    (((uint32_t)(*syslog_flags()) & (uint32_t)(flag)) != 0)
#else
#define SYSLOG_SET_FLAG(flag)    ((*syslog_flags()) |= (flag))
#define SYSLOG_CLEAR_FLAG(flag)  ((*syslog_flags()) &= ~(flag))
#define SYSLOG_TOGGLE_FLAG(flag) ((*syslog_flags()) ^= (flag))
#define SYSLOG_HAS_FLAG(flag)    (((*syslog_flags()) & (flag)) != 0)
#endif

#ifdef __cplusplus
}
#endif

#endif // SYSLOG_CORE_H
