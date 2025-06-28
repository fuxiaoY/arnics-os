# Data Platform

[中文](dataPlat.md) | English

## 1. Overview

DataPlat is a lightweight embedded system data management platform designed to provide unified mechanisms for managing, storing, and accessing various data objects. By separating public and private documents, DataPlat achieves data isolation and sharing between modules. It also supports persistent storage on Flash or other non-volatile memory, ensuring critical data can be recovered after power loss.

### Key Features
- **Unified Data Management**: Supports the definition, storage, and access of various data types.
- **Modular Design**: Separates public and private documents to achieve module-level data isolation.
- **Persistent Storage**: Supports CRC-verified Flash storage to ensure data integrity.
- **Flexible Expansion**: Simplifies data operations with macro definitions, enabling quick feature extensions.

---

## 2. System Architecture

DataPlat adopts a layered architecture, consisting of the following core components:

### 2.1 Data Object Layer (dataObj)
Defines specific data objects, such as:
- **arnicsPara**: System parameters
- **systemCfg**: System configuration
- **systemStatus**: System status

### 2.2 Data Management Layer (dataManager)
Provides unified data operation interfaces, including:
- Data storage and loading
- Data validation and packing
- Data access control

### 2.3 Storage Abstraction Layer
Uses FAL (Flash Abstraction Layer) to provide unified access to physical storage devices.

---

## 3. File Organization

```
dataPlat/
  ├── dataObj/                 # Data object definitions
  │   ├── arnicsPara/          # ARNICS parameter object
  │   │   ├── arnicsPara.h     # Private header file
  │   │   ├── arnicsPara.c     # Implementation file
  │   │   └── arnicsParaPublic.h # Public header file
  │   ├── systemCfg/           # System configuration object
  │   │   ├── systemConfig.h   # Private header file
  │   │   ├── systemConfig.c   # Implementation file
  │   │   └── systemConfigPublic.h # Public header file
  │   ├── systemStatus/        # System status object
  │   │   ├── systemStatus.h   # Private header file
  │   │   ├── systemStatus.c   # Implementation file
  │   │   └── systemStatusPublic.h # Public header file
  │   └── dataClass.h          # Unified data object definitions
  ├── dataManager/             # Data management layer
  │   ├── dataManagerList.h    # Data management list
  │   ├── dataStore.h          # Data storage interface
  │   └── dataStore.c          # Data storage implementation
  ├── dataCore/                # Data core layer
  │   ├── dataCore.h           # Core interface definitions
  │   └── dataStruct.h         # Data structure definitions
  ├── dataApi/                 # Data access interface
  │   ├── dataApi.h            # Data access interface definitions
  │   └── dataPublic.h         # Public data access interface
  └── thirdParty/              # Third-party libraries
      ├── AlgorithmLib/        # Algorithm library
      │   └── crc16.h          # CRC16 checksum algorithm
      └── fal/                 # Flash abstraction layer
          ├── fal.h            # FAL interface definitions
```

---

## 4. Public/Private Document Definitions

### 4.1 Definition Mechanism
- **Public Documents**: Define data and interfaces shared between modules, located in `*Public.h` files.
- **Private Documents**: Define data structures, functions, and macros used internally by a module, located in `.h` and `.c` files.

### 4.2 Examples
#### Public Document
```c
// filepath: dataPlat/dataObj/arnicsPara/arnicsParaPublic.h
extern bool arnics_para_save(void);
extern bool arnics_para_load(void);
```

#### Private Document
```c
// filepath: dataPlat/dataObj/arnicsPara/arnicsPara.h
extern uint8_t virtual_environment;  
extern bool eventos_want_sleep;
```

---

## 5. Persistent Storage Support

DataPlat integrates with FAL (Flash Abstraction Layer) to provide unified access to Flash and other storage devices, enabling persistent data storage.

### 5.1 Storage Mechanism
- **CRC Validation**: Uses CRC16 checksum to ensure data integrity.
- **Partition Management**: Supports managing Flash partitions for storing different types of data.
- **Data Packing/Unpacking**: Provides unified mechanisms for managing storage formats.

### 5.2 Core Interfaces
```c
bool data_save(const char *part_name, size_t offset, uint8_t *buf, size_t len);
bool data_load(const char *part_name, size_t offset, uint8_t *buf, size_t len);
bool value_pack_with_crc16(const store_operation_t* op, uint8_t* src_buf, size_t src_len);
bool value_load_with_crc16(const store_operation_t* op, uint8_t* dest_buf, size_t dest_len);
```

