#ifndef INTERFACE_RTC_H
#define INTERFACE_RTC_H
#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/ProjDefine.h"
#include "Inc/typedef.h"
#include "drivers/common/drivers_common.h"

/* function prototypes -----------------------------------------------*/
enum RTC_CTL
{
  RTC_GETDATETIME = 0,
  RTC_SETDATETIME,
  RTC_SECONDUPDATE,
  RTC_ALARM_SET,
  RTC_ALARM_CLEAR,
  RTC_WAKEUP_SET,
  RTC_WAKEUP_CLEAR,
};


extern int rtc_open(device_t *self);
extern int rtc_close(device_t *self);

extern int rtc_ctl(device_t *self, int cmd, va_list ap);
extern void rtc_irq(device_t *self);

#ifdef __cplusplus
}
#endif
#endif /* INTERFACE_RTC_H */
