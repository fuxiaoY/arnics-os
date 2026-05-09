// example for arnics-os.

#include "Inc/include.h"
#include "Inc/typedef.h"

#define ARNICS_GLOBAL_REGISTRY(X)                                     \
    X("peripheralInit", peripheralInit, INIT_TAG, 1)                  \
    X("preLoadInit",       preLoadInit, INIT_TAG, 1)                  \
    X("deviceInit",         deviceInit, INIT_TAG, 1)


ARNICS_KERNEL_DECLARE_AND_BUILD_TABLE(arnics_init, ARNICS_GLOBAL_REGISTRY);


int main()
{
    arnics_core_init();
	arnics_task_init();
	while (1)
    {
        rtosThreadDelay(1000);
    }
	return 0;
}
