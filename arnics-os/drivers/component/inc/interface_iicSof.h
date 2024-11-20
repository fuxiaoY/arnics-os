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


//iic基本输出
extern void IIC_SCL(device_t *self,uint8_t level);
extern void IIC_SDA(device_t *self,uint8_t level);
extern uint8_t READ_SDA(device_t *self);

//初始化主IIC 引脚初始化 
extern void SDA_IN(device_t *self);
extern void SDA_OUT(device_t *self);

//IIC所有操作函数
extern void IIC_Init(device_t *self);                           //初始化IIC的IO口	
extern void IIC_Start(device_t *self);				            //发送IIC开始信号
extern void IIC_Stop(device_t *self);	  			            //发送IIC停止信号
extern void IIC_Send_Byte(device_t *self,uint8_t txd);			//IIC发送一个字节
extern uint8_t IIC_Read_Byte(device_t *self,unsigned char ack); //IIC读取一个字节
extern uint8_t IIC_Wait_Ack(device_t *self); 		            //IIC等待ACK信号
extern void IIC_Ack(device_t *self);					        //IIC发送ACK信号
extern void IIC_NAck(device_t *self);				            //IIC不发送ACK信号

extern int IIC_WaitForIdle(device_t *self,uint32_t TimeOut);
extern void IIC_Send7bitAddress(device_t *self,uint8_t Address, uint8_t Direction);
extern int IIC_ByteWrite(device_t *self,uint8_t devAddr, uint8_t regAddr, uint8_t data, uint32_t TimeOut);
extern int IIC_ByteRead(device_t *self,uint8_t devAddr, uint8_t regAddr, uint32_t TimeOut);
extern int IIC_BufferRead(device_t *self,uint8_t devAddr, uint8_t regAddr, uint8_t RWAddrSize, uint8_t *ReadBuf, uint32_t ReadLen, uint32_t TimeOut);




#ifdef __cplusplus
}
#endif

#endif
