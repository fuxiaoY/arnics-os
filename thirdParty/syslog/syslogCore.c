#include "syslogCore.h"
#include "syslogPort.h"

static syslog_control_t syslog_control = SYSLOG_CTRL_OFF;

syslog_control_t *syslog_flags(void)
{
    return &syslog_control;
}

syslog_structured_data_t create_structed_data(const char* sd_id, const char* fmt, ...)
{
    syslog_structured_data_t data;
    if (SYSLOG_CTRL_OFF ==*syslog_flags()) 
    {
        return data;
    }
    memset(&data, 0, sizeof(data));

    va_list args;
    va_start(args, fmt);

    // 第一次计算格式化长度
    int fmt_len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    // 计算前缀 [sd_id 空格 所需长度
    int prefix_len = snprintf(NULL, 0, "[%s ", sd_id);
    int required_len = prefix_len + fmt_len + 1; // +1 for closing ]

    // 检查总长度是否足够
    if ((size_t)required_len >= sizeof(data.struct_data_element)) 
    {
        return data;
    }

    // 重新开始读取参数
    va_start(args, fmt);
    int len = snprintf(data.struct_data_element, sizeof(data.struct_data_element), "[%s ", sd_id);
    int written = vsnprintf(data.struct_data_element + len, sizeof(data.struct_data_element) - len, fmt, args);
    snprintf(data.struct_data_element + len + written, sizeof(data.struct_data_element) - len - written, "]");
    data.len = len + written + 1;  // +1 for closing ]
    va_end(args);

    return data;
}

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
 *    <ID>VERSION TIMESTAMP HOSTNAME APPNAME PROCID MSGID STRUCTURED-DATA MSG
 */
void syslog_log(syslog_facility_t facility, syslog_severity_t severity,
    const char *app_name, const char *procid, const char *msgid,
    const char *file, int line,
    const syslog_structured_data_t *structured_data, int sd_count,
    const char *fmt, ...)
{

    if (SYSLOG_CTRL_OFF ==*syslog_flags()) 
    {
        return;
    }

    char buffer[SYSLOG_MAX_MSG_LEN]; 
    int pri = (facility << 3) | (severity & 0x7);
    int len = 0;

    /* ------- RFC5424 头部构建 ------- */
    // 1. PRI: 优先级 = (facility << 3) | severity
    //    VERSION: 版本号，固定为 1
    len = snprintf(buffer, sizeof(buffer), "<%d>1 ", pri);
    if(SYSLOG_HAS_FLAG(SYSLOG_CTRL_SHOW_TIMESTAMP))
    {
        // 2. TIMESTAMP: ISO8601格式时间戳 (YYYY-MM-DDTHH:MM:SSZ)
        char timestamp[32];
        syslog_port_get_timestamp(timestamp, sizeof(timestamp));
        len += snprintf(buffer + len, sizeof(buffer) - len, "%s ", timestamp);
    }
    else
    {
        len += snprintf(buffer + len, sizeof(buffer) - len, "- ");
    }

    // 3. HOSTNAME: 主机名字段
    if (SYSLOG_HAS_FLAG(SYSLOG_CTRL_SHOW_HOSTNAME)) 
    {
        char hostname[64];
        syslog_port_get_hostname(hostname, sizeof(hostname));
        len += snprintf(buffer + len, sizeof(buffer) - len, "%s ", hostname);
    } 
    else 
    {
        len += snprintf(buffer + len, sizeof(buffer) - len, "- ");
    }

    // 4. APP-NAME: 应用名字段
    if (SYSLOG_HAS_FLAG(SYSLOG_CTRL_SHOW_CENTER_ID)) 
    {
        len += snprintf(buffer + len, sizeof(buffer) - len, "%s ", 
                       app_name ? app_name : "-");
    } 
    else 
    {
        len += snprintf(buffer + len, sizeof(buffer) - len, "- ");
    }

    // 5. PROCID: 进程ID/函数名字段
    if (SYSLOG_HAS_FLAG(SYSLOG_CTRL_SHOW_FUNCTION)) 
    {
        len += snprintf(buffer + len, sizeof(buffer) - len, "%s ",
                       procid ? procid : "-");
    } 
    else 
    {
        len += snprintf(buffer + len, sizeof(buffer) - len, "- ");
    }

    // 6. MSGID: 消息ID字段
    if (SYSLOG_HAS_FLAG(SYSLOG_CTRL_SHOW_MSGID)) 
    {
        len += snprintf(buffer + len, sizeof(buffer) - len, "%s ", 
                       msgid ? msgid : "-");
    } 
    else 
    {
        len += snprintf(buffer + len, sizeof(buffer) - len, "- ");
    }

    // 7. STRUCTURED-DATA: 结构化数据
    bool has_structured_data = false;
    
    // 7.1 文件与行号（如果启用）
    if (SYSLOG_HAS_FLAG(SYSLOG_CTRL_SHOW_FILE_LINE) && file) 
    {
        len += snprintf(buffer + len, sizeof(buffer) - len, 
                       "[location@00001 FILE=\"%s\" LINE=\"%d\"] ", file, line);
        has_structured_data = true;
    }
    // 7.2 STRUCTURED-DATA: 结构化数据 SD-ID "KEY="VALUE" 根据协议定义，value必须为字符串

    if (structured_data && sd_count > 0) 
    {
        for (int i = 0; i < sd_count; i++) 
        {
            if(0 != structured_data[i].len)
            {
                len += snprintf(buffer + len, sizeof(buffer) - len,
                    "%s", structured_data[i].struct_data_element);
                len += snprintf(buffer + len, sizeof(buffer) - len, " ");
                has_structured_data = true;
            }
        
        }
    } 
    // 7.3 如果没有结构化数据，添加 -
    if (!has_structured_data) 
    {
        len += snprintf(buffer + len, sizeof(buffer) - len, "- ");
    } 


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

