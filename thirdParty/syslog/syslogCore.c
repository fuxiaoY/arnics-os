#include "syslogCore.h"
#include "syslogPort.h"



/**
 * 生成并输出一条符合RFC5424格式的syslog日志。
 *
 * RFC5424格式如下:
 * 
 *   - PRI:             优先级，等于 (facility << 3) | severity
 *   - VERSION:         协议版本号，通常为1
 *   - TIMESTAMP:       时间戳，格式为YYYY-MM-DDTHH:MM:SSZ（UTC时间）
 *   - HOSTNAME:        主机名
 *   - APPNAME:         应用名
 *   - PROCID:          进程ID
 *   - MSGID:           消息ID
 *   - STRUCTURED-DATA: 结构化数据（本实现用"-"占位）
 *   - MSG:             日志正文
 *
 *    <7>1 2024-05-01T13:45:30Z - business 0 MSG001 - sending message to eventos:rfid_action
 */
void syslog_log(syslog_facility_t facility, syslog_severity_t severity,
                const char *app_name, const char *procid, const char *msgid, 
                const char *function, int line,
                const char *fmt, ...)
{
    char buffer[SYSLOG_MAX_MSG_LEN]; 
    int pri = (facility << 3) | (severity & 0x7);
    int len = 0;

    /* ------- RFC5424 头部构建 ------- */
    // 1. PRI: 优先级 = (facility << 3) | severity
    //    VERSION: 版本号，固定为 1
    len = snprintf(buffer, sizeof(buffer), "<%d>1 ", pri);

    // 2. TIMESTAMP: ISO8601格式时间戳 (YYYY-MM-DDTHH:MM:SSZ)
    char timestamp[32];
    syslog_port_get_timestamp(timestamp, sizeof(timestamp));
    len += snprintf(buffer + len, sizeof(buffer) - len, "%s ", timestamp);

    // 3. HOSTNAME: 主机名（本实现用"-"占位）
    len += snprintf(buffer + len, sizeof(buffer) - len, "- ");

    // 4. APP-NAME: 应用名
    len += snprintf(buffer + len, sizeof(buffer) - len, "%s ", 
                   app_name ? app_name : "-");

    // 5. PROCID: 进程ID
    len += snprintf(buffer + len, sizeof(buffer) - len, "%s ",
                   procid ? procid : "-");

    // 6. MSGID: 消息ID
    len += snprintf(buffer + len, sizeof(buffer) - len, "%s ", 
                   msgid ? msgid : "-");

    // 7. STRUCTURED-DATA: 结构化数据 SD-ID "KEY="VALUE" 根据协议定义，value必须为字符串
    len += snprintf(buffer + len, sizeof(buffer) - len, "[location FILE=\"%s\" LINE=\"%d\"] ",
   function, line);


    // 8. MSG: 消息正文
    if (len < (int)sizeof(buffer)) {
        va_list args;
        va_start(args, fmt);
        int remain = sizeof(buffer) - len - 2; // 预留\r\n和\0
        if (remain > 0) {
            int msg_len = vsnprintf(buffer + len, remain + 1, fmt, args);
            // vsnprintf自动截断，保证不会越界
            len += (msg_len > remain) ? remain : msg_len;
        }
        va_end(args);
    }

    // 添加行终止符，保证不越界
    if (len > (int)sizeof(buffer) - 3) 
    {
        len = (int)sizeof(buffer) - 3;
    }
    buffer[len++] = '\r';
    buffer[len++] = '\n';
    buffer[len] = '\0';

    syslog_port_print(buffer);
}

