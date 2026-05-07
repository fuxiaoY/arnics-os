# Porting Layer

English | [中文](port.md)

## 1. Overview

The `port` directory serves as one of the low-level adaptation layers for `arnics-os`. It primarily handles adaptations related to specific hardware platforms and basic C library functions. By porting within this directory, you can easily run the system on different hardware platforms.

## 2. Key Responsibilities

- **Dynamic Memory Allocation Adaptation (`arnicsMalloc.h`)**: All system calls to `malloc` and `free` are encapsulated here. For bare-metal or resource-constrained platforms, you can replace them with specific memory pool management (e.g., FreeRTOS's `pvPortMalloc` or TLSF).
- **Persistent Storage Interface (`arnicsStore.c/h`)**: Interfaces with low-level Flash operation APIs. When `DataPlat` stores parameters and configurations, it relies on the physical read/write capabilities provided here.
- **System Time and Basic Types (`arnicsPort.h`)**: Defines timestamp retrieval functions (e.g., `HAL_GetTick()` or Linux's `gettimeofday`) and platform-specific macros.

## 3. Porting Guide

When porting `arnics-os` to a new MCU or operating system, you need to:
1. Implement the memory allocation interfaces defined in `arnicsMalloc.h`.
2. Connect your Flash or EEPROM drivers in `arnicsStore.c`.
3. Ensure that the system clock tick function in `arnicsPort.h` correctly returns a millisecond-level timestamp.
