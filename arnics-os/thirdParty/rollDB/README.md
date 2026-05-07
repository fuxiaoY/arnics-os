# rollDB 数据库

`rollDB` 是一个轻量级的数据库，专为嵌入式系统设计，支持日志的增、删、查操作，并具有自动回滚功能。它基于 Flash 存储，可提供高效的日志管理能力，适用于资源受限的嵌入式设备。

---

## 特性

- **轻量级**：占用资源少，适合嵌入式系统。
- **自动回滚**：当存储空间不足时，自动清理旧日志。
- **高效存储**：支持按扇区管理日志，优化 Flash 写入和擦除操作。
- **灵活读取**：支持批量读取和按范围读取日志。
- **容量管理**：提供剩余容量查询功能，便于监控存储使用情况。

---

## 原理与机制

### 数据库结构

`rollDB` 将存储空间划分为两个分区：
1. **系统分区 (System Info)**：
   - 存储数据库的元信息，如日志数量、当前写入扇区等。
   - 用于管理日志分区的状态。

2. **日志分区 (Log Sector)**：
   - 存储实际的日志数据。
   - 每条日志由日志头和日志数据组成。

### 数据库布局

```
+-------------------+
|    System Sector  |  系统分区
+-------------------+
|                   |  
|     Log Sector    |  日志分区(多个扇区循环使用)
|                   |
+-------------------+
```

### 系统分区字段

| 字段名              | 描述                                   |
|---------------------|----------------------------------------|
| `magic_valid`       | 系统分区有效标志，用于验证分区有效性。 |
| `sys_info_len`      | 系统分区数据长度。                     |
| `log_data_start`    | 日志分区中第一条日志的起始地址。       |
| `log_data_end`      | 日志分区中最后一条日志的结束地址。     |
| `log_num`           | 当前日志数量。                         |
| `log_action_num`    | 日志操作序号。                         |
| `current_sector`    | 当前写入的日志扇区索引。               |
| `log_sector_status` | 每个日志扇区的状态。                   |

### 日志分区字段

| 字段名         | 描述                                   |
|----------------|----------------------------------------|
| `magic_valid`  | 日志有效标志，用于验证日志有效性。     |
| `addr_offset`  | 下一个日志的地址偏移。                 |
| `number`       | 日志序号。                             |
| `len`          | 当前日志的总长度（包括日志头）。       |

---

## 回滚原理

`rollDB` 的回滚机制用于在存储空间不足时自动清理旧日志，以确保数据库能够持续写入新日志。回滚机制的核心思想是通过循环使用日志分区，清理最早的日志数据，为新日志腾出空间。

### 回滚触发条件

1. 当前扇区的剩余空间不足以容纳新日志。
2. 当前扇区已被标记为 `SECTOR_FULL`。
3. 写入新日志时，检测到需要切换到下一个扇区。

### 回滚流程

1. **切换扇区**：
   - 检查当前扇区是否已满或无法容纳新日志。
   - 如果需要，切换到下一个扇区。

2. **清理旧日志**：
   - 如果切换到的扇区不是空的（状态不为 `SECTOR_EMPTY`），则擦除该扇区。
   - 在擦除前，统计该扇区内的有效日志数量，并从系统分区中减去这些日志数量。

3. **更新系统分区**：
   - 更新系统分区中的日志起始地址 (`log_data_start`) 和日志数量 (`log_num`)。
   - 将系统分区数据写回 Flash。

4. **继续写入**：
   - 在新的扇区中写入新日志。


## 用法

### 初始化数据库

```c
rollts_sys_t sys_handle;
if (rollts_init(&sys_handle)) {
    printf("Database initialized successfully.\n");
} else {
    printf("Database initialization failed.\n");
}
```

### 追加日志

```c
uint8_t data[] = "This is a test log.";
rollts_log_t log_handle;
if (rollts_add(&sys_handle, &log_handle, data, sizeof(data))) {
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

if (rollts_read_all(&sys_handle, &log_handle, log_callback)) {
    printf("Logs read successfully.\n");
} else {
    printf("Failed to read logs.\n");
}
```

### 按范围读取日志

```c
if (rollts_read_pick(&sys_handle, &log_handle, 2, 5, log_callback)) {
    printf("Logs read successfully.\n");
} else {
    printf("Failed to read logs.\n");
}
```

### 清除日志

```c
if (rollts_clear(&sys_handle)) {
    printf("Logs cleared successfully.\n");
} else {
    printf("Failed to clear logs.\n");
}
```

### 查询日志数量

```c
uint32_t log_count = rollts_num(&sys_handle);
printf("Number of logs: %u\n", log_count);
```

### 查询剩余容量

```c
uint32_t capacity = rollts_capacity(&sys_handle);
printf("Remaining capacity: %u%%\n", capacity);

uint32_t capacity_size = rollts_capacity_size(&sys_handle);
printf("Remaining capacity size: %u bytes\n", capacity_size);
```

---

## API 函数表

| 函数名                        | 描述                                   |
|-------------------------------|----------------------------------------|
| `bool rollts_init(rollts_sys_t *handle)` | 初始化数据库。                       |
| `bool rollts_deinit(rollts_sys_t *handle)` | 注销数据库，擦除系统分区。           |
| `bool rollts_clear(rollts_sys_t *handle)` | 清除所有日志数据。                   |
| `bool rollts_add(rollts_sys_t *handle, rollts_log_t *log_handle, uint8_t *data, uint16_t length)` | 追加一条日志数据。<br>支持指定数据和长度。 |
| `bool rollts_read_all(rollts_sys_t *handle, rollts_log_t *log_handle, rollTscb cb)` | 批量读取所有日志。<br>支持回调处理日志数据。 |
| `bool rollts_read_pick(rollts_sys_t *handle, rollts_log_t *log_handle, uint32_t start_num, uint32_t end_num, rollTscb cb)` | 按范围读取日志。<br>支持指定起始和结束序号。 |
| `uint32_t rollts_num(rollts_sys_t *handle)` | 查询当前日志数量。                   |
| `uint32_t rollts_capacity(rollts_sys_t *handle)` | 查询剩余容量百分比。                 |
| `uint32_t rollts_capacity_size(rollts_sys_t *handle)` | 查询剩余容量大小（字节）。           |


## 贡献

欢迎提交 Issue 和 Pull Request 来改进 `rollDB`。

---

## 许可证

`rollDB` 遵循 MIT 许可证。有关详细信息，请参阅 [LICENSE](LICENSE) 文件。