# ARNICS-OS/drivers Driver Framework

> A cross-platform unified driver abstraction layer, inspired by Unix-like device models, providing consistent device lifecycle and operation interfaces, aimed at reducing hardware coupling, improving portability efficiency, and system maintainability.

## Overview

The ARNICS-OS driver framework is an abstract driver operation platform built for microcontrollers, emulating Linux file operation style. This driver framework adopts a layered design, providing unified device operation interfaces, achieving decoupling of hardware abstraction and device management, enabling applications to access and control various hardware devices in a unified manner.

## Core Objectives

- **Pluggable**: Devices can be dynamically registered and managed
- **Extensible**: Supports multiple device types, easy to add new devices
- **Portable**: Layered design, hardware-related code concentrated in the BSP layer
- **User-friendly Interface**: Unified device operation API, similar to file operations

## Architecture Design

The driver framework adopts a layered architecture, mainly divided into the following four layers:

### 1. Application Layer

Applications access devices through unified API interfaces without needing to care about underlying hardware implementation details. Using standard device operation interfaces: open, close, read, write, control, and interrupt handling.

### 2. Interface Layer

The interface layer is located in the `component` directory, defining standard operation interfaces for each device type. Each device type has a corresponding interface file, such as `interface_uart.h`, `interface_gpio.h`, etc. These interface files define device operation function prototypes, such as open, close, read, write, control, and interrupt handling.
Device operation prototypes are adapted according to the device.

### 3. Driver Abstraction Layer

The driver abstraction layer is located in the `common` directory, providing core functionalities for device management, including:

- **Device Registration and Management**: Implemented through `drivers_list.c/h` for device registration, lookup, and management.
- **General Operation Interfaces**: Defines unified device operation interfaces through `drivers_common.c/h`, such as `dev_open()`, `dev_close()`, `dev_read()`, `dev_write()`, `dev_ctl()`, etc.
- **Parameter Mapping Mechanism**: Based on X-Macro to implement runtime access to device parameters by name, facilitating online configuration and diagnostics.

### 4. Board Support Package (BSP Layer)

The BSP layer is located in the `bsp` directory, responsible for interfacing with specific hardware platforms, implementing hardware-related operations. This layer includes:

- **Hardware Abstraction Definitions**: Defines hardware structures and parameter mapping tables for various devices through `bsp_def.h`.
- **Device Instance Management**: Manages device instances through `drivers_var.c/h`.

## API Interfaces

### Core APIs

- `bool dev_reg(const char *name, device_t *dev)` — Register a device, returns true/false for success/failure.
- `void *dev_get(int index)` — Get device instance by device index, returns NULL on failure.
- `int dev_open(device_t *dev)` — Open device.
- `int dev_close(device_t *dev)` — Close device.
- `int dev_read(device_t *dev, void *buf, size_t count)` — Read operation.
- `int dev_write(device_t *dev, const void *buf, size_t count)` — Write operation.
- `int dev_ctl(device_t *dev, int cmd, ...)` — Device control with customizable parameters.
- `void dev_irq(device_t *dev)` — Device interrupt handling.

### Parameter Mapping APIs

- `int dev_params_set(device_t *device, const param_value_t *params, size_t param_count)` — Set device parameters.
- `DEV_PARAMS_SET(dev, ...)` — Macro function, simplifying parameter setting operations.

### Error Handling

All interfaces typically return >=0 for success (data length or status), <0 indicates errors. Error codes are defined in relevant header files.

## Supported Device Types

- UART: Serial communication devices
- GPIO: General Purpose Input/Output
- SPI: Serial Peripheral Interface
- Flash: Internal Flash memory
- IWDG: Independent Watchdog
- RTC: Real-Time Clock
- ADC: Analog-to-Digital Converter
- IIC (Software): Software simulated I2C interface
- RNG: Random Number Generator
- SDMMC: SD/MMC memory card interface

