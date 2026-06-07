# 移植与适配层 (Porting Layer)

[English](port.en.md) | 中文

## 1. 概述

`port` 目录是 `arnics-os` 的底层适配层之一，主要负责与特定硬件平台、C 库基础功能相关的适配工作。通过在此目录下进行移植，您可以将系统轻松运行在不同的硬件平台上。

## 2. 主要职责

- **动态内存分配适配 (`arnicsMalloc.h`)**：系统所有对 `malloc` 和 `free` 的调用都被封装在这里。如果是裸机或资源受限的平台，您可以替换为特定的内存池管理（如 FreeRTOS 的 `pvPortMalloc` 或 TLSF）。
- **持久化存储对接 (`arnicsStore.c`)**：对接底层的 Flash 操作接口。`DataPlat` 进行参数和配置存储时，会调用此处的物理读写能力。
- **MCU 平台移植 (`portMcu/arnicsMcu.c/h`)**：MCU 特定的底层适配实现。
- **系统时间与基础类型 (`arnicsPort.h`)**：定义时间戳获取函数（如 `HAL_GetTick()` 或 Linux 的 `gettimeofday`）以及平台相关的特殊宏。
- **统一包含入口 (`portInclude.h`)**：port 层的一体化包含头文件。

## 3. 移植指南

在将 `arnics-os` 移植到新的 MCU 或系统时，您需要：
1. 实现 `arnicsMalloc.h` 中定义的内存分配接口。
2. 在 `arnicsStore.c` 中对接您的 Flash 或 EEPROM 驱动。
3. 确保 `arnicsPort.h` 中的系统时钟 Tick 函数能够正常返回毫秒级时间戳。
