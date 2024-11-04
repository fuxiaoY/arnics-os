#ifndef INTERFACE_IWDG_H
#define INTERFACE_IWDG_H
#ifdef __cplusplus
extern "C" {
#endif
#include "../../../Inc/ProjDefine.h"
#include "../../../Inc/typedef.h"
#include "../../common/drivers_common.h"

/* function prototypes -----------------------------------------------*/
enum IWDG_CTL
{
  IWDG_RELOAD = 0,
};

extern int iwdg_open(device_t *self);
extern int iwdg_close(device_t *self);
extern int iwdg_ctl(device_t *self, int cmd, void *args);


#ifdef __cplusplus
}
#endif
#endif /* INTERFACE_IWDG_H */
