# OS Abstraction Layer (OSAL / rtosInterface)

English | [中文](rtosInterface.md)

## 1. Overview

The `rtosInterface` directory serves as the **Operating System Abstraction Layer (OSAL)** for `arnics-os`. To ensure that code from various departments (Event, Decision, Media, Guard) can run seamlessly across different platforms like bare-metal, FreeRTOS, Linux, and Windows, the system encapsulates all OS-related operations into a unified API.

## 2. Key Features

### 2.1 Task Management
Provides unified interfaces for thread/task creation, suspension, resumption, and delay:
- `rtosTaskCreate()`
- `rtosThreadDelay()`
- `rtosTaskSuspendAll()` / `rtosTaskResumeAll()`

### 2.2 Department Communication Queues
In the "corporate society" model, departments communicate through "documents (messages)". `rtosInterface` provides exclusive cross-platform message queues and mutex interfaces for each center:
- **Administration (Ad)**: `rtosAdGetMsg()`, `rtosAdSendMsg()`
- **Event Center (Eventos)**: `rtosEventosGetMsg()`, `rtosEventosSendMsg()`
- **Media Center (Media)**: `rtosMediaGetMsg()`, `rtosMediaSendMsg()`

## 3. Platform Support

Currently, the system has built-in support for the following platforms:
- `freertos/`: Native encapsulation based on FreeRTOS.
- `linux/`: Encapsulation based on pthreads and POSIX message queues.
- `win/`: Encapsulation based on Windows APIs.

## 4. Porting Guide

If you need to introduce a new RTOS (such as RT-Thread or Zephyr), simply create a new directory under `rtosInterface` and implement all standard OS APIs defined in `rtosInterface.h`. No modifications to the upper-level code will be required.
