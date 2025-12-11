# rollDB 数据库

`rollDB` 是一个轻量级的数据库，专为嵌入式系统设计，支持日志的增、删、查操作，并具有自动回滚功能。它基于 Flash 存储，可提供高效的日志管理能力，适用于资源受限的嵌入式设备。

版本：`1.0.1`

---

## 特性

- 轻量级：占用资源少，适合嵌入式系统。
- 自动回滚：当存储空间不足时，自动清理旧日志。
- 高效存储：支持按扇区管理日志，优化 Flash 写入和擦除操作。
- 灵活读取：支持批量读取和按范围读取日志。
- 容量管理：提供剩余容量查询功能，便于监控存储使用情况。

---

## 原理与机制

### 数据库结构

`rollDB` 将存储空间划分为两个分区：
1. 系统分区 (System Sector)：
   - 存储数据库的元信息，如日志区范围、块大小等。
   - 用于管理日志分区的状态与布局。

2. 日志分区 (Log Sector)：
   - 存储实际的日志数据。
   - 每条日志由日志头和日志数据组成。

### 数据库布局

```
+-------------------+
|    System Sector  |  系统分区（地址 0）
+-------------------+
|                   |
|     Log Sector    |  日志分区(多个扇区循环使用)
|                   |
+-------------------+
```

### 系统分区字段

| 字段名 | 描述 |
|--------|------|
| `magic_valid` | 系统分区有效标志，用于验证分区有效性（`core/rollTs.c:91-102` 初始化）。 |
| `data_start_block_num` | 日志分区起始块编号（固定为 1）。 |
| `data_end_block_num` | 日志分区结束块编号（`ROLLTS_MAX_BLOCK_NUM - 1`）。 |
| `log_size` | 日志分区大小（字节）。 |
| `rollts_max_size` | 数据库总大小（字节）。 |
| `single_block_size` | 单块大小（字节）。 |
| `min_write_unit_size` | 最小编程粒度（字节）。 |
| `rollts_max_block_num` | BLOCK 总数。 |
| `data_start_addr` | 日志分区起始地址。 |
| `data_end_addr` | 日志分区结束地址。 |

### 日志分区字段

| 字段名 | 描述 |
|--------|------|
| `magic_valid` | 日志有效标志，用于验证日志有效性（`MAGIC_DATA_VALID`）。 |
| `pre_addr` | 上一条日志地址。 |
| `cur_addr` | 当前日志地址。 |
| `next_addr` | 下一条日志地址。 |
| `payload_len` | 当前日志数据长度。 |

### 日志分区数据结构设计

#### 数据结构概述

- 日志分区由两个主要表构成：`rollts_data_t` 与 `block_info_t`。
- `block_info_t` 位于每个块的起始位置，用于描述块级元信息（块角色、最后数据地址、数据条数等）。
- `rollts_data_t` 紧随 `block_info_t` 之后按序排列，构成块内的链式日志记录，每条记录包含双向链表指针与负载长度。
- 二者共同组成“块头 + 块内链表”的结构：块头管理边界与计数，链表承载记录并依靠 `next_addr` 前进。

#### 注意事项

- 对齐限制：`ROLLTS_MAX_SIZE` 必须是 `SINGLE_BLOCK_SIZE` 的整数倍，且块数至少为 5。
- 负载大小：单条记录总长不得超过 `single_block_size - sizeof(block_info_t) - 4`。
- 一致性修复：启动时若块尾信息不一致将自动修复 `last_data_addr/data_num`。
- 并发与互斥：启用 `RTOS_MUTEX_ENABLE` 时需正确实现 `mutex_lock/unlock`，避免读写竞态。
- 擦除与写入：块迁移会触发擦除操作，请确保底层闪存驱动的擦除/写入原子性与可靠性（`flash_ops_t`）。
- 遍历边界：读取时遇到非有效 `magic_valid` 即停止当前块遍历，避免越界访问。

## 回滚原理

`rollDB` 的回滚机制用于在存储空间不足时自动清理旧日志，以确保数据库能够持续写入新日志。核心思想是通过循环使用日志分区，清理最早的日志数据，为新日志腾出空间。

### 回滚触发条件

1. 当前块剩余空间不足以容纳新日志。
2. 当前块已到达块末尾（被视为满块）。
3. 写入新日志时需要切换到下一个块。

