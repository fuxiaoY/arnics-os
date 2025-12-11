
#ifndef ROLLDEF_H
#define ROLLDEF_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <cstdint>
#include <stdint.h>
#include <stdbool.h>

#define RTOS_MUTEX_ENABLE

#define ROLLDB_LOG_ERROR_ENABLE     

#define ROLLDB_LOG_INFO_ENABLE 

#define ROLLDB_LOG_ALT_ENABLE 

// #define ROLLDB_LOG_DEBUG_ENABLE



#ifndef ROLLDB_PRINTF
#define ROLLDB_PRINTF                     printf
#endif
// debug level log 
#ifdef  log_debug
#undef  log_debug
#endif

#ifdef ROLLDB_LOG_DEBUG_ENABLE
#define log_debug(...)                    ROLLDB_PRINTF("ROLLDB[DEBUG]: "); ROLLDB_PRINTF(__VA_ARGS__);ROLLDB_PRINTF("\r\n")
#else
#define log_debug(...)
#endif

// info level log 
#ifdef  log_info
#undef  log_info
#endif
#ifdef ROLLDB_LOG_INFO_ENABLE
#define log_info(...)                     ROLLDB_PRINTF("ROLLDB[INFO]: ");  ROLLDB_PRINTF(__VA_ARGS__);ROLLDB_PRINTF("\r\n")
#else
#define log_info(...)
#endif

// alt level log 
#ifdef  log_alt
#undef  log_alt
#endif
#ifdef ROLLDB_LOG_ALT_ENABLE
#define log_alt(...)                     ROLLDB_PRINTF("ROLLDB[ALT]:(%s:%d) ", __func__, __LINE__);ROLLDB_PRINTF(__VA_ARGS__);ROLLDB_PRINTF("\r\n")
#else
#define log_alt(...)
#endif


// error level log 
#ifdef  log_error
#undef  log_error
#endif
#ifdef ROLLDB_LOG_ERROR_ENABLE
#define log_error(...)                     ROLLDB_PRINTF("ROLLDB[ERROR]:(%s:%d) ", __func__, __LINE__);ROLLDB_PRINTF(__VA_ARGS__);ROLLDB_PRINTF("\r\n")
#else
#define log_error(...)
#endif



#ifdef __cplusplus
}
#endif


#endif // ROLLDEF_H
