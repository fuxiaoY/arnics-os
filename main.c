// example for arnics-os.

#include "Inc/include.h"

int main()
{
    arnics_task_init();
    while (1)
    {
        rtosThreadDelay(1000);
    }
    return 0;
}
