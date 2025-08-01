#include "../../common/drivers_list.h"
#include "../inc/interface_iicSof.h"
#include "../../../port/delay.h"
#include "../../bsp/bsp_def.h"
#include "gpio.h"

// 读取SDA引脚状态
uint8_t READ_SDA(device_t *self)
{
    iicSof_t *iicSofInstance = (iicSof_t *)self->device;
    return bsp_gpio_get(&(iicSofInstance->SDA_IN));
}

// 初始化主IIC 引脚
void SDA_IN(device_t *self)
{
    iicSof_t *iicSofInstance = (iicSof_t *)self->device;
    bsp_gpio_init(&(iicSofInstance->SDA_IN));
}

void SDA_OUT(device_t *self)
{
    iicSof_t *iicSofInstance = (iicSof_t *)self->device;
    // 修改为开漏输出模式
    bsp_gpio_init(&(iicSofInstance->SDA_OUT)); 
}
// I2C基本输出
void IIC_SCL(device_t *self, uint8_t level)
{
    iicSof_t *iicSofInstance = (iicSof_t *)self->device;
    bsp_gpio_set(&(iicSofInstance->SCL), level);
}

void IIC_SDA(device_t *self, uint8_t level)
{
    iicSof_t *iicSofInstance = (iicSof_t *)self->device;
    bsp_gpio_set(&(iicSofInstance->SDA_OUT), level);
}

int IIC_Init(device_t *self)
{
    iicSof_t *iicSofInstance = (iicSof_t *)self->device;
    bsp_gpio_init(&(iicSofInstance->SCL));
    bsp_gpio_init(&(iicSofInstance->SDA_OUT));

    IIC_SCL(self, 1);
    IIC_SDA(self, 1);
    return 0;
}

// 产生IIC起始信号
void IIC_Start(device_t *self)
{
    SDA_OUT(self); // sda线输出
    IIC_SDA(self, 1);
    IIC_SCL(self, 1);
    Delay_us(4);
    IIC_SDA(self, 0); // START: when CLK is high, DATA change from high to low
    Delay_us(4);
    IIC_SCL(self, 0); // 钳住I2C总线，准备发送或接收数据
}

// 产生IIC停止信号
void IIC_Stop(device_t *self)
{
    SDA_OUT(self); // sda线输出
    IIC_SCL(self, 0);
    IIC_SDA(self, 0); // STOP: when CLK is high DATA change from low to high
    Delay_us(4);
    IIC_SCL(self, 1);
    IIC_SDA(self, 1); // 发送I2C总线结束信号
    Delay_us(4);
}

// 等待应答信号到来
// 返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(device_t *self)
{
    uint8_t ucErrTime = 0;
    SDA_IN(self); // SDA设置为输入
    IIC_SDA(self, 1);
    Delay_us(1);
    IIC_SCL(self, 1);
    Delay_us(1);
    while (READ_SDA(self))
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            IIC_Stop(self);
            return 1;
        }
    }
    IIC_SCL(self, 0); // 时钟输出0
    return 0;
}

// 产生ACK应答
void IIC_Ack(device_t *self)
{
    IIC_SCL(self, 0);
    SDA_OUT(self);
    IIC_SDA(self, 0);
    Delay_us(2);
    IIC_SCL(self, 1);
    Delay_us(2);
    IIC_SCL(self, 0);
}

// 不产生ACK应答
void IIC_NAck(device_t *self)
{
    IIC_SCL(self, 0);
    SDA_OUT(self);
    IIC_SDA(self, 1);
    Delay_us(2);
    IIC_SCL(self, 1);
    Delay_us(2);
    IIC_SCL(self, 0);
}

// IIC发送一个字节
// 返回从机有无应答
// 1，有应答
// 0，无应答
void IIC_Send_Byte(device_t *self,uint8_t txd)
{
    uint8_t t;
    SDA_OUT(self);
    IIC_SCL(self, 0); // 拉低时钟开始数据传输
    for (t = 0; t < 8; t++)
    {
        IIC_SDA(self, (txd & 0x80) >> 7);
        txd <<= 1;
        Delay_us(2); // 对TEA5767这三个延时都是必须的
        IIC_SCL(self, 1);
        Delay_us(2);
        IIC_SCL(self, 0);
        Delay_us(2);
    }
}