---

## 6. Data Operation Macros

DataPlat provides a series of macros to simplify the definition and operation of data objects:

```c
// Define a data structure with CRC validation
#define STRUCT_WITH_CRC(type, var) typedef struct {     uint16_t crc16;     type var; } STRUCT_WITH_CRC_NAME(var)

// Define storage support
#define SOTRE_SUPPORT(var) SMALLOC(STRUCT_WITH_CRC_NAME(var),var##_store, sizeof(STRUCT_WITH_CRC_NAME(var)))

// Define data operation functions
#define DEFINE_DATA_ACTION_FUNC(type_t,param) static bool param##_data_action(type_t *p_arnics_para_store, dataAction_e action) {     store_operation_t op;     operation_set(op, p_arnics_para_store, param);     if (DATA_PACK == action) {         return value_pack_with_crc16(&op, (uint8_t*)&param, sizeof(param));     } else if (DATA_LOAD == action) {         return value_load_with_crc16(&op, (uint8_t*)&param, sizeof(param));     }     return false; }
```

---

## 7. Example Code (Using arnicsPara)

Below is a complete example of managing `arnicsPara` parameters using DataPlat.

### 7.1 Define Parameters

In `arnicsParaPublic.h`, define the public parameter structure and interfaces:

```c
// filepath: dataPlat/dataObj/arnicsPara/arnicsParaPublic.h
typedef struct {
    uint32_t deviceId;         // Device ID
    uint8_t workMode;          // Work mode
    uint16_t commInterval;     // Communication interval
} arnicsParams_t;

extern arnicsParams_t arnicsParams;  // Global parameter instance
extern bool loadArnicsParams(void);  // Load parameters interface
extern bool saveArnicsParams(void);  // Save parameters interface
```

### 7.2 Implement Parameter Storage Operations

In `arnicsPara.c`, implement the logic for loading and saving parameters:

```c
// filepath: dataPlat/dataObj/arnicsPara/arnicsPara.c
#include "arnicsPara.h"
// ...existing code...

bool arnics_para_save(void) {
    bool result = false;
    dataStoreArnicsPara_t arnics_para_store;
    DATA_ACTION_FUNC(&arnics_para_store, virtual_environment, DATA_PACK);
    DATA_ACTION_FUNC(&arnics_para_store, sys_allow_sleep, DATA_PACK);
    result = data_save(PARTITION_NAME_ARNICS_SYS_PARA, 0, (uint8_t *)&arnics_para_store, sizeof(arnics_para_store));
    return result;
}

bool arnics_para_load(void) {
    bool result = false;
    bool need_reset_load = false;
    dataStoreArnicsPara_t arnics_para_store;
    data_load(PARTITION_NAME_ARNICS_SYS_PARA, 0, (uint8_t *)&arnics_para_store, sizeof(arnics_para_store));
    result = DATA_ACTION_FUNC(&arnics_para_store, virtual_environment, DATA_LOAD);
    if (!result) {
        need_reset_load = true;
        virtual_environment_reset();
    }
    result = DATA_ACTION_FUNC(&arnics_para_store, sys_allow_sleep, DATA_LOAD);
    if (!result) {
        need_reset_load = true;
        sys_allow_sleep_reset();
    }
    if (need_reset_load) {
        result = arnics_para_save();
    }
    return result;
}
```

### 7.3 Usage Example

In the application, use the `arnicsPara` parameters:

```c
// filepath: dataPlat/dataObj/arnicsPara/appExample.c
#include "arnicsParaPublic.h"
#include <stdio.h>

void appInit(void) {
    if (!arnics_para_load()) {
        arnics_start = false;
        arnics_systick = 0;
        sys_allow_sleep = false;
        virtual_environment = 0;
        arnics_para_save();
    }

    printf("Arnics Start: %d\n", arnics_start);
    printf("Arnics Systick: %u\n", arnics_systick);
    printf("Virtual Environment: %u\n", virtual_environment);

    virtual_environment = 1;
    arnics_para_save();
}
```

---

## 8. Summary

DataPlat provides a structured approach to managing various data in embedded systems. Through public/private document separation, CRC data validation, and persistent Flash storage, it ensures data security, integrity, and recoverability. Developers can use the provided macros and APIs to quickly define and manage data objects, improving code maintainability and system reliability.

Key Features:
1. Unified data access interfaces
2. Public/private document separation
3. CRC16-based data integrity validation
4. Support for Flash and other non-volatile storage media
5. Simplified development process with concise macros