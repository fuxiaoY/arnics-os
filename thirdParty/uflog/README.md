# uflog — 轻量级嵌入式日志库

中文 | [English Documentation](README.en.md)

轻量、可移植、配置化的嵌入式日志组件，适用于单片机固件项目。
该库借鉴类 Unix 设备/日志模型，提供分级（priority）、可选前缀（facility、timestamp、function 等）、十六进制数据输出与可选的日志缓存（auto store）功能。

本说明基于库源码（`uflog.h`、`uflog.c`、`uflogCfg.h`、`uflogPort.c`）编写，包含快速上手、API 参考、定制与移植要点以及常见示例，面向嵌入式固件工程师与 BSP 移植者。

---

## 目录

- 概览
- 特性
- 快速上手
- API 参考
- 控制标志（control flags）与日志级别
- 配置项（编译时）
- 移植与端口（port）实现
- 示例
- 调试与常见问题
- 许可

---

## 概览

uflog 是一个针对资源受限平台设计的日志组件：

- 小巧（可编译为静态内存模式）
- 易移植（提供弱符号/port 接口用于在目标平台实现）
- 功能实用：支持日志等级、可选前缀、函数/文件/行信息、十六进制数据展示与自动存储缓存

它的典型输出格式为：

[PRI] [FACILITY] TIMESTAMP VERSION> [KIND] message (function|file|line) .

示例：

[INF] [temp] 2025-09-26 12:00:00 1> [user] Device started (main|main.c|123) .

---

## 特性

- 日志级别：DBG/INF/WAR/ERR/ALT/NOT/ALW
- 可选前缀：facility、timestamp、function/file/line、kind
- 支持打印十六进制数据块（可选）
- 可选自动缓存（UFLOG_AUTO_STORE）并通过 `log_auto_store` 回调存储
- 可自定义输出函数 `log_printf`（默认弱符号实现会用 printf）
- 支持静态内存模式（`UFLOG_STATIC_MEMORY`）或动态分配（映射到项目的 malloc/free）

---

## 快速上手

1. 在工程中包含头文件并实现 platform port（见“移植与端口”）：

```c
#include "uflog.h"

// 在启动初始化
uflog_init(UFLOG_LEVEL_ALL, UFLOG_SHOW_FULL | UFLOG_AUTO_STORE);
```

2. 使用封装宏打印日志：

```c
UFLOG_INF("系统启动完成: version=%d", version);
UFLOG_DBG("调试信息: val=%d", val);
UFLOG_ERR("出错: code=%d", err);
```

3. 可选择打印十六进制数据：

```c
uint8_t payload[] = {0x01,0x02,0xFF};
xUFLOG_INF("net", "rx", WITH_HEX(payload, sizeof(payload)), "收到数据");
// 无hex需要打印，只需要改变facility和kind字段
xUFLOG_INF("net", "rx", NO_HEX, "收到数据");
```

4. 结束时释放资源（如果使用动态内存模式）：

```c
uflog_close();
```

---

## API 参考（摘要）
```
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
```
```
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
```
```
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
```
```
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
```
---

## 控制标志（control flags）与日志级别

### 日志级别（uflog_pri_e）

- UFLOG_PRI_DBG: 调试
- UFLOG_PRI_INF: 信息
- UFLOG_PRI_WAR: 警告
- UFLOG_PRI_ERR: 错误
- UFLOG_PRI_ALT: 严重错误
- UFLOG_PRI_NOT: 通知
- UFLOG_PRI_ALW: 总是输出

预定义组合：
- UFLOG_LEVEL_OFF / UFLOG_LEVEL_MASTER / UFLOG_LEVEL_ALL / UFLOG_LEVEL_FAIL / UFLOG_LEVEL_NORMAL

### 控制标志（uflog_control_e）

- UFLOG_SHOW_BASIC: 基本格式
- UFLOG_SHOW_FUNTION: 显示函数/文件/行
- UFLOG_SHOW_FACILITY: 显示 facility
- UFLOG_SHOW_TIMESTAMP: 显示时间戳
- UFLOG_SHOW_KIND: 显示 kind
- UFLOG_AUTO_STORE: 自动存储（启用后库会把内容写入实例缓存，并在每次日志结束时调用 `log_auto_store`）

常用组合：
- UFLOG_SHOW_DEFAULT = UFLOG_SHOW_BASIC | UFLOG_SHOW_FUNTION
- UFLOG_SHOW_FULL = UFLOG_SHOW_BASIC | UFLOG_SHOW_FUNTION | UFLOG_SHOW_FACILITY | UFLOG_SHOW_TIMESTAMP | UFLOG_SHOW_KIND

