#ifndef _RUNONCE_H
#define _RUNONCE_H

#ifdef __cplusplus
extern "C" {
#endif

// Initialize execution status
#define RUN_ONCE_INIT(function_name) \
    char executed_##function_name##Flag = 0

// Expose execution status
#define RUN_ONCE_EXPOSE(function_name) \
    extern char executed_##function_name##Flag

// Execute scheme
#define RUN_ONCE(function_name, function_call) \
    ({ \
        extern char executed_##function_name##Flag; \
        if (!executed_##function_name##Flag) { \
            executed_##function_name##Flag = 1; \
                function_call; \
        } \
    })

// Reset execution status
#define RUN_RESET(function_name) \
    ({ \
        extern char executed_##function_name##Flag; \
        executed_##function_name##Flag = 0; \
    })

#ifdef __cplusplus
}
#endif

#endif // !_RUNONCE_H
