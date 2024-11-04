#ifndef INTERFACE_RTC_H
#define INTERFACE_RTC_H
#ifdef __cplusplus
extern "C" {
#endif
#include "../../../Inc/ProjDefine.h"
#include "../../../Inc/typedef.h"
#include "../../common/drivers_common.h"

/* function prototypes -----------------------------------------------*/
enum RTC_CTL
{
  RTC_GETDATETIME = 0,
  RTC_SETDATETIME = 1,
};


extern int rtc_open(device_t *self);
extern int rtc_close(device_t *self);

extern int rtc_ctl(device_t *self, int cmd, void *args);


#ifdef __cplusplus
}
#endif
#endif /* INTERFACE_RTC_H */
