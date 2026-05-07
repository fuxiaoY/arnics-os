# System Routines and Task Registration (Routine)

English | [中文](routine.md)

## 1. Overview

The `routine` directory is responsible for the specific registration and task list management of `arnics-os` core modules. It acts as a bridge between the system architecture and low-level capabilities, integrating scattered initialization functions and tasks into execution tables that can be directly invoked by the `arnicsCore` kernel.

## 2. Core Files and Responsibilities

- **`arnicsTaskManager.c/h`**: Centrally manages all initialization task lists.

### Task List Categories

1. **`initTaskList`**: Low-level initialization tasks executed early in the system startup.
2. **`preloadTaskList`**: Preprocessing tasks during the system loading phase.
3. **`deviceTaskList`**: Initialization tasks for peripherals and drivers.

## 3. Working Mechanism

During system startup, `arnicsCore` executes initialization functions one by one according to the tables defined in `routine`. This design avoids lengthy and messy initialization call chains in the `main` function.

```c
// Example: Unified registration of peripheral initialization in routine
const tTaskFunc deviceTaskList[] = {
    driver_init,   // Driver framework initialization
    param_init,    // Parameter storage initialization
    third_init,    // Third-party library initialization
    NULL           // End of list marker
};
```

When developers add new peripherals or middleware, they only need to append an entry to the corresponding list.
