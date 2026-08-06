# arnics-os Unity 单元测试方案

## 1. 目标与范围

本方案为 arnics-os 建立可审计、可重复、可跨平台扩展的 C 单元测试体系。测试框架直接使用仓库已有的 Unity 2.6，不新增 GoogleTest、Catch2 或其他测试依赖。

首阶段目标：

- Windows + Visual Studio/MSVC 环境可独立配置、构建和执行 Unity 测试。
- 使用 CTest 统一编排测试，并输出 JUnit XML 结果。
- 优先覆盖不依赖真实硬件的核心基础设施：CRC16、环形缓冲、统一队列平台后端及兼容 API。
- 测试代码不进入产品 `arnics-os` 目标，避免影响固件和平台构建。
- 后续 Linux/ARM-Linux 和 FreeRTOS 复用同一批测试断言，仅替换平台后端或测试入口。

硬件驱动、BSP、真实 Flash/串口、FreeRTOS 调度和板级流程不纳入 Windows 主机单元测试的虚假覆盖范围，后续应通过目标板测试或 HIL 测试验证。

## 2. 框架与报告标准

| 领域 | 选型 | 说明 |
|---|---|---|
| C 单元测试 | 仓库内 Unity 2.6 | 适合纯 C、嵌入式、MSVC、GCC 和交叉编译 |
| 构建 | CMake | 与现有跨平台工程保持一致 |
| 测试编排 | CTest | 支持标签、失败重跑、并行执行和 CI 集成 |
| 结果报告 | CTest JUnit XML | 记录套件、用例、失败信息和耗时 |
| 覆盖率 | Windows 探测 OpenCppCoverage/LLVM；Linux 探测 gcovr/llvm-cov | 未安装工具时不得伪造百分比，报告必须标记 unavailable |
| 性能数据 | CTest 时间 + 可选独立 benchmark | 单元测试只记录稳定、低成本的执行耗时 |

报告至少应包含：提交版本、构建器、编译器、平台宏、测试数量、通过/失败数量、失败详情、总耗时、覆盖率工具和覆盖率范围。

## 3. 目录与构建

```text
tests/
├── CMakeLists.txt
├── test_main.c
├── test_crc16.c
├── test_ringbuffer.c
├── test_queue_win.c
├── run_tests.ps1
└── run_tests.sh
```

顶层 [CMakeLists.txt](../CMakeLists.txt) 使用 `ARNICS_BUILD_TESTS` 控制测试目标，默认关闭：

```powershell
cmake -S . -B build-test-vs -G "Visual Studio 17 2022" -A x64 -DARNICS_BUILD_TESTS=ON
cmake --build build-test-vs --config Debug --parallel 4 -- /v:minimal
ctest --test-dir build-test-vs -C Debug --output-on-failure --output-junit test-results.xml
```

测试目标使用仓库内：

- `thirdParty/unity/unity.c`
- `thirdParty/unity/unity.h`
- `thirdParty/AlgorithmLib/crc16.c`
- `thirdParty/ring/ringbuffer.c`
- `rtosInterface/win/queue_win.c`

## 4. 测试分层

### 4.1 算法单元测试

- CRC16 标准向量、二进制数据、空输入、分段计算一致性。
- 环形缓冲区初始化、非 2 次幂容量拒绝、满/空边界、回绕、清空、读写截断。

### 4.2 RTOS/OSAL 单元测试

- Windows queue backend 的创建、非法参数、FIFO、满队列、空队列、阻塞、超时、虚假唤醒和多线程压力测试。
- Linux queue backend 的同等语义，以及阻塞、超时、虚假唤醒和多线程压力测试。
- FreeRTOS backend 在目标工程中验证 `xQueue*` 映射，不在 Windows 主机伪造 FreeRTOS 内核行为。

### 4.3 事件中心测试

- 事件位图空状态和跨字边界状态。
- 事件标志和消息标志的置位、清零、全清零。
- 事件消息 ID、事件状态和消息状态字段一致性。
- 事件处理状态枚举值稳定性。
- 依赖消息队列、任务调度和日志的完整流程另列为集成测试，不在主机单元测试中伪造。

### 4.4 数据平台测试

- 数据对象指针获取和固定长度数据读取。
- 正常长度、短缓冲区、超长缓冲区和空边界行为。
- 数据写入、部分写入和超长写入失败路径。
- 数据认证/校验入口。
- 数据事件状态初始化、置位、查询和清除。
- Flash、硬件互斥和临界区通过平台桩隔离，真实介质读写另列为目标板测试。

### 4.5 兼容接口测试

- `compat_queue.c` 的部门请求/响应映射。
- Count、Peek、QueueSpacesAvailable 与 `queue_defs.h` 容量一致。
- 无效句柄、未初始化队列和非法参数返回失败，不崩溃。
- `entry_rtos_api` 函数指针表与兼容函数绑定一致。

### 4.4 核心业务和流程测试

- X-macro 表项数量和索引稳定性。
- 事件状态机的等待、执行、响应和异常状态转换。
- 数据对象默认值、边界长度和 CRC 校验。
- 业务模块使用 fake/mock 的平台 API 验证跨模块交互。

## 5. 用例设计规范

每个测试用例必须：

1. 只验证一个行为或一组紧密相关的不变量；
2. 明确 Arrange/Act/Assert；
3. 覆盖正常、边界、非法参数和失败路径；
4. 不依赖测试执行顺序；
5. 不依赖真实时间，除非测试的是超时；
6. 失败输出包含输入、期望值和实际值；
7. 测试后恢复全局状态和临时资源。

并发测试使用唯一消息序号和校验值检查丢失、重复、乱序和数据损坏；不得只检查线程是否返回。

## 6. 覆盖率门禁

核心覆盖率目标为行覆盖率不低于 90%，但必须针对明确的核心源码集合统计：

- `rtosInterface/queue/`
- `rtosInterface/*/queue_*.c`
- `thirdParty/AlgorithmLib/crc16.c`
- `thirdParty/ring/ring*.c`
- 后续纳入的纯 C 核心模块

以下内容不计入首阶段 Windows 主机门禁：

- STM32 HAL/BSP 和启动文件；
- FreeRTOS 内核及 CMSIS-OS；
- 真实设备驱动；
- 压缩包、生成代码、文档和示例。

覆盖率工具不可用时，测试报告必须写明 `coverage: unavailable`，不能把测试通过率当作覆盖率，也不能声称达到 90%。

## 7. CI/本地验收标准

通过条件：

- CMake 配置成功；
- Unity 测试目标构建成功；
- CTest 100% 通过；
- JUnit XML 成功生成；
- 失败用例有可定位的文件和行号；
- 覆盖率工具可用时核心行覆盖率 >= 90%；
- 覆盖率工具不可用时构建/测试通过，但覆盖率门禁状态为未完成；
- 结果报告记录平台、编译器、提交版本和执行命令。

## 8. 当前阶段结果

Windows/Visual Studio 2022 已验证：

- Unity 测试：1 个 CTest 测试项通过；
- 测试失败：0；
- CTest 总耗时：约 0.15 秒；
- JUnit XML：`build-test-vs/test-results.xml`；
- 覆盖率工具：当前环境未检测到 OpenCppCoverage、LLVM coverage 或 gcovr，因此覆盖率百分比暂未生成。

这表示测试运行环境和首批测试已经可用，但不等同于“全项目核心代码 90% 覆盖率已完成”。后续必须继续补充核心模块测试并安装/配置 Windows 覆盖率工具后，才能关闭 90% 门禁。
