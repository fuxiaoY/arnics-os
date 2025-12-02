#ifndef INTERFACE_SPI_H
#define INTERFACE_SPI_H
#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/ProjDefine.h"
#include "Inc/typedef.h"
#include "drivers/common/drivers_common.h"

/* function prototypes -----------------------------------------------*/
enum SPI_CTL
{
  SPI_GETSATATE = 0,
};


extern int spi_open(device_t *self);
extern int spi_close(device_t *self);
extern int spi_read(device_t *self,void *buf, size_t count);
extern int spi_write(device_t *self, void *buf, size_t count);
extern int spi_ctl(device_t *self, int cmd,va_list ap);


#ifdef __cplusplus
}
#endif
#endif /* INTERFACE_SPI_H */
