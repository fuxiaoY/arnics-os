# 系统例程与任务注册 (Routine)

[English](routine.en.md) | 中文

## 1. 概述

`routine` 目录负责 `arnics-os` 核心模块的具体注册与任务清单管理。它充当了系统架构与底层能力的桥梁，将各个零散的初始化函数和任务，整合为可以被 `arnicsCore` 内核直接调用的执行表。

## 2. 核心文件与职责

- **`arnicsTaskManager.c/h`**：集中管理所有的初始化任务列表（Task List）。

### 任务列表分类

1. **`initTaskList`**：系统启动早期的底层初始化任务。
2. **`preloadTaskList`**：系统加载阶段的预处理任务。
3. **`deviceTaskList`**：外设与驱动的初始化任务。

## 3. 工作机制

系统在启动时，`arnicsCore` 会根据 `routine` 中的 X-macro 注册表 (`ARNICS_GLOBAL_REGISTRY`)，生成初始化函数表并通过 `ARNICS_KERNEL_DECLARE_AND_BUILD_TABLE` 构建内核句柄。初始化分为三个阶段：

1. **外设初始化 (peripheralInit)**：执行 `initTaskList` 中的驱动初始化函数
2. **预加载 (preLoadInit)**：执行 `preloadTaskList` 中的参数加载函数
3. **外挂设备初始化 (deviceInit)**：执行 `deviceTaskList` 中的第三方库初始化函数

```c
// X-macro 全局注册表 — 自动生成内核句柄 arnics_init
#define ARNICS_GLOBAL_REGISTRY(X)                                     \
    X("peripheralInit", peripheralInit, INIT_TAG, 1)                  \
    X("preLoadInit",       preLoadInit, INIT_TAG, 1)                  \
    X("deviceInit",         deviceInit, INIT_TAG, 1)

ARNICS_KERNEL_DECLARE_AND_BUILD_TABLE(arnics_init, ARNICS_GLOBAL_REGISTRY);

// 任务清单示例
const tTaskFunc initTaskList[] = {
    TASK_FUNC(driver_init),
};
const tTaskFunc preloadTaskList[] = {
    TASK_FUNC(param_init),
};
const tTaskFunc deviceTaskList[] = {
    TASK_FUNC(third_init),
};
```

开发者在新增外设或中间件时，只需在对应的清单中追加 `TASK_FUNC()` 条目即可。
