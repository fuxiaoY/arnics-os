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

系统在启动时，`arnicsCore` 会根据 `routine` 中定义的表单，逐个执行初始化函数。这种设计避免了 `main` 函数中出现冗长、混乱的初始化调用链。

```c
// 示例：在 routine 中统一注册外设初始化
const tTaskFunc deviceTaskList[] = {
    driver_init,   // 驱动框架初始化
    param_init,    // 参数存储初始化
    third_init,    // 第三方库初始化
    NULL           // 列表结束标志
};
```

开发者在新增外设或中间件时，只需在对应的清单中追加条目即可。
