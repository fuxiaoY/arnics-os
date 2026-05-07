# OS 抽象层 (OSAL / rtosInterface)

[English](rtosInterface.en.md) | 中文

## 1. 概述

`rtosInterface` 目录是 `arnics-os` 的**操作系统抽象层 (OSAL)**。为了保证各部门（Event, Decision, Media, Guard）的代码能够在裸机、FreeRTOS、Linux、Windows 等不同平台上无缝运行，系统将所有与 OS 相关的操作进行了统一封装。

## 2. 主要功能

### 2.1 任务管理
提供统一的线程/任务创建、挂起、恢复及延时接口：
- `rtosTaskCreate()`
- `rtosThreadDelay()`
- `rtosTaskSuspendAll()` / `rtosTaskResumeAll()`

### 2.2 部门通信队列
“企业社会”模型中的各部门通过“公文（消息）”进行通信。`rtosInterface` 为各个中心提供了专属的跨平台消息队列及互斥锁接口：
- **行政管理 (Ad)**：`rtosAdGetMsg()`, `rtosAdSendMsg()`
- **事件中心 (Eventos)**：`rtosEventosGetMsg()`, `rtosEventosSendMsg()`
- **媒体中心 (Media)**：`rtosMediaGetMsg()`, `rtosMediaSendMsg()`

## 3. 平台支持

目前系统已内置以下平台的适配：
- `freertos/`：基于 FreeRTOS 的原生封装。
- `linux/`：基于 pthread 及 POSIX 消息队列的封装。
- `win/`：基于 Windows API 的封装。

## 4. 移植说明

如果需要引入新的 RTOS（如 RT-Thread 或 Zephyr），只需在 `rtosInterface` 下新建目录，并实现 `rtosInterface.h` 中定义的所有标准 OS API 即可，上层代码无需任何修改。
