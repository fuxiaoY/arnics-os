[English](dataPlat.en.md) | 中文

# dataLink 解耦说明

## 作用
`dataLink` 作为 modContact 的参数映射与访问层，负责将各类全局参数（如配置、状态、网络参数等）统一抽象为索引化的参数表，并提供统一的读写接口，便于上层模块通过索引访问和修改底层数据。

## 解耦思路
- **参数抽象**：通过 `dataLinkDef.h`中参数统一列举，自动生成索引枚举和参数表结构体。
- **统一接口**：提供 `mctParaGetPtr`、`mctParaGet`、`mctParaSet` 等接口，屏蔽具体参数的物理存储细节，上层只需通过索引操作。
- **类型安全**：每个参数都带有长度信息，防止越界访问。
- **易扩展**：新增参数只需在 `dataLinkDef.h`增加一行，无需修改其他代码。

## 主要接口说明

- `void* mctParaGetPtr(mctParaIndex id);`
  - 获取参数指针，适合直接操作原始数据。
- `uint32_t mctParaGet(mctParaIndex id, void* outBuf, uint32_t bufLen);`
  - 读取参数内容到缓冲区，返回实际拷贝字节数。
- `uint32_t mctParaSet(mctParaIndex id, const void* inData, uint32_t dataLen);`
  - 将数据写入参数，返回实际写入字节数。

## 使用方式

1. **参数访问**  
   通过宏 `MCT_GET(ID, OUT_BUF, BUF_LEN)` 或 `MCT_SET(ID, IN_DATA, DATA_LEN)` 进行参数读写。

2. **新增参数**  
   在`dataLinkDef.h` 增加一行即可自动支持，无需手动维护索引或表结构。

## 解耦优势

- 上层无需关心参数的具体存储结构，只需通过索引访问。
- 便于参数迁移、重构和单元测试。
- 支持参数动态扩展和统一管理。

