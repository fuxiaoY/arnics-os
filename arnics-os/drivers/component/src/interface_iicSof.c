#include "../inc/interface_iicSof.h"
#include "../../../port/delay.h"
#include "../../bsp/bsp_def.h"
#include "gpio.h"

// 读取SDA引脚状态
uint8_t READ_SDA(device_t *self)
{
    iicSof_t *iicSofInstance = (iicSof_t *)self->device;
    return bsp_gpio_get(&iicSofInstance->SDA_IN);
}

// 初始化主IIC 引脚
void SDA_IN(device_t *self)
{
    iicSof_t *iicSofInstance = (iicSof_t *)self->device;
    bsp_gpio_init(&iicSofInstance->SDA_IN);
}

void SDA_OUT(device_t *self)
{
    iicSof_t *iicSofInstance = (iicSof_t *)self->device;
    // 修改为开漏输出模式
    bsp_gpio_init(&iicSofInstance->SDA_OUT); 
}
// I2C基本输出
void IIC_SCL(device_t *self, uint8_t level)
{
    iicSof_t *iicSofInstance = (iicSof_t *)self->device;
    bsp_gpio_set(&iicSofInstance->SCL, level);
}

void IIC_SDA(device_t *self, uint8_t level)
{
    iicSof_t *iicSofInstance = (iicSof_t *)self->device;
    bsp_gpio_set(&iicSofInstance->SDA_OUT, level);
}

void IIC_Init(device_t *self)
{
    iicSof_t *iicSofInstance = (iicSof_t *)self->device;
    bsp_gpio_init(&iicSofInstance->SCL);
    bsp_gpio_init(&iicSofInstance->SDA_OUT);

    IIC_SCL(self, 1);
    IIC_SDA(self, 1);
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

int IIC_ByteWrite(device_t *self,uint8_t devAddr, uint8_t regAddr, uint8_t data, uint32_t TimeOut)
{
    int Ret = 0;

    // 步骤1：等待I2C总线恢复空闲
    Ret = IIC_WaitForIdle(self,TimeOut);
    if (Ret != 0)
    {
        return Ret;
    }

    // 步骤2：产生起始信号
    IIC_Start(self);

    // 步骤3：发送寻址，类似于发送广播要求访问指定的EEPROM
    // 发完之后阻塞等待ACK
    IIC_Send7bitAddress(self,devAddr, I2C_Direction_Transmitter); // 发送设备地址，写模式
    if (IIC_Wait_Ack(self))
    {
        IIC_Stop(self);
        return -13;
    }

    // 步骤4：发送存储地址（或者应该理解为EEPROM芯片的寄存器地址）
    // 发完之后阻塞等待ACK
    // 对于不同的EEPROM芯片，存储地址字长不同，对于多字节来表示地址的芯片，需要按大端字序依次发送地址值的每一个字节
    // 例如AT24C1024，地址字长为3字节，访问0x012345地址，则需要依次发送0x01，0x23，0x45，每发一个字节都要等一次ACK
    IIC_Send_Byte(self,regAddr);
    if (IIC_Wait_Ack(self))
    {
        IIC_Stop(self);
        return -14;
    }

    // 步骤5：发送待写入的字节，然后阻塞等待ACK
    IIC_Send_Byte(self,data);
    if (IIC_Wait_Ack(self))
    {
        IIC_Stop(self);
        return -15;
    }

    // 步骤6：产生停止信号
    IIC_Stop(self);

    return Ret;
}

int IIC_ByteRead(device_t *self,uint8_t devAddr, uint8_t regAddr, uint32_t TimeOut)
{
    uint8_t Ret = 0;
    uint8_t ReadChr = 0;

    // 步骤1：等待I2C总线恢复空闲
    Ret = IIC_WaitForIdle(self,TimeOut);
    if (Ret != 0)
    {
        return Ret;
    }

    // 步骤2：产生第一个起始信号
    IIC_Start(self);

    // 步骤3：向对方地址发出寻址指令，参数2填的是对方的设备地址
    // 发完之后阻塞等待ACK
    IIC_Send7bitAddress(self,devAddr, I2C_Direction_Transmitter); // 发送设备地址，写模式
    if (IIC_Wait_Ack(self))
    {
        IIC_Stop(self);
        return -13;
    }

    // 步骤4：发送数据，内容为数据存入对方存储空间中的存储地址（或者应该理解为EEPROM芯片的寄存器地址）
    // 发完之后阻塞等待ACK
    // 对于不同的EEPROM芯片，存储地址字长不同，对于多字节来表示地址的芯片，需要按大端字序依次发送地址值的每一个字节
    // 例如AT24C1024，地址字长为3字节，访问0x012345地址，则需要依次发送0x01，0x23，0x45，每发一个字节都要等一次ACK
    IIC_Send_Byte(self,regAddr);
    if (IIC_Wait_Ack(self))
    {
        IIC_Stop(self);
        return -14;
    }

    // 步骤5：产生第二个起始信号
    IIC_Start(self);

    // 步骤6：向对方地址发出读取指令，参数2填的是对方的设备地址
    // 发完之后阻塞等待ACK
    IIC_Send_Byte(self,(devAddr | 0x01)); // 读取时需要将地址的最低位设为1
    if (IIC_Wait_Ack(self))
    {
        IIC_Stop(self);
        return -22;
    }

    // 步骤7：收取应答并回NACK让对方结束
    ReadChr = IIC_Read_Byte(self,false); // 不发送ACK

    // 步骤8：产生停止信号
    IIC_Stop(self);

    return ReadChr;
}

// I2CGS_Master_BufferRead函数实现
int IIC_BufferRead(device_t *self,uint8_t devAddr, uint8_t regAddr, uint8_t RWAddrSize, uint8_t *ReadBuf, uint32_t ReadLen, uint32_t TimeOut)
{
    int Ret = 0;

    // 步骤1：等待I2C总线恢复空闲
    Ret = IIC_WaitForIdle(self,TimeOut);
    if (Ret != 0)
        return Ret;

    // 步骤2：产生第一个起始信号
    IIC_Start(self);

    // 步骤3：向对方地址发出寻址指令
    IIC_Send7bitAddress(self,devAddr, I2C_Direction_Transmitter); // 发送设备地址，写模式
    if (IIC_Wait_Ack(self) != 0)
    {
        IIC_Stop(self);
        return -13;
    }

    // 步骤4：发送寄存器地址
    IIC_Send_Byte(self,regAddr);
    if (IIC_Wait_Ack(self))
    {
        IIC_Stop(self);
        return -14;
    }

    // 步骤5：产生第二个起始信号
    IIC_Start(self);

    // 步骤6：向对方地址发出读取指令，参数2填的是对方的设备地址
    IIC_Send7bitAddress(self,devAddr, I2C_Direction_Receiver); // 发送设备地址，读模式
    if (IIC_Wait_Ack(self) != 0)
    {
        IIC_Stop(self);
        return -22;
    }
    // 步骤7：读取数据
    for (uint32_t i = 0; i < ReadLen - 1; ++i)
    {
        ReadBuf[i] = IIC_Read_Byte(self,1); // 读取数据并发送ACK
    }

    // 最后一个字节读取后发送NACK
    ReadBuf[ReadLen - 1] = IIC_Read_Byte(self,0);

    // 步骤8：产生停止信号
    IIC_Stop(self);

    return Ret;
}
