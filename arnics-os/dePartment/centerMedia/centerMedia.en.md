# Center Media

English | [中文](centerMedia.md)

## 1. Overview

The Media Center (`centerMedia`) is the "PR and Communications Department" of the `arnics-os` architecture. It is fully responsible for Human-Machine Interaction (HMI) between the system and the outside world, including screen displays (UI), indicator lights, audio broadcasts, and other multimedia feedback.

## 2. Responsibilities

- **UI Display Management**: Drives LCD/OLED screens, interfaces with graphics libraries like LVGL, and manages UI transitions and rendering.
- **Media Strategy Abstraction**: Supports different media presentation strategies, decoupling business logic from display details.

## 3. Core Files and Interfaces

- `media_process.h/c`: The main loop for rendering and output control in the Media Center.
- `entry_media_api.h/c`: APIs for other departments (like the Decision Layer) to request media playback or UI updates.
- `mediaStrategy/`: Specific implementations of media presentation strategies (e.g., `media_EventDrive.c`, `media_alwaysOn.c`).

### Key Interfaces

```c
// Main processing flow of the Media Center (handles rendering and media output)
extern void media_process(void);

// Receives media requests from other departments
extern bool readMediaReq(mediaMessage_t *msg, time_t wait);

// Sends media playback/display messages to the Media Center
extern void SendMsgToMediaCenter(mediaMessage_t *message);
```

## 4. Working Mechanism

1. **Message-Driven**: The Decision Layer does not manipulate screens or LEDs directly. When the business state changes, it calls `SendMsgToMediaCenter` to send abstract UI/Audio instructions.
2. **Strategy Matching**: Upon receiving the message, the Media Center determines the specific audio/visual effects or UI interface based on the currently configured `mediaStrategy`.
3. **Asynchronous Rendering**: UI rendering and audio playback are executed asynchronously within `media_process`, ensuring the business flow is never blocked.

