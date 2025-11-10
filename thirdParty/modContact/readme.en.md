[中文文档](readme.md) | English

# modContact

## Introduction
modContact (abbreviated as mct) is a universal serial communication framework designed for embedded platforms. It supports any master-slave protocol with a (request<->response) pattern and features an elegant master/slave switching mechanism, robust multi-frame recognition, frame splitting, and parsing.

Unlike other open-source communication software, mct is designed based on a more general approach to embedded serial communication: protocols such as AT commands or custom serial protocols typically follow a request<->response pattern. Any protocol with this pattern can be implemented using modContact.

## Commonality of Serial Communication Protocols
Almost all serial communication protocols follow a (request<->response) master-slave pattern: the master initiates a request, and the slave returns a response.
For example, in AT command sets, the MCU sends an AT command, and the module returns OK.
```
mcu->modem : AT
modem->mcu : OK
```

## modContact Design Philosophy
The request<->response master-slave interaction can be abstracted as follows:  
![Frame Interaction Behavior](document/abstract.svg)

modContact fully manages the above behavior and provides callback functions for frame packing and parsing, as well as unified interfaces for the application layer:

### Interface Description
modContact can create multiple instances, each controlling a separate serial port or protocol, enabling multi-protocol operation.

#### 1. Preparation
1. Two data buffers:  
   One for command packing and one for frame parsing. Buffer sizes depend on application needs, typically implemented as arrays, e.g.  
   <font color=#00FFFF>cmd_cache[100],   payload_cache[100]</font>

2. Two callback functions:  
   <font color=#FF0000>write_callback:</font>  
   Implemented by the user to write `len` bytes from `buf` to the serial port, returning the actual bytes sent.  
   <font color=#FF0000>read_callback:</font>  
   Implemented by the user to read up to `maxlen` bytes into `buf`, returning the actual bytes read.

#### 2. Initialize with Buffers and Callbacks
##### Initialization Function:
```
/**
 * @fn mct_init
 * @brief Initialize mct instance
 * 
 * @param [in] pInstance - mct instance pointer
 * @param [in] Cmd_cache - command buffer pointer
 * @param [in] cmd_max_size - command buffer size
 * @param [in] Payload_cache - parsing buffer pointer
 * @param [in] payload_max_size - parsing buffer size
 * @param [in] write_callback - write callback function pointer
 * @param [in] read_callback - read callback function pointer
 * @retval None
 */
void mct_init(MctInstance* pInstance,uint8_t *Cmd_cache,size_t cmd_max_size, \
                                    uint8_t* Payload_cahe,size_t payload_max_size, \
                                    int (*write_callback)(void *file, uint16_t len), \
                                    uint32_t (*read_callback)(uint8_t *buf, uint16_t maxlen))
```

#### 3. Register Protocol for the modContact Instance
##### Registration Function:
```
/**
 * @fn mct_reg
 * @brief Register mct protocol
 * 
 * @param [in] name - protocol type, see modemList[]
 * @param [in] inst - mct instance pointer
 * @param [in] ModemAtuoRecogniton - enable auto recognition
 * @retval true - success, false - failure
 */
 bool mct_reg(const char* name,MctInstance *inst,bool ModemAtuoRecogniton)
 ```

##### Registration Method 1:
Auto-recognize protocol, suitable for protocols with identification commands (e.g., AT commands with ATI0). mct provides AT module auto-recognition; other protocols can be implemented by the user.
```
mct_reg(NULL,&modemInstance,true);
```

##### Registration Method 2:
Manually specify the protocol for the instance, suitable for custom protocols or when auto-recognition is not needed. For example, specify "CONSOLE":
```
mct_reg("CONSOLE",&consoleInstance,false);
```

#### 4. Call Interface
Unified interface for serial communication:
```
/**
 * @fn mctApiExecute
 * @brief Execute mct API
 * This function finds and executes the command function by command ID.
 * 
 * @param [in] inst - mct instance pointer
 * @param [in] id - command ID
 * @param [in] para - user data pointer
 * @retval true - success
 * @retval false - failure
 */
mctApiExecute(&consoleInstance,CMD_REV_FLOW,NULL);
```
#### 5. Adaptation Interface

Refer to `list/list.md` for adaptation instructions to add custom protocols to mct.

#### 6. Other
Since version 1.0.4, sticky-frame parsing can be optionally enabled or disabled. It is enabled by default.