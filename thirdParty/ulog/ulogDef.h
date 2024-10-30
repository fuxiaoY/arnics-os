#ifndef _ULOGDEF_H_
#define _ULOGDEF_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "ulog.h"

extern void usr_ulog_level_change(ulog_level_t level);
extern void usr_ulog_init(void);
extern void usr_ulog_deinit(void);
#ifdef __cplusplus
}
#endif
#endif

