# Center Guard

English | [中文](centerGuard.md)

## 1. Overview

The Guard Center (`centerGuard`) serves as the "Security and Protection Department" in the `arnics-os` architecture. Its core responsibility is to monitor the system's health and ensure safe recovery under abnormal conditions, similar to a company's security or compliance department.

## 2. Responsibilities

- **Watchdog Management**: Responsible for periodic feeding of hardware and software watchdogs. If critical tasks hang, the Guard Center stops feeding, triggering a system reset.
- **Exception Monitoring**: Monitors the operational status, memory usage, and CPU load of various departments (e.g., Decision Layer, Event Center, Media Center).
- **Error Recovery**: Attempts to restart relevant modules when non-fatal errors are detected; logs context information and safely reboots upon encountering fatal errors.
- **Logging & Audit**: Records critical security events and abnormal states through the logging system (`uflog`) for subsequent traceability.

## 3. Core Files and Interfaces

- `guard_process.h/c`: The monitoring and inspection entry point for the Guard Center.
- `entry_guard_api.h/c`: Registration and heartbeat APIs exposed by the Guard Center.

### Key Interfaces

```c
// Main processing flow of the Guard Center (periodically inspects system health)
extern void guard_process(void);
```

## 4. Best Practices

1. **Heartbeat Registration**: It is recommended that critical RTOS tasks register a heartbeat with the Guard Center.
2. **Hierarchical Processing**: Adopt different recovery strategies for different levels of anomalies (Warning -> Local Reset -> Global Reboot).
3. **Hardware Isolation**: Watchdog operations should be abstracted through `drivers/interface_iwdg` to maintain platform independence.
