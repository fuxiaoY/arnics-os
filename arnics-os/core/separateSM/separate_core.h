#ifndef SEPERATE_CORE_H
#define SEPERATE_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#define SEPARATE_CORE_VERSION "1.0.1"

// Macro to define a function that gets the current state value
#define SEPARATE_GET_STATE_FUNC(enum_type, func_name) \
    static void* func_name(void* context) {                \
        return (void*) (*((enum_type*)context));           \
    }

// Macro to simplify the declaration of a function that gets the current state
#define SEPARATE_DEFINE_GET_STATE_FUNC(enum_type) SEPARATE_GET_STATE_FUNC(enum_type, SEPARATE_GET_CURRENT_##enum_type##_STATE)

// Macro to initialize a state structure
#define SEPARATE_DEFINE_STATE(enum_value, execute_func) { (void*)(enum_value), execute_func }

// Macro to compare if two state IDs are equal
#define SEPARATE_STATE_ID_COMPARE(state_id, value) (state_id == (void*)(value))

// Definition of the state structure
typedef struct {
    void* stateID;            // State identifier
    void (*execute)();        // Execution function pointer
} SEPARATE_STATE;

// Definition of the callback function type
typedef void (*SEPARATE_Callback)(); 

// Definition of the kernel structure
typedef struct {
    const SEPARATE_STATE* states;  // Pointer to an array of states
    int stateCount;              // Number of elements in the state array
    void* (*getState)(void*);    // Function pointer to get the current state
    void* context;               // Context data
    SEPARATE_Callback onNotFound;  // Callback function when the state is not found
} SEPARATE_KERNEL;

// Macro to initialize the kernel
#define SEPARATE_INIT_KERNEL(pstatelist, inenum_type, incontext, inonNotFound) \
        SEPARATE_DEFINE_GET_STATE_FUNC(inenum_type);                      \
        static SEPARATE_KERNEL kernel = {                                 \
            .states = pstatelist,                                         \
            .stateCount = sizeof(pstatelist) / sizeof(pstatelist[0]),     \
            .getState = SEPARATE_GET_CURRENT_##inenum_type##_STATE,       \
            .context = incontext,                                         \
            .onNotFound = inonNotFound                                    \
        };   

// Function declaration to run the kernel
extern void separate_kernel_loop(SEPARATE_KERNEL* kernel,unsigned char autoLoop) ; 

// Macro to run the kernel
#define SEPARATE_RUN_KERNEL(autoLoop) separate_kernel_loop(&kernel,autoLoop)

#ifdef __cplusplus
}
#endif

#endif // !SEPERATE_CORE_H
