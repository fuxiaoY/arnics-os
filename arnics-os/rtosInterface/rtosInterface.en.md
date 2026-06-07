# OS Abstraction Layer (OSAL / rtosInterface)

English | [中文](rtosInterface.md)

## 1. Overview

The `rtosInterface` directory serves as the **Operating System Abstraction Layer (OSAL)** for `arnics-os`. To ensure that code from various departments (Event, Decision, Media, Guard) can run seamlessly across different platforms like bare-metal, FreeRTOS, Linux, and Windows, the system encapsulates all OS-related operations into a unified API.

## 2. Key Features

### 2.1 Task Management
Provides unified interfaces for thread/task creation, suspension, resumption, and delay:
- `arnics_task_init()`
- `rtosTaskCreate()`
- `rtosThreadDelay()`
- `rtosTaskSuspendAll()` / `rtosTaskResumeAll()`

### 2.2 Function Pointer API Table (`entry_rtos_api`)
All cross-module APIs are exposed through a function pointer table (`tRTOSEntry` struct, ~40 function pointers). `entry_rtos_api.h` defines the typedef, struct, and macro mapping:
- `T_TYPEDEF(name)` — generates a function pointer type
- `T_STRUCT_MEMBER(name)` — struct member declaration
- `RTOS_MICRODEF(name)` — expands to `(rtos_api->t_##name)`
- The instance `entry_rtos_list` is a const global, initialized with `INIT_MEMBER()` macros in `entry_rtos_api.c`
- Upper layers call through macros (e.g., `#define rtosThreadDelay RTOS_MICRODEF(rtosThreadDelay)`) to achieve compile-time polymorphism

### 2.3 Department Communication Queues
In the "corporate society" model, three departments with independent queues communicate through request/response queue pairs. Each department provides ~10 queue operation functions:
- **Administration (Ad)**: `rtosAdGetMsg()`, `rtosAdSendMsg()`, `rtosTakeMsgFromAd()`, `rtosDeliverMsgToAd()`, `TakeAdMsgQueueMutex()`, `ReleaseAdMsgQueueMutex()`, `CheckAdRspMesgNum()`, `CheckAdReqMesgNum()`, `PeekAdRspMesg()`, `CheckAdQueueSpacesAvailable()`
- **Event Center (Eventos)**: `rtosEventosGetMsg()`, `rtosEventosSendMsg()`, `rtosTakeMsgFromEventos()`, `rtosDeliverMsgToEventos()`, `TakeEventosMsgQueueMutex()`, `ReleaseEventosMsgQueueMutex()`, `TakeEventosMutex()`, `ReleaseEventosMutex()`, `CheckEventRspMesgNum()`, `PeekEventRspMesg()`, `CheckEventQueueSpacesAvailable()`
- **Media Center (Media)**: `rtosMediaGetMsg()`, `rtosMediaSendMsg()`, `rtosTakeMsgFromMedia()`, `rtosDeliverMsgToMedia()`, `TakeMediaMutex()`, `ReleaseMediaMsgQueueMutex()`, `CheckMediaRspMesgNum()`, `CheckMediaReqMesgNum()`, `PeekMediaRspMesg()`, `CheckMediaQueueSpacesAvailable()`

> **Note**: Decision Layer (Business), Guard Center, and Console do not own independent queues. They communicate through the Event Center or the Message Bus (centerBus).

## 3. Platform Support

Currently, the system has built-in support for the following platforms:
- `freertos/`: Native encapsulation based on FreeRTOS (480 lines), CMSIS-OS style.
- `linux/`: Encapsulation based on pthreads and POSIX condition variables (696 lines), systick via `timerfd`.
- `win/`: Encapsulation based on Windows APIs (705 lines), systick via `CreateWaitableTimer`.

## 4. Porting Guide

If you need to introduce a new RTOS (such as RT-Thread or Zephyr), simply create a new directory under `rtosInterface` and implement all function pointer interfaces from the `tRTOSEntry` struct defined in `entry_rtos_api.h`. No modifications to the upper-level code will be required.
