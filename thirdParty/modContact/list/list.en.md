[中文](list.md) | English

# mct Instance Interface Adaptation

## Protocol Registration
mct registers protocols via the `modemList[]` array in `mctlist.c` to identify which protocol is used by each instance.
```
const tModemList modemList[] =
{
    {.name = "CONSOLE",    .api = mctConsoleApiGet},
    {.name = "A7680C",     .api = mctA7680CApiGet},
    {.name = "Y7025",      .api = mctY7025ApiGet},
};
```
Users can add any number of protocols to the list for mct to register and select.

## Protocol Construction
1. Each protocol is divided into a **protocol layer** and a **protocol adapter layer**.  
2. The protocol adapter layer is what gets added to `modemList[]`.

![Protocol Implementation](../document/mctlist.svg)

Create a new protocol folder under the `list` directory, e.g., for a protocol named `xprotocol`:
|-xprotocol  
&nbsp;&nbsp;|-command_xprotocol.c  
&nbsp;&nbsp;|-command_xprotocol.h  
&nbsp;&nbsp;|-command_xprotocolAdapter.c  
&nbsp;&nbsp;|-command_xprotocolAdapter.h  

### Protocol Layer
The protocol layer provides packing and parsing interfaces for command formats.  
For example, for the following AT command frame interaction:
```
MCU disables module echo
mcu->mod : ATI0\r\n 

Module replies echo disabled successfully
mod->mcu : OK\r\n ATI0:0\r\n
```

1. Add packing and parsing interfaces for the command format:
```
/**
 * @fn cmd_PackATcommand
 * @brief AT command frame packing interface
 * 
 * @param [in] buf - pointer to the packed command frame buffer
 * @param [in] len - pointer to the packed frame length
 * @param [in] para - user parameter for packing
 * @retval None
 */
static bool cmd_PackATcommand(uint8_t* buf, size_t *len, void *para)
{
    uint8_t *echo = (uint8_t *)para;
    // Pack buf in ATE<X> format, where X is echo
    return true;
}
```
```
/**
 * @fn cmd_AnalyzeATcommand
 * @brief AT command frame parsing interface
 * 
 * @param [in] buf - pointer to the received frame buffer
 * @param [in] len - length of the received frame
 * @param [in] para - pointer to application data to return
 * @retval None
 */
static bool cmd_AnalyzeATcommand(uint8_t* buf, size_t len, void *para)
{
    uint8_t *echo = (uint8_t *)para;
    // Parse len bytes in buf with expected format ATE:<X>, assign X to echo
    return true;
}
```

2. Construct protocol command interface:  
The protocol uses the **tCmd** structure for description. A single command parameter is defined as follows:  
**ASCII Protocol**  
Frame ID | Timeout | Frame Header | Frame Tail | Error Field | Frame Type | Pack & Parse Function Name

```
CMD_ADD(CMD_CONSOLE_ID_REV, 2,  "$$COMX$$","*#*#",  NULL,  RecvSend,  RevFlow),
```
**Byte Stream Protocol**  
Frame ID | Timeout | Header | Header Len | Tail | Tail Len | Error Field | Error Len | Frame Type | Pack & Parse Function Name
```
CMD_HEX_ADD(CMD_CONSOLE_ID_REV2,   2,  &header,sizeof(header),      &tail,sizeof(header),       NULL,0,   RecvSend RevHexFlow),
```
