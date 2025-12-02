#ifndef _INTERFACE_IICSOF_H
#define _INTERFACE_IICSOF_H
#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/ProjDefine.h"
#include "Inc/typedef.h"
#include "drivers/common/drivers_common.h"

//I2C方向，将决定发送的地址的bit0
#ifndef I2C_Direction_Transmitter
#define I2C_Direction_Transmitter      0
#endif

#ifndef I2C_Direction_Receiver
#define I2C_Direction_Receiver         1
#endif
/* function prototypes -----------------------------------------------*/
enum IIC_CTL
{
  IIC_WRITE = 0,
  IIC_READ,
  IIC_WRITE16,
  IIC_READ16,
};

typedef enum 
{
  ADDR_8BIT,
  ADDR_16BIT,
}addr_type_e;

extern int iic_open(device_t *self);
extern int iic_close(device_t *self);
extern int iic_ctl(device_t *self, int cmd,va_list ap);


#ifdef __cplusplus
}
#endif

#endif