### 回滚流程

1. 切换块：
   - 检查当前块是否已满或无法容纳新日志。
   - 若需要，切换到下一个块（`head_block_move`）。

2. 清理旧日志：
   - 进入下一个块前，若该块已有数据则擦除并初始化块头。
   - 统计块内有效日志数量并写入块头（`data_num`）。

3. 更新系统分区与目录表：
   - 更新 `head` 与 `head_backup` 的定位。
   - 更新 `pre_addr` 以指向当前写入块。

4. 继续写入：
   - 在新的块内从块头偏移处开始写入日志。

---

## 用法

### 初始化数据库

```c
rollts_manager_t mgr = {0};
mgr.flash_ops.erase_sector  = erase_sector;
mgr.flash_ops.write_data    = write_data;
mgr.flash_ops.read_data     = read_data;
#ifdef RTOS_MUTEX_ENABLE
mgr.flash_ops.mutex_lock    = mutex_lock;
mgr.flash_ops.mutex_unlock  = mutex_unlock;
#endif

if (rollts_init(&mgr) == 0) {
    printf("Database initialized successfully.\n");
} else {
    printf("Database initialization failed.\n");
}
```

### 追加日志

```c
uint8_t data[] = "This is a test log.";
if (rollts_add(&mgr, data, sizeof(data))) {
    printf("Log added successfully.\n");
} else {
    printf("Failed to add log.\n");
}
```

### 批量读取日志

```c
bool log_callback(uint8_t *buf, uint32_t len) {
    printf("Log: %.*s\n", len, buf);
    return true; // 返回 true 继续读取
}

uint8_t buf[128];
if (rollts_get_all(&mgr, buf, sizeof(buf), log_callback)) {
    printf("Logs read successfully.\n");
} else {
    printf("Failed to read logs.\n");
}
```

### 按范围读取日志

```c
uint8_t buf[128];
if (rollts_read_pick(&mgr, 2, 5, buf, sizeof(buf), log_callback)) {
    printf("Logs read successfully.\n");
} else {
    printf("Failed to read logs.\n");
}
```

### 清除日志

```c
if (rollts_clear(&mgr)) {
    printf("Logs cleared successfully.\n");
} else {
    printf("Failed to clear logs.\n");
}
```

### 查询日志数量

```c
int32_t log_count = rollts_get_total_record_number(&mgr);
printf("Number of logs: %ld\n", (long)log_count);
```

### 查询剩余容量

```c
uint8_t capacity = rollts_capacity(&mgr);
printf("Remaining capacity: %u%%\n", capacity);

uint32_t capacity_size = rollts_capacity_size(&mgr);
printf("Remaining capacity size: %u KB\n", capacity_size);
```

---

## API 函数表

| 函数名 | 描述 |
|--------|------|
| `int rollts_init(rollts_manager_t *rollts_manager)` | 初始化数据库，完成系统分区校验与格式化。 |
| `bool rollts_clear(rollts_manager_t *rollts_manager)` | 清除所有日志数据并重新初始化。 |
| `bool rollts_add(rollts_manager_t *rollts_manager, uint8_t *data, uint32_t payload_len)` | 追加一条日志数据。 |
| `bool rollts_get_all(rollts_manager_t *rollts_manager, uint8_t *data, uint32_t max_payload_len, rollTscb cb)` | 批量读取所有日志并通过回调处理。 |
| `bool rollts_read_pick(rollts_manager_t *rollts_manager, uint32_t start_num, uint32_t end_num, uint8_t *data, uint32_t max_payload_len, rollTscb cb)` | 按范围读取日志。 |
| `int32_t rollts_get_total_record_number(rollts_manager_t *rollts_manager)` | 查询当前日志总数。 |
| `uint8_t rollts_capacity(rollts_manager_t *rollts_manager)` | 查询剩余容量百分比。 |
| `uint32_t rollts_capacity_size(rollts_manager_t *rollts_manager)` | 查询容量大小（KB）。 |

---

## 贡献

欢迎提交 Issue 和 Pull Request 来改进 `rollDB`。

---

## 许可证

`rollDB` 遵循 MIT 许可证。有关详细信息，请参阅 [LICENSE](LICENSE) 文件。
