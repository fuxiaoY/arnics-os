# uflog — Lightweight Embedded Logging Library
[中文文档](README.md) | English

A lightweight, portable, configurable embedded logging component suitable for MCU firmware projects.
This library draws inspiration from the Unix device/logging model, providing hierarchical (priority), optional prefixes (facility, timestamp, function, etc.), hexadecimal data output, and optional log caching (auto store) functionality.

This documentation is based on the library source code (`uflog.h`, `uflog.c`, `uflogCfg.h`) and includes a quick start guide, API reference, customization and porting guidelines, as well as common examples, targeting embedded firmware engineers and BSP porters.

---

## Table of Contents

- Overview
- Features
- Quick Start
- API Reference
- Control Flags and Log Levels
- Configuration Options (Compile-time)
- Porting and Port Implementation
- Examples
- Debugging and Common Issues
- License

---

## Overview

uflog is a logging component designed for resource-constrained platforms:

- Compact (can be compiled in static memory mode)
- Easily portable (provides weak symbols/port interfaces for implementation on target platforms)
- Practical functionality: supports log levels, optional prefixes, function/file/line information, hexadecimal data display, and automatic storage caching

Its typical output format is:

[PRI] [FACILITY] TIMESTAMP VERSION> [KIND] message (function|file|line) .

Example:

[INF] [temp] 2025-09-26 12:00:00 1> [temp] Device started (main|main.c|123) .

---

## Features

