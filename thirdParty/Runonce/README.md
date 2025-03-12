# Runonce

**是否还在为代码中只执行一次或只执行数次的编程逻辑而苦恼？ 是否在处理重复逻辑时，每次都要判断是否已经执行过？**

`Runonce` 库提供了高度集成化的逻辑管理，对于处理不应重复的代码操作非常有用（如初始化、逻辑单次触发等）,使用Runonce可提高代码可读性和可维护性，减少漫天飞舞的状态量管控。


## 函数功能说明

### RUN_ONCE_INIT(function_name)
- **说明**: 创建某函数的执行管控，默认为未执行。
- **示例**:
  ```
  int print_once(int num)
  {
    printf("this will print once,num is :%d\n", num);
    return 1;
  }
  RUN_ONCE_INIT(print_once);
  ```
### RUN_ONCE(function_name，function_call)
- **说明**: 执行该函数一次,如果该函数已经执行过，则不再执行。
- **示例**:
  ```
  RUN_ONCE(print_once, int result = print_once(1));
  ```

### RUN_RESET(function_name)
- **说明**: 重置执行状态，允许函数或代码块再次执行。
- **示例**:
  ```
  RUN_RESET(print_once);
  ```

## 完整的用例：
  ```

#include <stdio.h>
#include "Runonce.h"

// 定义一个只执行一次的函数
int print_once(int num)
{
    printf("This will print once, num is: %d\n", num);
    return 1;
}
// 初始化执行状态
RUN_ONCE_INIT(print_once);


// 定义另一个只执行一次的函数
void setup_hardware()
{
    printf("Hardware setup complete.\n");
}
// 初始化执行状态
RUN_ONCE_INIT(setup_hardware);



void main(void)
{
    int result = 0;

    // 尝试多次调用print_once函数，但只会执行一次
    RUN_ONCE(print_once, result = print_once(1));
    RUN_ONCE(print_once, result = print_once(2));
    RUN_ONCE(print_once, result = print_once(3));

    // 尝试多次调用setup_hardware函数，但只会执行一次
    RUN_ONCE(setup_hardware, setup_hardware());
    RUN_ONCE(setup_hardware, setup_hardware());

    // 重置print_once的执行状态
    RUN_RESET(print_once);

    // 再次调用print_once函数，由于状态已重置，会再次执行
    RUN_ONCE(print_once, int result = print_once(4));

}

  ```
执行结果如下：
  ```
This will print once, num is: 1
Hardware setup complete.
This will print once, num is: 4
  ```


