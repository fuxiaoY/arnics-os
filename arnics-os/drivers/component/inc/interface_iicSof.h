#ifndef _INTERFACE_IICSOF_H
#define _INTERFACE_IICSOF_H
#ifdef __cplusplus
extern "C" {
#endif
#include "../../../Inc/ProjDefine.h"
#include "../../../Inc/typedef.h"
#include "../../common/drivers_common.h"

//I2C方向，将决定发送的地址的bit0
#ifndef I2C_Direction_Transmitter
#define I2C_Direction_Transmitter      0
#endif

#ifndef I2C_Direction_Receiver
#define I2C_Direction_Receiver         1
#endif


//IIC所有操作函数
extern int IIC_Init(device_t *self);                           //初始化IIC的IO口	
extern int IIC_ByteWrite(device_t *self,uint8_t devAddr, uint8_t regAddr, uint8_t data, uint32_t TimeOut);
extern int IIC_ByteRead(device_t *self,uint8_t devAddr, uint8_t regAddr, uint32_t TimeOut);
extern int IIC_BufferRead(device_t *self,uint8_t devAddr, uint8_t regAddr, uint8_t *ReadBuf, uint32_t ReadLen, uint32_t TimeOut);




#ifdef __cplusplus
}
#endif

#endif
