

#ifndef SYSLOG_FORMAT_H
#define SYSLOG_FORMAT_H
#include "Inc/ProjDefine.h"
#include "Inc/typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* config ----------------------------------------------------------*/
// 报文总长度
#define SYSLOG_MAX_MSG_LEN 512

// 结构化数据长度
#define SYSLOG_MAX_SINGLE_STRUCT_ID_LEN   64


/* define ----------------------------------------------------------*/
/**
 *   SYSLOG日志控制
 *   <ID>VERSION TIMESTAMP HOSTNAME APPNAME PROCID MSGID STRUCTURED-DATA MSG
 */
typedef enum
{
    SYSLOG_CTRL_OFF              = 0x00000000,  /* 关闭所有日志 */
    SYSLOG_CTRL_BASIC_ON         = 0x00000001,  /* 启用基本日志 <ID>VERSION - - - - - - MSG*/
    SYSLOG_CTRL_SHOW_TIMESTAMP   = 0x00000002,  /* 显示时间戳   <ID>VERSION TIMESTAMP - - - - - -*/
    SYSLOG_CTRL_SHOW_HOSTNAME    = 0x00000004,  /* 显示主机名   <ID>VERSION - HOSTNAME - - - - -*/
    SYSLOG_CTRL_SHOW_CENTER_ID   = 0x00000008,  /* 显示进程名   <ID>VERSION - - APPNAME - - - -*/
    SYSLOG_CTRL_SHOW_FUNCTION    = 0x00000010,  /* 显示函数名   <ID>VERSION - - - PROCID(FUNC) - - -*/
    SYSLOG_CTRL_SHOW_MSGID       = 0x00000020,  /* 显示消息ID   <ID>VERSION - - - - MSGID - -*/
    SYSLOG_CTRL_SHOW_FILE_LINE   = 0x00000040,  /* 显示文件名   <ID>VERSION - - - - - [location@00001 FILE LINE] -*/

   /*-------------------------------------------------*/
    SYSLOG_CTRL_STORE            = 0x00000100,  /* 是否自动存储日志 */


    /* 组合标志 */
    SYSLOG_CTRL_DEFAULT          = SYSLOG_CTRL_BASIC_ON|SYSLOG_CTRL_SHOW_TIMESTAMP, /* 默认打印 */
    SYSLOG_CTRL_FULL_DEBUG       = SYSLOG_CTRL_BASIC_ON|SYSLOG_CTRL_SHOW_TIMESTAMP|SYSLOG_CTRL_SHOW_HOSTNAME|
                                    SYSLOG_CTRL_SHOW_CENTER_ID|SYSLOG_CTRL_SHOW_FUNCTION|SYSLOG_CTRL_SHOW_MSGID|
                                    SYSLOG_CTRL_SHOW_FILE_LINE, /* 完整调试日志 */
} syslog_control_t;



/**
 * syslog_facility_t
 * 日志Facility（设施）定义，参见RFC5424标准。
 * Facility用于标识日志来源的子系统或模块，取值范围0-23。
 * 在计算PRI值时，Facility值会左移3位: (facility << 3) | severity
 */
typedef enum 
{
    SYSLOG_FACILITY_KERN = 0,     /* 内核消息 */
    SYSLOG_FACILITY_USER = 1,     /* 用户级消息 */
    SYSLOG_FACILITY_MAIL = 2,     /* 邮件系统消息 */
    SYSLOG_FACILITY_DAEMON = 3,   /* 系统守护进程消息 */
    SYSLOG_FACILITY_AUTH = 4,     /* 安全/认证消息 */
    SYSLOG_FACILITY_SYSLOG = 5,   /* syslogd内部生成的消息 */
    SYSLOG_FACILITY_LPR = 6,      /* 行式打印机子系统 */
    SYSLOG_FACILITY_NEWS = 7,     /* 网络新闻子系统 */
    SYSLOG_FACILITY_UUCP = 8,     /* UUCP子系统 */
    SYSLOG_FACILITY_CRON = 9,     /* 时钟守护程序 */
    SYSLOG_FACILITY_AUTHPRIV = 10,/* 安全/认证私有消息 */
    SYSLOG_FACILITY_FTP = 11,     /* FTP守护进程 */
    SYSLOG_FACILITY_LOCAL0 = 16,  /* 本地使用0 - 留给本地应用自定义使用 */
    SYSLOG_FACILITY_LOCAL1 = 17,  /* 本地使用1 - 留给本地应用自定义使用 */
    SYSLOG_FACILITY_LOCAL2 = 18,  /* 本地使用2 - 留给本地应用自定义使用 */
    SYSLOG_FACILITY_LOCAL3 = 19,  /* 本地使用3 - 留给本地应用自定义使用 */
    SYSLOG_FACILITY_LOCAL4 = 20,  /* 本地使用4 - 留给本地应用自定义使用 */
    SYSLOG_FACILITY_LOCAL5 = 21,  /* 本地使用5 - 留给本地应用自定义使用 */
    SYSLOG_FACILITY_LOCAL6 = 22,  /* 本地使用6 - 留给本地应用自定义使用 */
    SYSLOG_FACILITY_LOCAL7 = 23   /* 本地使用7 - 留给本地应用自定义使用 */
} syslog_facility_t;

/**
 * syslog_severity_t
 * 日志Severity（严重性）定义，参见RFC5424标准。
 * Severity用于标识日志消息的重要级别，取值范围0-7。
 * 数字越小表示级别越高/越紧急，0为最高级别，7为最低级别。
 */
typedef enum 
{
    SYSLOG_SEVERITY_EMERG = 0,    /* 紧急：系统不可用 - 最高级别，如内核崩溃 */
    SYSLOG_SEVERITY_ALERT = 1,    /* 警报：必须立即采取措施 - 如系统数据库损坏 */
    SYSLOG_SEVERITY_CRIT = 2,     /* 严重：危急情况 - 如硬件故障、链接中断 */
    SYSLOG_SEVERITY_ERR = 3,      /* 错误：错误状况 - 如应用程序错误 */
    SYSLOG_SEVERITY_WARNING = 4,  /* 警告：可能的问题 - 如磁盘空间不足 */
    SYSLOG_SEVERITY_NOTICE = 5,   /* 注意：正常但重要的情况 - 如服务启动/停止 */
    SYSLOG_SEVERITY_INFO = 6,     /* 信息：提供信息的消息 - 如常规操作信息 */
    SYSLOG_SEVERITY_DEBUG = 7     /* 调试：调试级别信息 - 最低级别，详细程序调试信息 */
} syslog_severity_t;


/**
 *   SYSLOG日志参数
 *   msgid      - 消息ID
 */

// 系统事件
#define MSGID_SYS_START      "SYS_START"
#define MSGID_SYS_REBOOT     "SYS_REBOOT"
#define MSGID_SYS_ERROR      "SYS_ERROR"

// device事件

// driver事件

// 媒体中心事件


#ifdef __cplusplus
}
#endif

#endif // SYSLOG_FORMAT_H
