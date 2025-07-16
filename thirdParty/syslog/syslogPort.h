#ifndef SYSLOG_PORT_H
#define SYSLOG_PORT_H

#include "Inc/ProjDefine.h"
#include "Inc/typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void syslog_port_print(const char *msg);

extern void syslog_port_get_timestamp(char *buf, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif // SYSLOG_PORT_H
