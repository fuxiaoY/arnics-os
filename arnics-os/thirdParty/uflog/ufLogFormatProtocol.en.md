# Message Format Specification
[中文文档](ufLogFormatProtocol.md) | English
## 1. Message Format Overview

### 1.1 Full Format

```
[pri] [facility] timestamp version> [kinds] msg (function|file|line) .
```

### 1.2 Component Description

| Component                | Position       | Separator | Optionality | Description                                         |
| ------------------------ | -------------- | --------- | ----------- | --------------------------------------------------- |
| pri                      | start          | space     | required    | Priority field, enclosed in square brackets         |
| facility                 | after pri      | space     | optional    | Category field, enclosed in square brackets; "-" when omitted |
| timestamp                | after facility | space     | optional    | Timestamp field; "-" when omitted                  |
| version>                 | after timestamp| fixed     | required    | Version field, format "number>" (currently "1>")   |
| kinds                    | after version> | space     | optional    | Module/type field, enclosed in square brackets; "-" when omitted |
| msg                      | after kinds    | space     | required    | Message content                                     |
| (function|file|line)     | after msg      | space     | optional    | Optional trio enclosed in parentheses              |
| .                        | end            | fixed     | required    | Message terminator                                  |

## 2. Field Definitions

### 2.1 pri (Priority)

| Token | Meaning       | Use case                                            |
| ----- | ------------- | --------------------------------------------------- |
| DBG   | Debug         | Debug information during development for troubleshooting |
| INF   | Info          | Normal runtime information, non-impacting           |
| WAR   | Warning       | Potential issues that do not block current operation |
| ERR   | Error         | Errors occurred but system still runs (reduced functionality) |
| ALT   | Alert/Critical| Severe errors/abnormalities that may disable subsystems or need immediate attention |
| NOT   | Notice        | Important notices (not errors), e.g. state changes or events |
| ALW   | Always         | Messages that must be output regardless of log level (e.g. critical audits/security logs) |

### 2.2 facility (Category)

| Name | Definition | Applicable area |
| ---- | ---------- | --------------- |
| driv | driver     | Device driver related messages |
| devc | device     | Hardware device related messages |
| kern | kernel     | System kernel related messages |
| mail | mail       | Mail service related messages |
| auth | auth       | Authentication related messages |
| temp | temp       | Temporary debugging or test messages |
| evet | event center| Event center business |
| mdia | media center| Media-related business |
| busi | business   | Decision/business layer |
| admi | admin/policy| Administration/policy related |
| guad | guard/monitor| Supervision/daemon processes |
| csle | console    | Console-related business |
| dplt | data platform| Data platform related |

### 2.3 timestamp

Full date-time

- Format: YYYY-MM-DD HH:MM:SS
- Example: `2000-01-01 03:59:44`

### 2.4 version

- Fixed value: 1
- Format: `1>` (number followed by ">")
- Purpose: versioning of message format. Currently fixed at 1; increment when format changes.
- Example: `1>`
- Note: A newline after the version> field is permitted and treated as a separator; however, keeping fields on the same line is recommended for integrity.

### 2.5 kinds (Module / Business Type)

Examples of module types (23 entries):

| Module | Description     | Module | Description      |
| ------ | --------------- | ------ | ---------------- |
| wwdg   | window watchdog | iwdg   | independent watchdog |
| tim    | timer           | uart   | UART / serial    |
| can    | CAN bus         | iic    | I2C bus          |
| spi    | SPI             | pwm    | PWM              |
| gpio   | GPIO            | sdio   | SD IO            |
| adc    | ADC             | dac    | DAC              |
| dma    | DMA             | rng    | RNG              |
| crc    | CRC             | rtc    | RTC              |
| fpu    | FPU             | hash   | Hash algorithms  |
| dcmi   | DCMI            | lcd    | LCD module       |
| fsmc   | FSMC            | app    | Application      |

Business-type examples (user-defined):

| Type   | Description        |
| ------ | ------------------ |
| blink  | LED blink strategy |
| colorful | LED fancy mode   |
| temp   | temporary use      |

### 2.6 msg (Message content)

Supported formats:

#### 2.6.1 struct data format

- Syntax: `[name var="xxx" var2=123 var3=[01 02 03 04] var4={a_teset:123,b_test:"123"}]`
- Description: Enclosed in square brackets; supports key-value pairs. Values may be strings, numbers, arrays, or objects.
- Use case: structured data transfer

