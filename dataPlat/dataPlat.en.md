# Data Management Platform
[中文](dataPlat.md) | English

## 1. Overview

DataPlat is a lightweight embedded system data management platform designed to provide unified data management, storage, and access mechanisms. By separating public/private documents, DataPlat achieves data sharing and isolation between modules and supports persistent storage to ensure critical data recovery after power loss.

### Core Features
- **Unified Data Management**: Supports the definition, storage, and access of various data types.
- **Modular Design**: Achieves module-level data isolation through public/private document separation.
- **Persistent Storage**: Supports Flash storage with CRC verification to ensure data integrity.
- **Flexible Extension**: Simplifies data operations through macro definitions, enabling rapid feature expansion.

---

## 2. System Architecture

DataPlat adopts a layered architecture design, consisting of the following three core layers:

### 2.1 Data Object Layer (dataObj)
Defines specific data objects, such as:
- **arnicsPara**: System parameters
- **systemCfg**: System configuration
- **systemStatus**: System status

### 2.2 Data Management Layer (dataManager)
Provides unified data operation interfaces, including:
- Data storage and loading
- Data validation and packaging
- Data access control

### 2.3 Storage Abstraction Layer
Provides unified access to physical storage devices through FAL (Flash Abstraction Layer).

---

## 3. File Organization Structure

```
dataPlat/
  ├── dataObj/                 # Data object definitions
  │   ├── arnicsPara/          # System parameter objects
  │   │   ├── arnicsPara.h     # Private header file
  │   │   ├── arnicsPara.c     # Implementation file
  │   │   └── arnicsParaPublic.h # Public header file
  │   ├── systemCfg/           # System configuration objects
  │   │   ├── systemConfig.h   # Private header file
  │   │   ├── systemConfig.c   # Implementation file
  │   │   └── systemConfigPublic.h # Public header file
  │   ├── systemStatus/        # System status objects
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
      └── fal/                 # Flash Abstraction Layer
          ├── fal.h            # FAL interface definitions
```

---

## 4. Public/Private Document Definitions

### 4.1 Definition Mechanism
- **Public Documents**: Define shared data and interfaces between modules, located in `*Public.h` files.
- **Private Documents**: Define data and functions used internally within modules, located in `.h` and `.c` files.

### 4.2 Example
#### Public Document
```c
// filepath: dataPlat/dataObj/arnicsPara/arnicsParaPublic.h
extern bool arnics_para_save(void);
extern bool arnics_para_load(void);
```

#### Private Document
```c
// filepath: dataPlat/dataObj/arnicsPara/arnicsPara.h
extern uint8_t           virtual_environment;  
extern bool              eventos_want_sleep;
```

---

## 5. Persistent Storage Support

DataPlat provides support for non-volatile storage such as Flash, ensuring data persistence and integrity.

### 5.1 Storage Mechanism
- **CRC Verification**: Uses CRC16 checksum to ensure data integrity.
- **Partition Management**: Manages Flash partitions through FAL, storing different types of data in separate partitions.
- **Data Packing/Unpacking**: Manages storage formats through unified packing and unpacking mechanisms.

### 5.2 Core Interfaces
```c
bool data_save(const char *part_name, size_t offset, uint8_t *buf, size_t len);
bool data_load(const char *part_name, size_t offset, uint8_t *buf, size_t len);
bool value_pack_with_crc16(const store_operation_t* op, uint8_t* src_buf, size_t src_len);
bool value_load_with_crc16(const store_operation_t* op, uint8_t* dest_buf, size_t dest_len);
```

---

## 6. Data Operation Macros

DataPlat provides a series of macro definitions to simplify the definition and operation of data objects:

```c
// Define a data structure with CRC verification
#define STRUCT_WITH_CRC(type, var) 
// Define storage support
#define SOTRE_SUPPORT(var) 
// Define data operation functions
#define DEFINE_DATA_ACTION_FUNC(type_t, param) 
```

---

## 7. Example Code (Using arnicsPara)

The following is a complete example of managing `arnicsPara` parameters using DataPlat.

### 7.1 Define Parameters

The `arnicsPara` folder demonstrates examples of defining public/private documents, storage support, and data operation functions.

```
filepath: dataPlat\dataObj\arnicsPara
```

---

This example demonstrates how to define, load, save, and use parameters with the DataPlat platform. Developers can use this template to quickly implement the management of other parameters.