---

## 配置项（编译时）

文件 `uflogCfg.h` 定义了若干关键常量与开关：

- UFLOG_STATIC_MEMORY: 若定义，库使用静态全局实例 `uflog_default`，不会调用项目的 malloc/free（适合无堆或禁止动态分配场景）。
- UFLOG_USER_MSG_MAX_LEN: 用户消息缓冲区最大长度（默认 512）
- UFLOG_STORE_CACHE: 自动存储缓存大小（默认 256）
- UFLOG_MAX_PREFIX_LEN / UFLOG_MSG_STRUCT_LEN / UFLOG_FUN_LINE_LEN: 前缀、消息结构、函数行等各类缓冲长度配置

如果不定义 `UFLOG_STATIC_MEMORY`，库会将 `uflog_malloc` / `uflog_free` 映射到项目提供的内存管理函数（`arnicsMalloc`/`arnicsFree` 在该仓库示例中）。

---

## 移植与端口（Port）

库在 `uflogPort.c` 提供了几个弱符号实现（可被用户 override）：

- __weak void uflog_get_timestamp(char *timestamp, int timestamp_len)
  - 用于生成时间戳字符串。示例实现调用 RTC：

```c
void uflog_get_timestamp(char *timestamp, int timestamp_len)
{
    rtcTimeDateTypeDef_t DT;
    dev_ctl(&rtc_ds, RTC_GETDATETIME, &DT);
    snprintf(timestamp, timestamp_len, "%02d-%02d-%02d %02d:%02d:%02d", DT.Year, DT.Month, DT.Date, DT.Hour, DT.Minute, DT.Second);
}
```

- __weak void log_printf(char *out_str)
  - 库默认实现会调用 `printf("%s", out_str)`。在裸机或不同终端时请实现为写串口或其他输出接口。

- __weak void log_auto_store(char *str)
  - 当开启 `UFLOG_AUTO_STORE` 时，库会把缓存传递给该回调。用户可以把缓存保存到持久介质或循环 buffer。

移植要点：

1. 如果目标无堆或不希望动态分配，定义 `UFLOG_STATIC_MEMORY`。
2. 根据平台实现 `uflog_get_timestamp`、`log_printf`（输出）和 `log_auto_store`（可选）。
3. 如果未使用静态内存，提供 `arnicsMalloc`/`arnicsFree` 或在 `uflogCfg.h` 中把 `uflog_malloc`/`uflog_free` 映射到项目的 malloc/free。
4. 确保 `UFLOG_USER_MSG_MAX_LEN` 等缓冲配置满足目标项目需求，避免栈/内存不足。

---

## 示例

完整的最小示例：

```c
#include "uflog.h"

int main(void)
{
    // 初始化：打开所有级别并显示完整前缀，启用自动存储
    uflog_init(UFLOG_LEVEL_ALL, UFLOG_SHOW_FULL | UFLOG_AUTO_STORE);

    UFLOG_INF("系统初始化完成, version=%d", 1);

    uint8_t packet[] = {0x01,0x02,0x03};
    xUFLOG_DBG("net", "recv", WITH_HEX(packet, sizeof(packet)), "收到数据");

    uflog_close();
    return 0;
}
```

若需要自定义输出函数并创建多个实例：

```c
void my_printer(char *s) { /* write to UART */ }
uflog_t *mylog = uflog_create(UFLOG_SHOW_DEFAULT, my_printer);
xUFLOG_INF_HELPER(mylog, "sensor", "sample", NO_HEX, "value=%d", value);
uflog_delete(mylog);
```

---

## 调试与常见问题

- 日志不输出：确认 `uflog_init` 的级别配置包含要打印的级别，且 `uflog_default_p` 不为空。
- 时间戳总是 9999-01-01...：说明 `uflog_get_timestamp` 未被平台实现（库默认返回占位时间串）。
- 自动存储无效：实现 `log_auto_store` 并确保 `UFLOG_AUTO_STORE` 标志已传入 `uflog_create` 或 `uflog_init`。
- 内存错误：检查是否开启 `UFLOG_STATIC_MEMORY` 或正确映射 `uflog_malloc`/`uflog_free`。

---

## 贡献与扩展

欢迎提交 issue / PR：

- 补充更多平台的 port 示例（如 STM32 HAL 串口输出）
- 增加对不同时间源/时区的时间戳支持
- 提供基于环形缓冲区的 log_auto_store 实现

---

## 许可

请参见仓库顶层 `LICENSE` 文件。