#### 2.6.2 hex format

- Syntax: `|>0x85 0x96 0x69<|`
- Description: Use `|>` and `<|` as delimiters; hex bytes separated by spaces.
- Use case: display binary data

#### 2.6.3 json format

- Syntax: `{a_teset:123,b_test:"123"}`
- Description: Standard JSON-like structure
- Use case: complex data structures

#### 2.6.4 string format

- Syntax: `hello world! i want to print 23`
- Description: Plain text
- Use case: simple messages

### 2.7 (function|file|line) optional trio

- Format: `(function_name|file_path|line)`
- Description: three parts separated by a pipe character: function name, file path, and line number.
- Example: `(business_init|business_process.cpp|58)`
- Parts:
  - function: e.g. "business_init"
  - file: e.g. "business_process.cpp"
  - line: e.g. "58" — the source code line number of the logging statement

## 3. Constraints and Rules

### 3.1 Field omission rules

- Optional fields (facility, timestamp, kinds, the optional trio) may be omitted; use "-" when omitted.
- Required fields (pri, version>, msg, and '.') must not be omitted.
- Version field is currently fixed to 1 and must be present.
- Examples:
  - Omitting facility and timestamp: `[DBG] - - 1> [adc] message .`
  - Only required fields: `[DBG] - - 1> message .`

### 3.2 Special character handling

- Square brackets `[]`: field markers — must be preserved.
- Hex delimiters `|>` and `<|`: must be preserved.
- Comma `,`: used to separate array elements.
- Space: field separator; spaces inside message content are allowed.
- Double quotes `"`: string delimiter, e.g. `var="xxx"`.
- Trailing `>` after version: fixed delimiter immediately following the version number.

### 3.3 Data type conventions

- Array: use square brackets `[]`, elements separated by spaces, e.g. `[01 02 03 04]`.
- Object/dictionary: use braces `{}`, key-value pairs separated by commas, e.g. `{a_teset:123,b_test:"123"}`.
- Hex bytes: `0x` prefix, e.g. `0x85`.
- Strings: quoting optional; use double quotes when containing spaces.

## 4. Full Examples and Breakdown

### 4.1 Example 1: full-format message

```
[DBG] [temp] 00-01-01 03:59:44 1> [adc] this is a print,the local var is:32,{a_teset:123,b_test:"123"},hex is 
|> 51 68 07 65 16 08 <|
 (business_init|business_process.cpp|58) .
```

- Field breakdown:
  - pri: DBG
  - facility: temp
  - timestamp: 00-01-01 03:59:44
  - version: 1
  - kinds: adc
  - msg: mixed text, hex and struct data
  - optional trio: business_init, business_process.cpp, 58

### 4.2 Example 2: partial omission

```
[DBG] [temp] - 1> this is a print,the local var is:32 .
```

- Field breakdown:
  - pri: DBG
  - facility: temp
  - timestamp: - (omitted)
  - version: 1
  - kinds: - (omitted)
  - msg: text
  - optional trio: omitted

### 4.3 Example 3: minimal format

```
[DBG] - - 1> this is a print,the local var is:32 .
```

- Field breakdown:
  - pri: DBG
  - facility: - (omitted)
  - timestamp: - (omitted)
  - version: 1
  - kinds: - (omitted)
  - msg: text

### 4.4 Additional scenarios

#### 4.4.1 INF priority with JSON message

```
[INF] [driv] 2023-10-01 12:00:00 1> [uart] {"status":"success","data":123} (init_uart|serial.cpp|36) .
```

#### 4.4.2 ERR priority with hex message

```
[ERR] [kern] - 15:30:45 1> [can] |>0xFF 0x00 0xEE<| (can_send|can_driver.c|128) .
```

## 5. Best Practices

### 5.1 Format selection guidance

- Development & debugging: use DBG with full timestamp and function/file info.
- Monitoring: use INF/WAR and simplify format where appropriate.
- Error reporting: use ERR with full context.
- Performance-sensitive cases: omit timestamp and optional trio to reduce overhead.

### 5.2 Extensibility considerations

- Versioning: current format is 1; increment (e.g. 2>, 3>) on breaking changes.
- Adding facility: prefer 3–4 letter abbreviations to match existing style.
- Adding kinds: use English hardware/function abbreviations.
- Adding msg formats: use unique delimiters to avoid conflicts with existing patterns.
