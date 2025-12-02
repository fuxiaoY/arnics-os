#ifndef INTERFACE_FLASH_H
#define INTERFACE_FLASH_H
#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/ProjDefine.h"
#include "Inc/typedef.h"
#include "drivers/common/drivers_common.h"
/* function prototypes -----------------------------------------------*/
enum FLASH_CTL
{
  FLASH_EREASE = 0,
};

extern int flash_open(device_t *self);
extern int flash_close(device_t *self);
extern int flash_read(device_t *self,void *buf, size_t count);
extern int flash_write(device_t *self, void *buf, size_t count);
extern int flash_ctl(device_t *self, int cmd, va_list ap);




#ifdef __cplusplus
}
#endif
#endif /* INTERFACE_flash_H */
