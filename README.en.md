# arnics-os

```text
________________________________________________
    __      ____    _   _  ___   ____  ____  
   /  \    |  _ \  | \ | ||_ _| / ___|/ ___| 
  / /\ \   | |_) | |  \| | | | | |    \___ \ 
 / ____ \  |  _ <  | . ` | | | | |     ___) |
/_/    \_\ |_| \_\ |_|\__||___| \____||____/ 
________________________________________________
```

**Artificial Socialnics Program Operation System**

English Documentation | [中文文档](README.md)

## 1. Introduction

`arnics-os` is an innovative **Socialnics Model** embedded/cross-platform system architecture. It abandons traditional strongly-coupled task division and abstracts the system into a "corporate society". In this society, different functional modules are divided into distinct "Departments", collaborating through standardized "documents (messaging mechanisms)" and "functions (APIs)".

This architecture brings exceptional **modularity, pluggability, and maintainability**. Whether running on bare-metal, an RTOS (like FreeRTOS), or a high-level OS (Linux/Windows), it provides a consistent development experience.

![Bionic Social Model](arnics-os/document/picture/arnics.png)

## 2. Core Architecture Design

The architecture of `arnics-os` is divided into the following layers from bottom to top:

### 2.1 Driver Framework
- **Unix-like Device Model**: Provides unified `open`, `close`, `read`, `write`, `ctl` interfaces.
- **Hardware Decoupling**: Separation of the BSP layer from the driver abstraction layer makes application code completely hardware-independent.
- [Learn more about the Driver Framework](arnics-os/drivers/drivers.en.md)

### 2.2 Core Kernel (arnicsCore)
- **Table-Driven Execution Kernel**: The system is no longer flooded with `if/else` or `switch/case`. All capabilities (initialization, command distribution, module functions) are registered in a function table.
- **Dispatcher**: Accurately executes system capabilities based on `Department`, `Name`, and `Level`.
- [Learn more about the Core Kernel](arnics-os/core/arnicsCore.md)

### 2.3 OS Abstraction Layer (OSAL / rtosInterface)
- **Unified Interfaces**: Smooths out differences between operating systems (thread creation, delay, queues, mutexes).
- **Multi-Platform Support**: Built-in support for FreeRTOS, Linux, and Windows interfaces.

### 2.4 Data Platform
- **Public/Private Separation**: Unified parameters, configuration, and state management with data isolation between modules.
- **Persistent Storage**: Flash persistence mechanism with built-in CRC validation to ensure data security after power loss.
- [Learn more about the Data Platform](arnics-os/dataPlat/dataPlat.en.md)

### 2.5 Department Abstraction
System functions are materialized into seven core departments:
- **Event Center**: Acts as the scheduling hub, using an "employee model" (hired, internal, external employees) to allocate and process system events. [Details](arnics-os/dePartment/centerEvent/centerEvent.en.md)
- **Decision Layer (Center Business)**: Handles core decision logic, focusing on product-specific feature implementation. [Details](arnics-os/dePartment/centerBusiness/centerBusiness.en.md)
- **Media Center**: Responsible for all Human-Machine Interaction (HMI), UI display, audio prompts, and multimedia strategies. [Details](arnics-os/dePartment/centerMedia/centerMedia.en.md)
- **Guard Center**: Manages the system watchdog, exception monitoring, error recovery, and audit logs. [Details](arnics-os/dePartment/centerGuard/centerGuard.en.md)
- **Administrative Center**: Manages sleep/wake control, message routing, and cross-department coordination.
- **Console Center**: Provides CLI command-line interface for runtime debugging and system control.
- **Message Bus (Center Bus)**: Unified message bus with compile-time route table and O(1) dispatch, replacing hardcoded cross-department queue access.

## 3. Directory Structure

```text
arnics-os/
  ├── Inc/             # Global headers (projDefine.h, typedef.h, include.h)
  ├── core/            # Ultra-lightweight table-driven kernel (arnicsCore)
  ├── common/          # Common utilities (TaskTimer software timers, etc.)
  ├── dataPlat/        # Data management platform (parameters, state, persistence)
  ├── dePartment/      # Business departments (core of bionic architecture)
  │   ├── centerEvent/         # Event Center
  │   ├── centerBusiness/      # Decision Layer
  │   ├── centerMedia/         # Media Center
  │   ├── centerGuard/         # Guard Center
  │   ├── centerAdministrative/# Administrative Center
  │   ├── centerConsole/       # Console Center
  │   └── centerBus/           # Message Bus
  ├── drivers/         # Cross-platform driver framework (Unix style)
  ├── port/            # Memory and basic interface porting layer
  ├── routine/         # System task management and initialization list
  ├── rtosInterface/   # OS Abstraction Layer (FreeRTOS/Linux/Win)
  ├── thirdParty/      # Third-party components (uflog, cJSON, unity, etc.)
  └── document/        # Design documents and architecture diagrams
```

## 4. Quick Start
this project:
- Can be directly opened, compiled, and run using Visual Studio.
- Can run `build-arnics-os.sh` on Linux to generate and run the executable file.
- Can be embedded as an underlying system to run in a microcontroller project environment.

### 4.1 Environment Setup
- **Supported Platforms**: STM32 (all series), Windows, Linux
- **Compilers**: GCC, Keil MDK, IAR
- **Dependencies**: Ensure your platform has BSP support. Project templates for STM32 series are provided under `document/templete`.

### 4.2 System Startup Flow
System startup is extremely simple. Thanks to the table-driven kernel, you only need to call:

```c
#include "Inc/include.h"

int main()
{
  // 1. Start OS scheduling
  arnics_task_init();
  while (1)
    {
        rtosThreadDelay(1000);
    }
  return 0;
}
```

## 5. Contributing

Feel free to submit Issues and Pull Requests to help us improve this "corporate society"!
When adding a new module, please follow these rules:

## 6. License

This project is licensed under the Apache 2.0 License. For details, please see the [LICENSE](LICENSE) file.