// 读1个字节，ack=1时，发送ACK，ack=0，发送nACK
uint8_t IIC_Read_Byte(device_t *self,unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA_IN(self); // SDA设置为输入
    for (i = 0; i < 8; i++)
    {
        IIC_SCL(self, 0);
        Delay_us(2);
        IIC_SCL(self, 1);
        receive <<= 1;
        if (READ_SDA(self))
            receive++;
        Delay_us(1);
    }
    if (!ack)
        IIC_NAck(self); // 发送nACK
    else
        IIC_Ack(self); // 发送ACK
    return receive;
}

int IIC_WaitForIdle(device_t *self,uint32_t TimeOut)
{
    int Ret = 0;
    int LoopCnt = 0;

    // 产生起始信号
    IIC_Start(self);

    // 设置SDA和SCL为高电平
    IIC_SDA(self, 1);
    Delay_us(100);
    IIC_SCL(self, 1);
    Delay_us(100);

    // SDA设置为输入模式
    SDA_IN(self);
    // 等待SDA变为高电平
    while (READ_SDA(self) == 0)
    {
        if (++LoopCnt > TimeOut)
        {
            Ret = -1;
            break;
        }
    }

    // 产生停止信号
    IIC_Stop(self);

    return Ret;
}

void IIC_Send7bitAddress(device_t *self,uint8_t Address, uint8_t Direction)
{
    if (Direction != I2C_Direction_Transmitter)
    {
        Address |= 0x0001;
    }
    else
    {
        Address &= 0xFFFE;
    }
    IIC_Send_Byte(self,Address);
}

int IIC_Write(device_t *self, uint8_t devAddr, uint16_t regAddr, addr_type_e regType, uint8_t *buf, uint16_t len,uint32_t timeout)
{
    int Ret = 0;

    // 步骤1：等待I2C总线恢复空闲
    Ret = IIC_WaitForIdle(self, timeout); //  timeout
    if (Ret != 0)
    {
        return Ret;
    }

    // 步骤2：产生起始信号
    IIC_Start(self);

    // 步骤3：发送寻址，类似于发送广播要求访问指定的EEPROM
    // 发完之后阻塞等待ACK
    IIC_Send7bitAddress(self, devAddr, I2C_Direction_Transmitter); // 发送设备地址，写模式
    if (IIC_Wait_Ack(self))
    {
        IIC_Stop(self);
        return -13;
    }

    // 步骤4：发送寄存器地址
    switch(regType)
    {
        case ADDR_8BIT:
            IIC_Send_Byte(self, (uint8_t)regAddr);
            if (IIC_Wait_Ack(self))
            {
                IIC_Stop(self);
                Ret = -14; 
                return Ret;
            }
        break;
        case ADDR_16BIT:
            IIC_Send_Byte(self, (uint8_t)(regAddr >> 8));
            if (IIC_Wait_Ack(self))
            {
                IIC_Stop(self);
                Ret = -14; 
                return Ret;
            }
            IIC_Send_Byte(self, (uint8_t)regAddr);
            if (IIC_Wait_Ack(self))
            {
                IIC_Stop(self);
                Ret = -14; 
                return Ret;
            }
        break;
    }

    // 步骤5：发送待写入的字节，然后阻塞等待ACK
    for (uint16_t i = 0; i < len; ++i)
    {
        IIC_Send_Byte(self, buf[i]);
        if (IIC_Wait_Ack(self))
        {
            IIC_Stop(self);
            return -15;
        }
    }

    // 步骤6：产生停止信号
    IIC_Stop(self);

    return Ret;
}