- Log levels: DBG/INF/WAR/ERR/ALT/NOT/ALW
- Optional prefixes: facility, timestamp, function/file/line, kind
- Supports printing hexadecimal data blocks (optional)
- Optional automatic caching (UFLOG_AUTO_STORE) and storage via `log_auto_store` callback
- Customizable output function `log_printf` (default weak symbol implementation uses printf)
- Supports static memory mode (`UFLOG_STATIC_MEMORY`) or dynamic allocation (mapped to project's malloc/free)

---

## Quick Start

1. Include the header file in your project and implement the platform port (see "Porting and Port Implementation"):

```c
#include "uflog.h"

// During startup initialization
uflog_init(UFLOG_LEVEL_ALL, UFLOG_SHOW_FULL | UFLOG_AUTO_STORE);
```

2. Use wrapper macros to print logs:

```c
UFLOG_INF("System startup complete: version=%d", version);
UFLOG_DBG("Debug information: val=%d", val);
UFLOG_ERR("Error occurred: code=%d", err);
```

3. Optionally print hexadecimal data:

```c
uint8_t payload[] = {0x01,0x02,0xFF};
xUFLOG_INF("mdia", "rx", WITH_HEX(payload, sizeof(payload)), "Received data");
// No hex to print, just change facility and kind fields
xUFLOG_INF("mdia", "rx", NO_HEX, "Received data");
```
4. force save log：
```c
sUFLOG_INF("mdia", "rx", NO_HEX, "Received data");
```
5. Release resources when finished (if using dynamic memory mode):

```c
uflog_close();
```

---

## API Reference (Summary)
```
/**
 * @brief Initialize the UFLOG system
 *
 * This function is used to initialize the UFLOG log system, setting the initial
 * log level and control options. This function must be called before using any
 * other UFLOG functions.
 *
 * @param uflog_level_in Initial log level
 *        - UFLOG_PRI_DBG: Debug level
 *        - UFLOG_PRI_INF: Information level
 *        - UFLOG_PRI_WAR: Warning level
 *        - UFLOG_PRI_ERR: Error level
 *        - UFLOG_PRI_ALT: Critical error level
 *        - UFLOG_PRI_NOT: Notice level
 *        - UFLOG_PRI_ALW: Always output level
 *
 * @param control Control options, can be a combination of the following values
 *        - uflog_control_e
 */
extern void uflog_init(uflog_pri_e uflog_level_in, int control);
```
```
/**
 * @brief Change UFLOG log level
 *
 * This function is used to dynamically change the log output level of the UFLOG system.
 * Only logs with a level equal to or higher than the set level will be output.
 *
 * @param uflog_level The log level to be set
 *        - UFLOG_PRI_DBG: Debug level
 *        - UFLOG_PRI_INF: Information level
 *        - UFLOG_PRI_WAR: Warning level
 *        - UFLOG_PRI_ERR: Error level
 *        - UFLOG_PRI_ALT: Critical error level
 *        - UFLOG_PRI_NOT: Notice level
 *        - UFLOG_PRI_ALW: Always output level
 */
extern void uflog_level_change(uflog_pri_e uflog_level);
```
```
/**
 * @brief Configure the UFLOG system
 *
 * This function is used to change the control options of the UFLOG system at runtime
 * without affecting the log level.
 *
 * @param control Control options, can be a combination of the following values
 *        - uflog_control_e

 */
extern void uflog_config(int control);
```
```
/**
 * @brief Close the UFLOG system
 *
 * This function is used to properly close the UFLOG system and release all allocated resources.
 * This function should be called before the program ends to ensure all logs are properly
 * processed and released.
 */
extern void uflog_close(void);
```
---

## Control Flags and Log Levels

### Log Levels (uflog_pri_e)

- UFLOG_PRI_DBG: Debug
- UFLOG_PRI_INF: Information
- UFLOG_PRI_WAR: Warning
- UFLOG_PRI_ERR: Error
- UFLOG_PRI_ALT: Critical Error
- UFLOG_PRI_NOT: Notice
- UFLOG_PRI_ALW: Always Output

Predefined combinations:
- UFLOG_LEVEL_OFF / UFLOG_LEVEL_MASTER / UFLOG_LEVEL_ALL / UFLOG_LEVEL_FAIL / UFLOG_LEVEL_NORMAL

### Control Flags (uflog_control_e)

- UFLOG_SHOW_BASIC: Basic format
- UFLOG_SHOW_FUNTION: Show function/file/line
- UFLOG_SHOW_FACILITY: Show facility
- UFLOG_SHOW_TIMESTAMP: Show timestamp
- UFLOG_SHOW_KIND: Show kind
- UFLOG_AUTO_STORE: Automatic storage (when enabled, the library writes content to the instance cache and calls `log_auto_store` at the end of each log)

Common combinations:
- UFLOG_SHOW_DEFAULT = UFLOG_SHOW_BASIC | UFLOG_SHOW_FUNTION
- UFLOG_SHOW_FULL = UFLOG_SHOW_BASIC | UFLOG_SHOW_FUNTION | UFLOG_SHOW_FACILITY | UFLOG_SHOW_TIMESTAMP | UFLOG_SHOW_KIND

---

## Configuration Options (Compile-time)

The file `uflogCfg.h` defines several key constants and switches:

- UFLOG_STATIC_MEMORY: If defined, the library uses a static global instance `uflog_default` and does not call the project's malloc/free (suitable for no-heap or dynamic allocation forbidden scenarios).
- UFLOG_USER_MSG_MAX_LEN: Maximum length of user message buffer (default 512)
- UFLOG_STORE_CACHE: Automatic storage cache size (default 256)
- UFLOG_MAX_PREFIX_LEN / UFLOG_MSG_STRUCT_LEN / UFLOG_FUN_LINE_LEN: Buffer length configurations for prefixes, message structures, function lines, etc.

If `UFLOG_STATIC_MEMORY` is not defined, the library will map `uflog_malloc` / `uflog_free` to memory management functions provided by the project (`arnicsMalloc`/`arnicsFree` in this repository's example).

---

## Porting and Port Implementation

The library provides several weak symbol default implementations in `uflog.c` (can be overridden by users):

- __weak void uflog_get_timestamp(char *timestamp, int timestamp_len)
  - Used to generate timestamp strings. Example implementation calls RTC:

```c
void uflog_get_timestamp(char *timestamp, int timestamp_len)
{
    rtcTimeDateTypeDef_t DT;
    dev_ctl(&rtc_ds, RTC_GETDATETIME, &DT);
    snprintf(timestamp, timestamp_len, "%02d-%02d-%02d %02d:%02d:%02d", DT.Year, DT.Month, DT.Date, DT.Hour, DT.Minute, DT.Second);
}
```

- __weak void log_printf(char *out_str)
  - The library's default implementation calls `printf("%s", out_str)`. On bare-metal or different terminals, implement as writing to UART or other output interfaces.

- __weak void log_auto_store(char *str)
  - When `UFLOG_AUTO_STORE` is enabled, the library passes the cache to this callback. Users can save the cache to persistent media or a circular buffer.

Porting guidelines:

1. If the target has no heap or dynamic allocation is not desired, define `UFLOG_STATIC_MEMORY`.
2. Implement `uflog_get_timestamp`, `log_printf` (output), and `log_auto_store` (optional) according to the platform.
3. If not using static memory, provide `arnicsMalloc`/`arnicsFree` or map `uflog_malloc`/`uflog_free` to the project's malloc/free in `uflogCfg.h`.
4. Ensure buffer configurations like `UFLOG_USER_MSG_MAX_LEN` meet the target project's requirements to avoid stack/memory shortages.

---

## Examples

Complete minimal example:

```c
#include "uflog.h"

int main(void)
{
    // Initialize: enable all levels and show full prefixes, enable automatic storage
    uflog_init(UFLOG_LEVEL_ALL, UFLOG_SHOW_FULL | UFLOG_AUTO_STORE);

    UFLOG_INF("System initialization complete, version=%d", 1);

    uint8_t packet[] = {0x01,0x02,0x03};
    xUFLOG_DBG("net", "recv", WITH_HEX(packet, sizeof(packet)), "Received data");

    uflog_close();
    return 0;
}
```

If you need to customize the output function and create multiple instances:

```c
void my_printer(char *s) { /* write to UART */ }
uflog_t *mylog = uflog_create(UFLOG_SHOW_DEFAULT, my_printer);
/* Example: print an INFO log on the custom instance (NO_HEX expands to NULL, 0) */
uflog_log(mylog, UFLOG_PRI_INF, "sensor", "sample", NO_HEX, false, __FUNCTION__, __FILENAME__, __LINE__, "value=%d", value);
uflog_delete(mylog);
```

---

## Debugging and Common Issues

- Logs not outputting: Confirm that the level configuration in `uflog_init` includes the level to be printed, and that `uflog_default_p` is not NULL.
- Timestamp always shows 9999-01-01...: This indicates that `uflog_get_timestamp` has not been implemented on the platform (the library returns a placeholder time string by default).
- Automatic storage not working: Implement `log_auto_store` and ensure the `UFLOG_AUTO_STORE` flag has been passed to `uflog_create` or `uflog_init`.
- Memory errors: Check if `UFLOG_STATIC_MEMORY` is enabled or if `uflog_malloc`/`uflog_free` are correctly mapped.

---

## Contributions and Extensions

Issues and PRs are welcome:

- Add more platform port examples (such as STM32 HAL UART output)
- Add support for different time sources/timezones for timestamps
- Provide a ring buffer-based implementation of log_auto_store

---

## License

Please see the `LICENSE` file at the top level of the repository.
