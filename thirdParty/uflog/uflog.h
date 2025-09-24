#ifndef UFLOG_H
#define UFLOG_H


#include "uflogCfg.h"

#ifdef __cplusplus
extern "C" {
#endif
/* define ----------------------------------------------------------*/

/** @brief UFLOG库版本号 / UFLOG library version number */
#define UFLOG_VERSION                               1

/** @brief UFLOG日志消息结束标记 / UFLOG log message end marker */
#define UFLOG_END                                   " .\r\n"

/**
 * UFLOG level
 * 
 */
typedef enum 
{
    UFLOG_PRI_DBG         = 0x01,
    UFLOG_PRI_INF         = 0x02,
    UFLOG_PRI_WAR         = 0x04,
    UFLOG_PRI_ERR         = 0x08,
    UFLOG_PRI_ALT         = 0x10,
    UFLOG_PRI_NOT         = 0x20,
    UFLOG_PRI_ALW         = 0x80,
   /*--------------------------------------------------------------*/
    UFLOG_LEVEL_OFF       = 0x00,                                     // 关闭所有日志 

    UFLOG_LEVEL_MASTER    = UFLOG_PRI_INF|UFLOG_PRI_ERR|UFLOG_PRI_ALW, // 简化版日志 

    UFLOG_LEVEL_ALL       = UFLOG_PRI_DBG|UFLOG_PRI_INF|UFLOG_PRI_WAR|
                            UFLOG_PRI_ERR|UFLOG_PRI_ALT|UFLOG_PRI_NOT|UFLOG_PRI_ALW,
    UFLOG_LEVEL_FAIL      = UFLOG_PRI_WAR|UFLOG_PRI_ERR|UFLOG_PRI_ALT|UFLOG_PRI_NOT,

    UFLOG_LEVEL_NORMAL    = UFLOG_PRI_INF|UFLOG_PRI_WAR|
                            UFLOG_PRI_ERR|UFLOG_PRI_ALT|UFLOG_PRI_NOT|UFLOG_PRI_ALW,

} uflog_pri_e;


/**
 *   UFLOG options
 *   [pri] [facility] timestamp version> [kind] msg (function|file|line) .
 */
typedef enum
{
    UFLOG_SHOW_BASIC       = 0x00000001,                              // 启用基本日志     [pri] - - version> msg . 
    UFLOG_SHOW_FUNTION     = 0x00000002,                              // 显示函数名       [pri] - - version> msg (function|file|line) . 
    UFLOG_SHOW_FACILITY    = 0x00000004,                              // 显示设备字       [pri] [facility] - version> msg .
    UFLOG_SHOW_TIMESTAMP   = 0x00000008,                              // 显示时间戳       [pri] - timestamp version> msg .
    UFLOG_SHOW_KIND        = 0x00000010,                              // 显示种类         [pri] - - version> [kind] msg .

   /*--------------------------------------------------------------*/
    UFLOG_AUTO_STORE       = 0x00000100,                              // 自动存储日志 

    /* 组合标志 */
    UFLOG_SHOW_DEFAULT    = UFLOG_SHOW_BASIC|UFLOG_SHOW_FUNTION,      // 默认打印 
    UFLOG_SHOW_FULL       = UFLOG_SHOW_BASIC|UFLOG_SHOW_FUNTION|
                            UFLOG_SHOW_FACILITY|UFLOG_SHOW_TIMESTAMP|UFLOG_SHOW_KIND, // 完整打印日志
} uflog_control_e;


typedef struct
{
    uint8_t *hex;
    size_t hex_len;
} uflog_hex_t;


#define UFLOG_HAS_FLAG(control,flag) ((control & flag) == flag)

// 运行时从 __FILE__ 去掉路径
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : \
                      (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__))

typedef struct _uflog_t uflog_t;

typedef void (*uflog_print)(char *out_str);

extern void uflog_level_change(uflog_pri_e uflog_level);

extern uflog_t* uflog_create(int control,uflog_print print);

extern void uflog_delete(uflog_t *uflog_p);

extern void uflog_control_config(uflog_t *uflog_p, int control);

extern void uflog_log(uflog_t *uflog_p,uflog_pri_e log_level, const char *facility, 
    const char *kind, uint8_t *data, size_t len, const char *func, const char *file, int line, const char *fmt, ...);