int IIC_Read(device_t *self, uint8_t devAddr, uint16_t regAddr, addr_type_e regType, uint8_t *ReadBuf, uint32_t ReadLen, uint32_t TimeOut)
{
    int Ret = 0;

    // 步骤1：等待I2C总线恢复空闲
    Ret = IIC_WaitForIdle(self, TimeOut);
    if (Ret != 0)
    {
        Ret = -1; 
        return Ret;
    }

    // 步骤2：产生第一个起始信号
    IIC_Start(self);

    // 步骤3：向对方地址发出寻址指令
    IIC_Send7bitAddress(self, devAddr, I2C_Direction_Transmitter); // 发送设备地址，写模式
    if (IIC_Wait_Ack(self) != 0)
    {
        IIC_Stop(self);
        Ret = -13; 
        return Ret;
    }

    // 步骤4：发送寄存器地址
    switch(regType)
    {
        case ADDR_8BIT:
            IIC_Send_Byte(self, (uint8_t)regAddr);
            if (IIC_Wait_Ack(self))
            {
                IIC_Stop(self);
                Ret = -14; 
                return Ret;
            }
        break;
        case ADDR_16BIT:
            IIC_Send_Byte(self, (uint8_t)(regAddr >> 8));
            if (IIC_Wait_Ack(self))
            {
                IIC_Stop(self);
                Ret = -14; 
                return Ret;
            }
            IIC_Send_Byte(self, (uint8_t)regAddr);
            if (IIC_Wait_Ack(self))
            {
                IIC_Stop(self);
                Ret = -14; 
                return Ret;
            }
        break;
    }

    // 步骤5：产生第二个起始信号
    IIC_Start(self);

    // 步骤6：向对方地址发出读取指令
    IIC_Send7bitAddress(self, devAddr, I2C_Direction_Receiver); // 发送设备地址，读模式
    if (IIC_Wait_Ack(self) != 0)
    {
        IIC_Stop(self);
        Ret = -22; 
        return Ret;
    }

    // 步骤7：读取数据
    for (uint32_t i = 0; i < ReadLen; ++i)
    {
        if (i == ReadLen - 1)
        {
            ReadBuf[i] = IIC_Read_Byte(self, 0); // 最后一个字节读取后发送NACK
        }
        else
        {
            ReadBuf[i] = IIC_Read_Byte(self, 1); // 读取数据并发送ACK
        }
    }

    // 步骤8：产生停止信号
    IIC_Stop(self);

    return Ret; // 返回成功状态码 0
}


int iic_open(device_t *self)
{
    return IIC_Init(self);
}

int iic_close(device_t *self)
{
    iicSof_t *iicSofInstance = (iicSof_t *)self->device;
    bsp_gpio_close(&(iicSofInstance->SCL));
    bsp_gpio_close(&(iicSofInstance->SDA_OUT));
    return 0;
}

int iic_ctl(device_t *self, int cmd,va_list ap)
{
    uint8_t devAddr = (uint8_t)va_arg(ap, int);
    uint16_t regAddr = (uint16_t)va_arg(ap, int);

    if(self->ds == 0)
    {
        return -1;
    }

    switch(cmd)
    {
        case IIC_WRITE:
        {
            uint8_t *Writedata = (uint8_t*)va_arg(ap, int);
            uint32_t WriteLen = (uint32_t)va_arg(ap, int); 
            uint32_t TimeOut = (uint32_t)va_arg(ap, int); 
            return IIC_Write(self,devAddr,regAddr,ADDR_8BIT,Writedata,WriteLen,TimeOut);
        }
        case IIC_READ:
        {
            uint8_t* ReadBuf = va_arg(ap, uint8_t*);
            uint32_t ReadLen = (uint32_t)va_arg(ap, int); 
            uint32_t TimeOut = (uint32_t)va_arg(ap, int); 
            return IIC_Read(self,devAddr,regAddr,ADDR_8BIT,ReadBuf,ReadLen,TimeOut);   
        }
        case IIC_WRITE16:
        {
            uint8_t *Writedata = (uint8_t*)va_arg(ap, int);
            uint32_t WriteLen = (uint32_t)va_arg(ap, int); 
            uint32_t TimeOut = (uint32_t)va_arg(ap, int); 
            return IIC_Write(self,devAddr,regAddr,ADDR_16BIT,Writedata,WriteLen,TimeOut);
        }
        case IIC_READ16:
        {
            uint8_t* ReadBuf = va_arg(ap, uint8_t*);
            uint32_t ReadLen = (uint32_t)va_arg(ap, int); 
            uint32_t TimeOut = (uint32_t)va_arg(ap, int); 
            return IIC_Read(self,devAddr,regAddr,ADDR_16BIT,ReadBuf,ReadLen,TimeOut);   
        }
        default: return -1;

    }
}

