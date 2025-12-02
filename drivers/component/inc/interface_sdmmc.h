#ifndef INTERFACE_SDMMC_H
#define INTERFACE_SDMMC_H
#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/ProjDefine.h"
#include "Inc/typedef.h"
#include "drivers/common/drivers_common.h"

/* function prototypes -----------------------------------------------*/
enum SDMMC_CTL
{
    SDMMC_READ = 0,
    SDMMC_WRITE,
    SDMMC_ERASE,
    SDMMC_GET_CARD_INFO,
    SDMMC_GET_CARD_STATUS,
};


extern int sdmmc_open(device_t *self);
extern int sdmmc_close(device_t *self);
extern int sdmmc_ctl(device_t *self, int cmd, va_list ap);


#ifdef __cplusplus
}
#endif
#endif /* INTERFACE_SDMMC_H */