#define _UFLOG_SIMPLE(uflog_p,pri,facility,kind,data,len,fmt, ...) \
uflog_log(               \
    uflog_p,             \
    pri,                 \
    facility,            \
    kind,                \
    data,                \
    len,                 \
    __FUNCTION__,        \
    __FILENAME__,        \
    __LINE__,            \
    fmt,                 \
    ##__VA_ARGS__        \
)

#define WITH_HEX(data,len)  data,len
#define NO_HEX              NULL,0

#define xUFLOG_DBG_HELPER(uflog_p, facility, kind, data, len, fmt, ...)  _UFLOG_SIMPLE(uflog_p, UFLOG_PRI_DBG, facility, kind, data, len, fmt, ##__VA_ARGS__)
#define xUFLOG_INF_HELPER(uflog_p, facility, kind, data, len, fmt, ...)  _UFLOG_SIMPLE(uflog_p, UFLOG_PRI_INF, facility, kind, data, len, fmt, ##__VA_ARGS__)
#define xUFLOG_WAR_HELPER(uflog_p, facility, kind, data, len, fmt, ...)  _UFLOG_SIMPLE(uflog_p, UFLOG_PRI_WAR, facility, kind, data, len, fmt, ##__VA_ARGS__)
#define xUFLOG_ERR_HELPER(uflog_p, facility, kind, data, len, fmt, ...)  _UFLOG_SIMPLE(uflog_p, UFLOG_PRI_ERR, facility, kind, data, len, fmt, ##__VA_ARGS__)
#define xUFLOG_ALT_HELPER(uflog_p, facility, kind, data, len, fmt, ...)  _UFLOG_SIMPLE(uflog_p, UFLOG_PRI_ALT, facility, kind, data, len, fmt, ##__VA_ARGS__)
#define xUFLOG_NOT_HELPER(uflog_p, facility, kind, data, len, fmt, ...)  _UFLOG_SIMPLE(uflog_p, UFLOG_PRI_NOT, facility, kind, data, len, fmt, ##__VA_ARGS__)
#define xUFLOG_ALW_HELPER(uflog_p, facility, kind, data, len, fmt, ...)  _UFLOG_SIMPLE(uflog_p, UFLOG_PRI_ALW, facility, kind, data, len, fmt, ##__VA_ARGS__)

#define xUFLOG_DBG(facility, kind, hex_args, fmt, ...)                   xUFLOG_DBG_HELPER(uflog_default_p, facility, kind, hex_args, fmt, ##__VA_ARGS__)
#define xUFLOG_INF(facility, kind, hex_args, fmt, ...)                   xUFLOG_INF_HELPER(uflog_default_p, facility, kind, hex_args, fmt, ##__VA_ARGS__)
#define xUFLOG_WAR(facility, kind, hex_args, fmt, ...)                   xUFLOG_WAR_HELPER(uflog_default_p, facility, kind, hex_args, fmt, ##__VA_ARGS__)
#define xUFLOG_ERR(facility, kind, hex_args, fmt, ...)                   xUFLOG_ERR_HELPER(uflog_default_p, facility, kind, hex_args, fmt, ##__VA_ARGS__)
#define xUFLOG_ALT(facility, kind, hex_args, fmt, ...)                   xUFLOG_ALT_HELPER(uflog_default_p, facility, kind, hex_args, fmt, ##__VA_ARGS__)
#define xUFLOG_NOT(facility, kind, hex_args, fmt, ...)                   xUFLOG_NOT_HELPER(uflog_default_p, facility, kind, hex_args, fmt, ##__VA_ARGS__)
#define xUFLOG_ALW(facility, kind, hex_args, fmt, ...)                   xUFLOG_ALW_HELPER(uflog_default_p, facility, kind, hex_args, fmt, ##__VA_ARGS__)
    