## Usage Methods

### 1. Registering a Device

```c
// drivers_var.c: Define device instance
uart_t uart1 =
{
    .huart.Instance = USART1,
    .huart.Init.BaudRate = 115200,
    .huart.Init.WordLength = UART_WORDLENGTH_8B,
    .huart.Init.StopBits = UART_STOPBITS_1,
    .huart.Init.Parity = UART_PARITY_NONE,
    .huart.Init.Mode = UART_MODE_TX_RX,
    .huart.Init.HwFlowCtl = UART_HWCONTROL_NONE,
    .huart.Init.OverSampling = UART_OVERSAMPLING_16,
    .dma_mode = 0,
    .ring_rx = &debug_rbrecv,
    .ring_tx = &debug_rbsend,
    .rx_buf = debug_rxbuf,
    .tx_buf = debug_txbuf,
    .rx_buf_size = DEBUG_UART_RXBUF_SIZE,
    .tx_buf_size = DEBUG_UART_TXBUF_SIZE
};
// drivers_list.h: Append device mapping to DEVICE_MAP_X
#define DEVICE_MAP_X     ...     X("uart1", &uart1, "uart_t") 
// Register device
dev_reg("uart1", &dev_uart1);
```

### 2. Opening/Closing a Device

```c
// Open device
dev_open(&uart_ds);
// Use device...

// Close device
dev_close(&uart_ds);
```

### 3. Read/Write Operations

```c
// Write data
const char *msg = "Hello ARNICS-OS";
int bytes_written = dev_write(&uart_ds, msg, strlen(msg));

// Read data
char buffer[128];
int bytes_read = dev_read(&uart_ds, buffer, sizeof(buffer));
```

### 4. Control Commands

```c
// Other control commands
dev_ctl(&uart_ds, UART_WAKEUP_SET);
```

### 5. Parameter Mapping and Online Configuration

```c
// Use parameter mapping to set device parameters
uint32_t baud = 9600;
uint32_t parity = UART_PARITY_NONE;
DEV_PARAMS_SET(&uart_ds,
    {"BaudRate", &baud},
    {"Parity", &parity}
);
// Reopen device
dev_open(&uart_ds);
```

## Porting Guide

1. Define device structures and parameter mapping tables corresponding to the target MCU in `drivers/bsp/bsp_def.h`.
2. Create device instance mapping tables in `drivers/bsp/drivers_var.c/h` and register them at system startup.
3. Create driver abstraction layer lists in `drivers/common/drivers_list.c/h`.
4. Implement or adapt bridging code with MCU HAL in `component/src/interface_*.c`.
5. Verify step by step: reg -> open -> basic read/write -> ctl -> IRQ.

## Directory Structure

```
drivers/
├── bsp/                 # Board Support Package (platform-specific implementations and device mappings)
│   ├── bsp_def.h       # Hardware device structure and parameter mapping table definitions
│   ├── drivers_var.c    # Device instance management
│   └── drivers_var.h    # Device instance declarations
├── common/              # Driver abstraction layer (registration, lookup, general operations)
│   ├── drivers_common.c # General device operation implementations
│   ├── drivers_common.h # General device operation interface definitions
│   ├── drivers_list.c   # Device registration and lookup implementations
│   └── drivers_list.h   # Device registration and lookup interface definitions
├── component/           # Device interfaces (header files + reference implementations)
│   ├── inc/            # Interface header files
│   │   ├── interface_Include.h # Interface header file includes
│   │   ├── interface_uart.h    # UART interface
│   │   ├── interface_gpio.h    # GPIO interface
│   │   └── ...             # Other device interfaces
│   └── src/            # Interface implementations
│       ├── interface_uart.c    # UART interface implementation
│       ├── interface_gpio.c    # GPIO interface implementation
│       └── ...             # Other device interface implementations
└── driversInclude.h     # Unified include for driver framework
```
