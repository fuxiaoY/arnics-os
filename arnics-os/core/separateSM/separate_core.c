#include "separate_core.h"

// Kernel execution function
void separate_kernel_loop(SEPARATE_KERNEL *kernel,unsigned char autoLoop)
{
    do
    {
        void *currentState = kernel->getState(kernel->context); // Get the current state
        int found = 0;                                          // Flag to indicate if a matching state is found

        for (int i = 0; i < kernel->stateCount; i++)
        {
            if (SEPARATE_STATE_ID_COMPARE(kernel->states[i].stateID, currentState))
            {
                kernel->states[i].execute(); // Execute the corresponding state
                found = 1;                  // Mark as found
                break;
            }
        }

        if (!found && kernel->onNotFound)
        {
            kernel->onNotFound(); // If no matching state is found, call the callback function
        }
    }while(autoLoop);
}
