[中文](dataPlat.md) | English

# dataLink Decoupling Description

## Purpose
`dataLink` serves as the parameter mapping and access layer for modContact. It abstracts various global parameters (such as configuration, status, network parameters, etc.) into an indexed parameter table and provides unified read/write interfaces, allowing upper modules to access and modify underlying data via indices.

## Decoupling Approach
- **Parameter Abstraction**: All exposed parameters are listed in the `dataLinkDef.h` file, which automatically generates index enums and parameter table structures.
- **Unified Interface**: Functions like `mctParaGetPtr`, `mctParaGet`, and `mctParaSet` hide the physical storage details of parameters. The upper layer only needs to operate via indices.
- **Type Safety**: Each parameter has a length field to prevent out-of-bounds access.
- **Easy Expansion**: To add a new parameter, simply add a line to `dataLinkDef.h` without modifying other code.

## Main Interfaces

- `void* mctParaGetPtr(mctParaIndex id);`
  - Get a pointer to the parameter for direct data access.
- `uint32_t mctParaGet(mctParaIndex id, void* outBuf, uint32_t bufLen);`
  - Read parameter content into a buffer, returning the actual copied byte count.
- `uint32_t mctParaSet(mctParaIndex id, const void* inData, uint32_t dataLen);`
  - Write data to the parameter, returning the actual written byte count.

## Usage

1. **Parameter Access**  
   Use macros like `MCT_GET(ID, OUT_BUF, BUF_LEN)` or `MCT_SET(ID, IN_DATA, DATA_LEN)` for parameter read/write.

2. **Adding Parameters**  
   Add a line to `dataLinkDef.h` to support new parameters automatically.

## Decoupling Advantages

- The upper layer does not need to care about the storage structure, only access by index.
- Facilitates parameter migration, refactoring, and unit testing.
- Supports dynamic parameter expansion and unified management.