#define UFLOG_DBG(fmt, ...)                                              xUFLOG_DBG("temp", "user", NO_HEX, fmt, ##__VA_ARGS__)
#define UFLOG_INF(fmt, ...)                                              xUFLOG_INF("temp", "user", NO_HEX, fmt, ##__VA_ARGS__)
#define UFLOG_WAR(fmt, ...)                                              xUFLOG_WAR("temp", "user", NO_HEX, fmt, ##__VA_ARGS__)
#define UFLOG_ERR(fmt, ...)                                              xUFLOG_ERR("temp", "user", NO_HEX, fmt, ##__VA_ARGS__)
#define UFLOG_ALT(fmt, ...)                                              xUFLOG_ALT("temp", "user", NO_HEX, fmt, ##__VA_ARGS__)
#define UFLOG_NOT(fmt, ...)                                              xUFLOG_NOT("temp", "user", NO_HEX, fmt, ##__VA_ARGS__)
#define UFLOG_ALW(fmt, ...)                                              xUFLOG_ALW("temp", "user", NO_HEX, fmt, ##__VA_ARGS__)
/*port --------------------------------------------------------------*/
extern void uflog_get_timestamp(char *timestamp, int timestamp_len);
extern void log_auto_store(char * str);
extern void log_printf(char * str);
extern uflog_t *uflog_default_p;
/* user --------------------------------------------------------------*/

/** 
 * @brief 初始化UFLOG系统 / Initialize the UFLOG system
 * 
 * 此函数用于初始化UFLOG日志系统，设置初始日志级别和控制选项。
 * 必须在使用任何其他UFLOG功能之前调用此函数。
 * 
 * This function is used to initialize the UFLOG log system, setting the initial 
 * log level and control options. This function must be called before using any 
 * other UFLOG functions.
 * 
 * @param uflog_level_in 初始日志级别    / Initial log level
 *        - UFLOG_PRI_DBG: 调试级别      / Debug level
 *        - UFLOG_PRI_INF: 信息级别      / Information level
 *        - UFLOG_PRI_WAR: 警告级别      / Warning level
 *        - UFLOG_PRI_ERR: 错误级别      / Error level
 *        - UFLOG_PRI_ALT: 严重错误级别  / Critical error level
 *        - UFLOG_PRI_NOT: 通知级别      / Notice level
 *        - UFLOG_PRI_ALW: 总是输出级别  / Always output level
 * 
 * @param control 控制选项，可以是以下值的组合        / Control options, can be a combination of the following values
 *        - uflog_control_e
 */
extern void uflog_init(uflog_pri_e uflog_level_in, int control);

/** 
 * @brief 更改UFLOG日志级别 / Change UFLOG log level
 * 
 * 此函数用于动态更改UFLOG系统的日志输出级别。只有等于或高于设置级别的日志才会被输出。
 * 
 * This function is used to dynamically change the log output level of the UFLOG system.
 * Only logs with a level equal to or higher than the set level will be output.
 * 
 * @param uflog_level 要设置的日志级别   / The log level to be set
 *        - UFLOG_PRI_DBG: 调试级别     / Debug level
 *        - UFLOG_PRI_INF: 信息级别     / Information level
 *        - UFLOG_PRI_WAR: 警告级别     / Warning level
 *        - UFLOG_PRI_ERR: 错误级别     / Error level
 *        - UFLOG_PRI_ALT: 严重错误级别 / Critical error level
 *        - UFLOG_PRI_NOT: 通知级别     / Notice level
 *        - UFLOG_PRI_ALW: 总是输出级别 / Always output level
 */
extern void uflog_level_change(uflog_pri_e uflog_level);

/** 
 * @brief 配置UFLOG系统 / Configure the UFLOG system
 * 
 * 此函数用于在运行时更改UFLOG系统的控制选项，而不影响日志级别。
 * 
 * This function is used to change the control options of the UFLOG system at runtime 
 * without affecting the log level.
 * 
 * @param control 控制选项，可以是以下值的组合 / Control options, can be a combination of the following values
 *        - uflog_control_e

 */
extern void uflog_config(int control);

/** 
 * @brief 关闭UFLOG系统 / Close the UFLOG system
 * 
 * 此函数用于正确关闭UFLOG系统，释放所有分配的资源。
 * 在程序结束前应调用此函数以确保所有日志都被正确处理和释放。
 * 
 * This function is used to properly close the UFLOG system and release all allocated resources.
 * This function should be called before the program ends to ensure all logs are properly 
 * processed and released.
 */
extern void uflog_close(void);

#ifdef __cplusplus  
}
#endif


#endif
