# Unity 测试执行报告

- 项目：arnics-os
- 测试框架：Unity 2.6（仓库内置）
- 测试编排：CTest
- 平台：Windows x64
- 构建器：Visual Studio 17 2022
- 编译器：MSVC 19.44.35228.0
- 配置：Debug
- 执行日期：2026-07-19

## 测试结构

```text
tests/
├── unit/
│   ├── core/                 CRC16、ringbuffer、数据平台
│   ├── platform/             Windows queue backend
│   └── test_unit_main.c
├── system/
│   └── event_center/         事件中心收发、员工和压力测试
├── support/                  测试平台桩和 Windows RTOS 适配
├── run_tests.ps1             基础单元测试
└── run_event_system_tests.ps1 事件中心应用级系统测试
```

## 基础单元测试

执行命令：

```powershell
cmake --fresh -S . -B build-test-vs -G "Visual Studio 17 2022" -A x64 -DARNICS_BUILD_TESTS=ON
cmake --build build-test-vs --config Debug --target arnics-unit-tests --parallel 4 -- /v:minimal
ctest --test-dir build-test-vs -C Debug -R "^arnics-unit-tests$" --output-on-failure
```

| 指标 | 结果 |
|---|---:|
| CTest 测试项 | 1（包含 4 个 Unity 用例） |
| 通过 | 1 |
| 失败 | 0 |
| 通过率 | 100% |
| 总耗时 | 0.18 秒 |
| JUnit XML | `build-test-vs/unit-results.xml` |

已覆盖 CRC16、环形缓冲区、Windows 队列后端和数据平台核心数据读写。

## 事件中心应用级系统测试

执行命令：

```powershell
.\tests\run_event_system_tests.ps1 -BuildDir build-event-system -Configuration Debug
```

### 临时应用代码

系统测试按真实应用接入方式，在测试期间临时加入以下代码：

1. 三个员工消息类型，均包含 `request` 和 `response` 两个 `uint32_t` 字段。
2. 外部员工 `test_external_employee`：响应值为请求值加 100。
3. 内部员工 `test_internal_employee`：响应值为请求值乘 2。
4. 雇佣工 `test_hired_employee`：响应值与 `0x5a5a5a5a` 异或。
5. `REGISTER_ENTRIES` 临时注册以上三类员工，均要求返回响应。
6. `HIRD_EMPLOY_STATIC_REFERENCE` 为测试雇佣工配置普通优先级和 1024 字节栈。

脚本执行前保存 `centerEvent.h` 和 `eventList.h` 的原始字节，执行完成或失败时通过 `finally` 恢复。执行前后 SHA-256 校验结果：

```text
CENTER_RESTORED=True
EVENTLIST_RESTORED=True
```

因此生产事件中心源码中没有保留测试注册表、测试消息结构或测试注入宏。

### 执行结果

| Unity 用例 | 结果 |
|---|---|
| 外部员工请求—处理—响应 | PASS |
| 内部员工请求—处理—响应 | PASS |
| 雇佣工创建—处理—响应 | PASS |
| 多员工事件标志注册与非法员工拒绝 | PASS |
| 4 个并发发送端、每端 250 次、合计 1000 次交互 | FAIL（60 秒超时） |

| 指标 | 结果 |
|---|---:|
| Unity 用例 | 5 |
| 通过 | 4 |
| 失败 | 1 |
| 通过率 | 80% |
| 压力规模 | 4 × 250 = 1000 次请求/响应 |
| 超时阈值 | 60 秒 |
| JUnit XML | `build-event-system/event-system-results.xml` |

## 事件中心内核结论

### 已验证能力

- 外部员工、内部员工、雇佣工均可完成真实注册、消息投递、事件执行和响应获取。
- 单次请求的消息 ID、响应路由和消息数据回填工作正常。
- 非法员工名称不会被加入事件位图。
- 测试使用真实 `eventCore.c`、统一队列层和 Windows queue backend，不是仅验证结构体或位图。

### 发现的问题

1. **并发活性/吞吐不足**：1000 次并发请求/响应在 60 秒内无法全部结束，Windows 返回 `WAIT_TIMEOUT (258)`。这不是“258 条消息失败”，而是所有工作线程未在截止时间内退出。
2. **响应队列容量过小**：`EVENTOS_REQ` 和 `EVENTOS_RSP` 队列长度均为 3。高并发时生产者、事件中心和响应消费者频繁阻塞。
3. **响应查询为轮转扫描**：每个等待线程持有响应队列互斥锁，逐条取出不属于自己的响应，再重新入队。在多个并发等待者下产生锁竞争和重复搬运，吞吐随并发量快速下降。
4. **无限等待风险**：压力测试使用 `BLOCK_DELAY` 时，响应获取没有全局截止时间；若响应丢失、ID 异常或队列长期饥饿，调用线程可能永久等待。
5. **发送 ID 异常路径**：`SendEventCallToEventCenter()` 获取 ID 互斥锁失败后仍继续投递消息，此时消息 ID 保持为 0，调用者无法可靠匹配响应。
6. **响应缓冲区契约不安全**：`GetResponseMessageFromEventCenter()` 固定复制整个 `messageUnion_u`，API 没有接收缓冲区长度。如果应用按员工消息结构大小传入缓冲区，存在越界写风险；调用方必须实际提供完整 `messageUnion_u` 空间。

本轮按照“测试不改变最终生产源码”的要求，只记录缺陷，不修改事件中心内核以强行通过压力测试。

## 覆盖率状态

当前 MSVC 测试链路没有生成 gcov 覆盖率数据，因此不填写覆盖率百分比，也不宣称达到 90% 覆盖率门禁。
