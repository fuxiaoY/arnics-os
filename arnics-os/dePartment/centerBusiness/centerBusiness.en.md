# Decision Layer (Center Business)

English | [中文](centerBusiness.md)

## 1. Overview

The Decision Layer (`centerBusiness`) acts as the "Decision Execution Department" in the `arnics-os` bionic social architecture. It is dedicated to processing high-level decision making and business logic specific to the product. By isolating decision logic from low-level drivers and system event distribution, the Decision Layer ensures high cohesion and low coupling in the codebase.

## 2. Responsibilities

In the "corporate society" model, the Decision Layer is akin to a company's core decision-making team:
- **Hardware Agnostic**: Does not interact with hardware peripherals directly. All hardware interactions are handled through `DataPlat` or `Drivers`.
- **Workflow Orchestration**: Receives task assignments from the `centerEvent` (Event Center) and orchestrates specific decision and business workflows.
- **State Maintenance**: Updates system states according to decision workflows and dispatches instructions to `centerMedia` (Media Center) for UI or audio feedback.

## 3. Core Files and Interfaces

- `business_process.h/c`: The main scheduling and workflow entry point for the Decision Layer.
- `entry_business_api.h/c`: APIs provided by the Decision Layer to other departments.

### Key Interfaces

```c
// Main processing flow of the Decision Layer (usually called in a loop within an RTOS task or timer-driven)
extern void business_process(void);

// Initialization of the Decision Layer
extern void business_init(void);
```

## 4. Best Practices

When developing new decision-making features:
1. **Modularize Logic**: Break down complex logic into multiple sub-modules and schedule them uniformly in `business_process`.
2. **Event-Driven**: Decision modules should not use infinite loops to wait (unless in independent RTOS tasks). They should rely on state machines or event-driven models.
3. **Data Isolation**: Persistent data or configurations generated during decision flows should be managed by `DataPlat`.