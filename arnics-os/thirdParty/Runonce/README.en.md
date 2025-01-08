# Runonce

**Are you still struggling with programming logic that should only execute once or a limited number of times in your code? Do you have to check if the logic has already been executed every time you handle repetitive logic?**

The `Runonce` library provides highly integrated logic management, which is very useful for handling operations that should not be repeated (such as initialization, single-trigger logic), improving code readability and maintainability while reducing the need for scattered state management.


## Function Description

### RUN_ONCE_INIT(function_name)
- **Description**: Initializes the execution control for a function, setting its default state to unexecuted.
- **Example**:
  ```
  int print_once(int num)
  {
    printf("this will print once,num is :%d\n", num);
    return 1;
  }
  RUN_ONCE_INIT(print_once);
  ```
### RUN_ONCE(function_name，function_call)
- **Description**: Executes the function once. If the function has already been executed, it will not be executed.
- **Example**:
  ```
  RUN_ONCE(print_once, int result = print_once(1));
  ```

### RUN_RESET(function_name)
- **Description**:  Resets the execution state, allowing the function or code block to be executed again.
- **Example**:
  ```
  RUN_RESET(print_once);
  ```

## Complete Example:
  ```
#include <stdio.h>
#include "Runonce.h"

// Define a function that executes only once
int print_once(int num)
{
  printf("This will print once, num is: %d\n", num);
  return 1;
}
// Initialize the execution state
RUN_ONCE_INIT(print_once);

// Define another function that executes only once
void setup_hardware()
{
  printf("Hardware setup complete.\n");
}
// Initialize the execution state
RUN_ONCE_INIT(setup_hardware);

void main(void)
{
  int result = 0;
  while(1)
  {
      // Try to call the print_once function multiple times, but it will only execute once
      RUN_ONCE(print_once, result = print_once(1));
      RUN_ONCE(print_once, result = print_once(2));
      RUN_ONCE(print_once, result = print_once(3));

      // Try to call the setup_hardware function multiple times, but it will only execute once
      RUN_ONCE(setup_hardware, setup_hardware());
      RUN_ONCE(setup_hardware, setup_hardware());

      // Reset the execution state of print_once
      RUN_RESET(print_once);

      // Call the print_once function again; since the state has been reset, it will execute again
      RUN_ONCE(print_once, int result = print_once(4));
  }
}
  ```
Execution result:
  ```
This will print once, num is: 1
Hardware setup complete.
This will print once, num is: 4
  ```


