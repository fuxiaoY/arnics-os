#ifndef _UFLOG_CFG_H_
#define _UFLOG_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include "common/commonInclude.h"

/** @defgroup uflog_constants UFLOG Constants
 * @brief UFLOG库使用的常量定义 / Constant definitions used by UFLOG library
 * @{
 */

#define UFLOG_STATIC_MEMORY

/** @brief UFLOG日志自定义消息区最大长度 / Maximum length of a single user message */
#define UFLOG_USER_MSG_MAX_LEN                      512

/** @brief UFLOG存储缓存大小 / UFLOG storage cache size */
#define UFLOG_STORE_CACHE                           256

/** @brief UFLOG日志前缀最大长度 / Maximum length of UFLOG log prefix */
#define UFLOG_MAX_PREFIX_LEN                        40

/** @brief UFLOG消息结构体最大长度 / Maximum length of UFLOG message structure */
#define UFLOG_MSG_STRUCT_LEN                        128

/** @brief UFLOG函数名最大长度 / Maximum length of a single function name */
#define UFLOG_FUN_LINE_LEN                          128


/** @defgroup uflog_memory UFLOG Memory Management
 * @brief UFLOG内存管理相关定义 / UFLOG memory management related definitions
 * @{
 */


#ifndef UFLOG_STATIC_MEMORY
/** @brief UFLOG 内存分配函数映射 / UFLOG memory allocation function mapping
 *  
 */
#define uflog_malloc                               arnicsMalloc

/** @brief UFLOG 内存释放函数映射 / UFLOG memory release function mapping
 *  
 */
#define uflog_free                                 arnicsFree
#endif

/** @} */
   


#ifdef __cplusplus  
}
#endif

#endif 
