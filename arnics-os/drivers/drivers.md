# ARNICS-OS/drivers驱动框架

> 可跨平台的统一驱动抽象层，借鉴类 Unix 设备模型，提供一致的设备生命周期与操作接口，旨在降低硬件耦合、提升移植效率与系统可维护性。

## 概述

ARNICS-OS驱动框架是一个模拟Linux文件操作风格，为单片机构建的抽象驱动操作平台。该驱动框架采用分层设计，提供了统一的设备操作接口，实现了硬件抽象和设备管理的解耦，使应用程序能够以统一的方式访问和控制各种硬件设备。

## 核心目标

- 可插拔：设备可动态注册、管理
- 可扩展：支持多种设备类型，易于添加新设备
- 易移植：分层设计，硬件相关代码集中在BSP层
- 接口友好：统一的设备操作API，类似文件操作

## 架构设计

驱动框架采用分层架构，主要分为以下四层：

### 1. 应用层（Application Layer）

应用程序通过统一的API接口访问设备，无需关心底层硬件实现细节。使用标准的设备操作接口：打开、关闭、读、写、控制和中断处理。

### 2. 接口层（Interface Layer）

接口层位于`component`目录下，为每种设备类型定义标准操作接口。每种设备类型都有对应的接口文件，如`interface_uart.h`、`interface_gpio.h`等。这些接口文件定义了设备的操作函数原型，如打开、关闭、读、写、控制和中断处理。
设备操作原型根据设备适配。

### 3. 驱动抽象层（Driver Abstraction Layer）

驱动抽象层位于`common`目录下，提供了设备管理的核心功能，包括：

- **设备注册与管理**：通过`drivers_list.c/h`实现设备的注册、查找和管理。
- **通用操作接口**：通过`drivers_common.c/h`定义统一的设备操作接口，如`dev_open()`、`dev_close()`、`dev_read()`、`dev_write()`、`dev_ctl()`等。
- **参数映射机制**：基于X-Macro实现运行时按名称访问设备参数，便于在线配置与诊断。

### 4. 板级支持包（BSP Layer）

BSP层位于`bsp`目录下，负责与具体硬件平台对接，实现硬件相关的操作。该层包含：

- **硬件抽象定义**：通过`bsp_def.h`定义各种设备的硬件结构体和参数映射表。
- **设备实例管理**：通过`drivers_var.c/h`管理设备实例。

## API 接口

### 核心API

- `bool dev_reg(const char *name, device_t *dev)` — 注册设备，返回true/false表示成功/失败。
- `void *dev_get(int index)` — 通过设备索引获取设备实例，失败返回NULL。
- `int dev_open(device_t *dev)` — 打开设备。
- `int dev_close(device_t *dev)` — 关闭设备。
- `int dev_read(device_t *dev, void *buf, size_t count)` — 读取操作。
- `int dev_write(device_t *dev, const void *buf, size_t count)` — 写入操作。
- `int dev_ctl(device_t *dev, int cmd, ...)` — 设备控制，定制化传参。
- `void dev_irq(device_t *dev)` — 设备中断处理。

### 参数映射API

- `int dev_params_set(device_t *device, const param_value_t *params, size_t param_count)` — 设置设备参数。
- `DEV_PARAMS_SET(dev, ...)` — 宏函数，简化参数设置操作。

### 错误处理

所有接口通常返回 >=0 为成功（数据长度或状态），<0 表示错误。错误码定义在相关头文件中。

## 支持的设备类型

- UART：串口通信设备
- GPIO：通用输入输出
- SPI：串行外设接口
- Flash：内部Flash存储器
- IWDG：独立看门狗
- RTC：实时时钟
- ADC：模数转换器
- IIC（软件）：软件模拟I2C接口
- RNG：随机数生成器
- SDMMC：SD/MMC存储卡接口

## 使用方法

### 1. 注册设备

```c
// drivers_var.c：定义设备实例
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
// drivers_list.h：追加设备映射 DEVICE_MAP_X
#define DEVICE_MAP_X \
    ... \
    X("uart1", &uart1, "uart_t") \

// 注册设备
dev_reg("uart1", &dev_uart1);
```

### 2. 打开/关闭设备

```c
// 打开设备
dev_open(&uart_ds);
// 使用设备...

// 关闭设备
dev_close(&uart_ds);

```

### 3. 读写操作

```c
// 写入数据
const char *msg = "Hello ARNICS-OS";
int bytes_written = dev_write(&uart_ds, msg, strlen(msg));

// 读取数据
char buffer[128];
int bytes_read = dev_read(&uart_ds, buffer, sizeof(buffer));
```

### 4. 控制命令

```c
// 其他控制命令
dev_ctl(&uart_ds, UART_WAKEUP_SET);
```

### 5. 参数映射与在线配置

```c
// 使用参数映射设置设备参数
uint32_t baud = 9600;
uint32_t parity = UART_PARITY_NONE;
DEV_PARAMS_SET(&uart_ds, 
    {"BaudRate", &baud}, 
    {"Parity", &parity}
);
// 重新打开设备
dev_open(&uart_ds);
```

## 移植指南

1. 在`drivers/bsp/bsp_def.h`中定义与目标MCU对应的设备结构与参数映射表。
2. 在`drivers/bsp/drivers_var.c/h`中创建设备实例映射表并在系统启动时注册。
3. 在`drivers/common/drivers_list.c/h`中创建驱动抽象层列表。
4. 实现或适配`component/src/interface_*.c`中与MCU HAL的桥接代码。
5. 逐步验证：reg -> open -> basic read/write -> ctl -> IRQ。


## 目录结构

```
drivers/
├── bsp/                 # 板级支持包（平台相关实现与设备映射）
│   ├── bsp_def.h            # 硬件设备结构体和参数映射表定义
│   ├── drivers_var.c    # 设备实例管理
│   └── drivers_var.h    # 设备实例声明
├── common/              # 驱动抽象层（注册、查找、通用操作）
│   ├── drivers_common.c # 通用设备操作实现
│   ├── drivers_common.h # 通用设备操作接口定义
│   ├── drivers_list.c   # 设备注册与查找实现
│   └── drivers_list.h   # 设备注册与查找接口定义
├── component/            # 设备接口（头文件 + 参考实现）
│   ├── inc/            # 接口头文件
│   │   ├── interface_Include.h # 接口头文件包含
│   │   ├── interface_uart.h    # UART接口
│   │   ├── interface_gpio.h    # GPIO接口
│   │   └── ...             # 其他设备接口
│   └── src/            # 接口实现
│       ├── interface_uart.c    # UART接口实现
│       ├── interface_gpio.c    # GPIO接口实现
│       └── ...             # 其他设备接口实现
└── driversInclude.h     # 驱动框架统一包含
```
