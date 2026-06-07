# OS 抽象层 (OSAL / rtosInterface)

[English](rtosInterface.en.md) | 中文

## 1. 概述

`rtosInterface` 目录是 `arnics-os` 的**操作系统抽象层 (OSAL)**。为了保证各部门（Event, Decision, Media, Guard）的代码能够在裸机、FreeRTOS、Linux、Windows 等不同平台上无缝运行，系统将所有与 OS 相关的操作进行了统一封装。

## 2. 主要功能

### 2.1 任务管理
提供统一的线程/任务创建、挂起、恢复及延时接口：
- `arnics_task_init()`
- `rtosTaskCreate()`
- `rtosThreadDelay()`
- `rtosTaskSuspendAll()` / `rtosTaskResumeAll()`

### 2.2 函数指针 API 表 (`entry_rtos_api`)
所有跨模块 API 通过函数指针表暴露（`tRTOSEntry` 结构体，共 ~40 个函数指针）。`entry_rtos_api.h` 定义了 typedef、结构体和宏映射：
- `T_TYPEDEF(name)` — 生成函数指针类型
- `T_STRUCT_MEMBER(name)` — 结构体成员声明
- `RTOS_MICRODEF(name)` — 展开为 `(rtos_api->t_##name)`
- 实例 `entry_rtos_list` 为 const 全局变量，在 `entry_rtos_api.c` 中用 `INIT_MEMBER()` 宏初始化
- 上层通过宏间接调用（如 `#define rtosThreadDelay RTOS_MICRODEF(rtosThreadDelay)`），实现编译期多态

### 2.3 部门通信队列
“企业社会”模型中拥有独立队列的三个部门通过请求/响应队列对进行通信。每个部门提供约 10 个队列操作函数：
- **行政管理 (Ad)**：`rtosAdGetMsg()`, `rtosAdSendMsg()`, `rtosTakeMsgFromAd()`, `rtosDeliverMsgToAd()`, `TakeAdMsgQueueMutex()`, `ReleaseAdMsgQueueMutex()`, `CheckAdRspMesgNum()`, `CheckAdReqMesgNum()`, `PeekAdRspMesg()`, `CheckAdQueueSpacesAvailable()`
- **事件中心 (Eventos)**：`rtosEventosGetMsg()`, `rtosEventosSendMsg()`, `rtosTakeMsgFromEventos()`, `rtosDeliverMsgToEventos()`, `TakeEventosMsgQueueMutex()`, `ReleaseEventosMsgQueueMutex()`, `TakeEventosMutex()`, `ReleaseEventosMutex()`, `CheckEventRspMesgNum()`, `PeekEventRspMesg()`, `CheckEventQueueSpacesAvailable()`
- **媒体中心 (Media)**：`rtosMediaGetMsg()`, `rtosMediaSendMsg()`, `rtosTakeMsgFromMedia()`, `rtosDeliverMsgToMedia()`, `TakeMediaMutex()`, `ReleaseMediaMsgQueueMutex()`, `CheckMediaRspMesgNum()`, `CheckMediaReqMesgNum()`, `PeekMediaRspMesg()`, `CheckMediaQueueSpacesAvailable()`

> **注意**：决策层 (Business)、督察中心 (Guard)、控制台 (Console) 不拥有独立队列，它们通过事件中心或消息总线 (centerBus) 进行通信。

## 3. 平台支持

目前系统已内置以下平台的适配：
- `freertos/`：基于 FreeRTOS 的原生封装（480 行），CMSIS-OS 风格。
- `linux/`：基于 pthread 及 POSIX 条件变量的封装（696 行），systick 使用 `timerfd`。
- `win/`：基于 Windows API 的封装（705 行），systick 使用 `CreateWaitableTimer`。

## 4. 移植说明

如果需要引入新的 RTOS（如 RT-Thread 或 Zephyr），只需在 `rtosInterface` 下新建目录，并实现 `entry_rtos_api.h` 中 `tRTOSEntry` 结构体的所有函数指针接口即可，上层代码无需任何修改。
