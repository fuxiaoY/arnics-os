# 媒体中心 (Center Media)

[English](centerMedia.en.md) | 中文

## 1. 概述

媒体中心 (`centerMedia`) 是 `arnics-os` 架构中的“宣传与公关部门”。它全权负责系统与外部世界的人机交互（HMI），包括屏幕显示（UI）、指示灯状态、音频播报及其他多媒体反馈。

## 2. 职责定位

- **UI 显示管理**：驱动 LCD/OLED 屏幕，对接 LVGL 等图形库，管理界面跳转与刷新。
- **媒体策略抽象**：支持不同的媒体呈现策略，解耦业务逻辑与显示细节。

## 3. 核心文件与接口

- `media_process.h/c`：媒体中心的渲染与输出控制主循环。
- `entry_media_api.h/c`：供决策层等其他部门调用，用于请求播放媒体或刷新界面的接口。
- `mediaStrategy/`：媒体呈现策略的具体实现（如 `media_EventDrive.c`, `media_alwaysOn.c`）。

### 主要接口

```c
// 媒体中心主处理流程（处理渲染与媒体输出）
extern void media_process(void);

// 接收其他部门发来的媒体请求
extern bool readMediaReq(mediaMessage_t *msg, time_t wait);

// 向媒体中心发送媒体播放/显示消息
extern void SendMsgToMediaCenter(mediaMessage_t *message);
```

## 4. 工作机制

1. **消息驱动**：决策层不直接操作屏幕或 LED。当业务状态发生变化时，决策层调用 `SendMsgToMediaCenter` 发送抽象的 UI/音频指令。
2. **策略匹配**：媒体中心接收到消息后，根据当前配置的 `mediaStrategy`，决定具体的声光电效果或 UI 界面。
3. **异步渲染**：UI 的渲染和音频的播放在 `media_process` 中异步执行，绝不阻塞业务流程。
