/* information */
/**
  ******************************************************************************
  * @file           : command_modbus.c
  * @brief          : modbus协议实现
  * 
  * 该文件适用于mct框架下modbus协议的从机模式。
  * 
  * @version        : 1.0.0
  * @date           : 2025-04-09
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 ARSTUDIO.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "command_modbus.h"
#include "../../common/mctLib.h"
#include "../../../AlgorithmLib/crc16.h"

/*-----------------------------------------------------------------------------*/

/*-------------------------------Slave Mode------------------------------------*/



/* typedef --------------------------------------------------------------------*/
#define SLAVE_ADDRESS 0x01 // 从机地址

/* function prototypes --------------------------------------------------------*/
/**
 * @brief 线圈与寄存器接口函数
 * @note 用户实现的读写函数，实际应用中需要替换
 */
static uint16_t testReg_1 = 0x1234;
static uint16_t testReg_2 = 0x5678;
static bool testCoil_1 = false;
uint16_t read_register_1() 
{
    return testReg_1;
}

void write_register_1(uint16_t value) 
{
    testReg_1 = value;
}
uint16_t read_register_2() 
{
    return testReg_2;
}

void write_register_2(uint16_t value) 
{
    testReg_2 = value;
}

bool read_coil_1() 
{
    return testCoil_1;
}

void write_coil_1(bool value) 
{
    testCoil_1 = value;
}
/**
 * @brief 线圈与寄存器注册列表
 */
static ModbusRegister registers[] = {
    {0x0000, read_register_1, write_register_1},
    {0x0001, read_register_2, write_register_2},

};

static ModbusCoil coils[] = {
    {0x0001, read_coil_1, write_coil_1},


};
static uint16_t registersNumGet(void)
{
    return (sizeof(registers) / sizeof(ModbusRegister));
}
static uint16_t CoilsNumGet(void)
{
    return (sizeof(coils) / sizeof(ModbusCoil));
}

/**
 * @brief 读写线圈与寄存器接口
 */
bool modbus_read_register(uint16_t address,uint16_t *value) 
{
    for (uint16_t i = 0; i < registersNumGet(); i++) 
    {
        if (registers[i].address == address) 
        {
            if(NULL != registers[i].read)
            {
                *value = registers[i].read();
                return true;
            }
            else
            {
                return false;
            }

        }
    }
    return false; // 地址未找到
}
bool modbus_read_coil(uint16_t address,bool *value) 
{
    for (uint16_t i = 0; i < CoilsNumGet(); i++) 
    {
        if (coils[i].address == address) 
        {
            if(NULL != coils[i].read)
            {
                *value = coils[i].read();
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return false;  // 地址未找到
}

bool modbus_write_register(uint16_t address, uint16_t value) 
{
    for (uint16_t i = 0; i < registersNumGet(); i++) 
    {
        if (registers[i].address == address) 
        {
            if(NULL != registers[i].write)
            {
                registers[i].write(value);
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return false; // 地址未找到
}

bool modbus_write_coil(uint16_t address, bool value) 
{
    for (uint16_t i = 0; i < CoilsNumGet(); i++) 
    {
        if (coils[i].address == address) 
        {
            if(NULL != coils[i].write)
            {
                coils[i].write(value);
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return false; // 地址未找到
}

/**
 * @brief 基础校验函数
 */

static bool modbus_ValidateLen(uint8_t* buf, size_t len)
{
    // 基础长度校验（地址+功能码+CRC）
    return (len >= 4);
}
static bool modbus_ValidateSlaveAddress(uint8_t addr,ModbusFrame *frame)
{
    // 地址校验
    return (addr == ADDRESS_BROADCAST) ||  (addr == frame->addr);
}
static bool modbus_ValidateCRC(uint8_t* buf, size_t len)
{
    //CRC16校验
    uint16_t crc_received = (buf[len-1] << 8) | buf[len-2]; // 小端模式
    uint16_t crc_calc = DL_CRC16(buf, len-2);
    if (crc_calc == crc_received)
    {
        return true;
    }
    else
    {
        printf("CRC error: expected 0x%04X\n", crc_calc);
        return false;
    }
}
static bool modbus_ValidateFunctionCode(uint8_t buf, ModbusFrame* frame)
{
    // 功能码校验
    return (buf == frame->func_code);
}

/** 
 * @brief 处理Modbus 01功能码（读单线圈）
 * @note 支持01功能码，线圈寻址范围0x0000-0x007D(根据mct buffer大小限制)
 */
static bool modbus_Handle01_ReadCoil(uint8_t* buf, ModbusFrame* frame)
{
    // 解析线圈地址（2字节）
    frame->fun_params.read_coil_01.coil_addr = (buf[2] << 8) | buf[3];
    frame->fun_params.read_reg_03.num = (buf[4] << 8) | buf[5];
    return (frame->fun_params.read_reg_03.num >= 0x0000 && frame->fun_params.read_reg_03.num <= 0x007D); //小于等于125个线圈
}
static bool modbus_Handle01_ReadCoil_pack(uint8_t* buf, size_t *len,ModbusFrame* frame)
{

    size_t* length = len;
    //打包线圈字节数量
    *buf++ = (frame->fun_params.read_coil_01.num + 7) / 8; // 每个线圈1位，向上取整
    *length+= 1;

    //打包线圈状态
    bool value = false;
    // 初始化字节和位位置
    uint8_t coil_byte = 0;
    uint8_t bit_position = 0;
    for (uint16_t i = 0; i < frame->fun_params.read_coil_01.num; i++) 
    {
        if(!modbus_read_coil(frame->fun_params.read_coil_01.coil_addr + i, &value))
        {
            *length = 0;
            return false;
        }
        // 将线圈状态打包到字节中
        if (value)
        {
            coil_byte |= (1 << bit_position);
        }

        bit_position++;
        // 如果已经处理了8个线圈，写入当前字节并重置
        if (bit_position == 8)
        {
            *buf++ = coil_byte;
            *length += 1;
            coil_byte = 0;
            bit_position = 0;
        }
    }
    // 如果还有剩余的线圈状态未写入，写入最后一个字节
    if (bit_position > 0)
    {
        *buf++ = coil_byte;
        *length += 1;
    }
    return true;
}
/**
 * @brief 处理Modbus 03功能码（读保持寄存器）
 * @retval true 解析成功且寄存器数量合法（1-125）
 * @note 支持03功能码，寄存器寻址范围0x0000-0x007D (根据mct buffer大小限制)
 */
static bool modbus_Handle03_ReadReg(uint8_t* buf, ModbusFrame* frame)
{
    // 处理03/04功能码（读保持/输入寄存器）
    frame->fun_params.read_reg_03.start_addr = (buf[2] << 8) | buf[3];
    frame->fun_params.read_reg_03.num = (buf[4] << 8) | buf[5];
    return (frame->fun_params.read_reg_03.num >= 0x0000 && frame->fun_params.read_reg_03.num <= 0x007D); //小于等于125个寄存器
}

static bool modbus_Handle03_ReadReg_pack(uint8_t* buf, size_t *len,ModbusFrame* frame)
{
    uint16_t value = 0;
    size_t* length = len;
    //添加数量
    *buf++ = frame->fun_params.read_reg_03.num * 2; // 每个寄存器2字节 
    *length+= 1;

    //添加数据
    for (uint16_t i = 0; i < frame->fun_params.read_reg_03.num; i++) 
    {
        uint16_t address = frame->fun_params.read_reg_03.start_addr + i;
   
        if(modbus_read_register(address,&value))
        {
            // 将值放入响应缓冲区
            *buf++ = (value >> 8) & 0xFF; // 高字节
            *buf++ = value & 0xFF; // 低字节
            *length+=2;
        }
        else
        {
            *length = 0;
            // 读取寄存器失败，返回错误
            return false;
        }
    }
    return true;
}

/**
 * @brief 处理Modbus 05功能码（写单线圈）
 * @retval true 解析成功且强制值合法（0xFF00/0x0000）
 * @retval false 解析失败或数据值非法
 * @note 线圈状态转换规则：
 *       - 0xFF00 -> 线圈ON (0xFF)
 *       - 0x0000 -> 线圈OFF (0x00)
 */
static bool modbus_Handle05_writeCoil(uint8_t* buf, ModbusFrame* frame)
{
    // 解析线圈地址（2字节）
    frame->fun_params.write_coil_05.coil_addr = (buf[2] << 8) | buf[3];
    
    // 解析强制数据（2字节）
    uint16_t force_value = (buf[4] << 8) | buf[5];
    
    // 校验数据值（必须为0xFF00或0x0000）
    if((force_value != 0xFF00) && (force_value != 0x0000)) 
    {
        return false;
    }
    
    // 转换为布尔状态（0x00或0xFF）
    frame->fun_params.write_coil_05.coil_state = (force_value == 0xFF00) ? 0xFF : 0x00;
    
    return true;
}
static bool modbus_Handle05_writeCoil_pack(uint8_t* buf, size_t *len,ModbusFrame* frame)
{
    bool value = false;
    size_t* length = len;
    //打包线圈地址
    *buf++ = frame ->fun_params.write_coil_05.coil_addr >> 8; // 高字节
    *buf++ = frame ->fun_params.write_coil_05.coil_addr & 0xFF; // 低字节
    *length+= 2;
    // 写线圈状态
    if(!modbus_write_coil(frame->fun_params.write_coil_05.coil_addr, frame->fun_params.write_coil_05.coil_state))
    {
        *length = 0;
        return false;
    }
    // 读取并打包线圈状态
    if(!modbus_read_coil(frame->fun_params.write_coil_05.coil_addr, &value))
    {
        *length = 0;
        return false;
    }
    if(value)
    {
        *buf++ = 0xFF; //线圈ON
        *buf++ = 0x00;
        *length+= 2;
    }
    else
    {
        *buf++ = 0x00; //线圈OFF
        *buf++ = 0x00;
        *length+= 2;
    }
    return true;
}
/**
 * @brief 处理Modbus 06功能码（写单个寄存器）
 * @note 实际数据有效性需由应用层验证：
 *       - reg_value指向原始数据区（2字节）
 *       - 调用者需确保数据区有效性
 */
static bool modbus_Handle06_writeSingleReg(uint8_t* buf, ModbusFrame* frame)
{
    // 处理05/06功能码（写单线圈/寄存器）
    frame->fun_params.write_reg_06.reg_addr = (buf[2] << 8) | buf[3];
    frame->fun_params.write_reg_06.reg_value = (buf[4] << 8) | buf[5]; // 指向数据区
    return true;
}

static bool modbus_Handle06_writeSingleReg_pack(uint8_t* buf, size_t *len,ModbusFrame* frame)
{
    size_t* length = len;
    //打包寄存器地址
    *buf++ = frame ->fun_params.write_reg_06.reg_addr >> 8; // 高字节
    *buf++ = frame ->fun_params.write_reg_06.reg_addr & 0xFF; // 低字节
    *length+= 2;
    // 写寄存器值
    if(!modbus_write_register(frame->fun_params.write_reg_06.reg_addr, frame->fun_params.write_reg_06.reg_value))
    {
        *length = 0;
        return false;
    }
    // 读取并打包寄存器值
    uint16_t value = 0;
    if(!modbus_read_register(frame->fun_params.write_reg_06.reg_addr, &value))
    {
        *length = 0;
        return false;
    }
    else
    {
        *buf++ = (value >> 8) & 0xFF; // 高字节
        *buf++ = value & 0xFF; // 低字节
        *length+= 2;
    }
    return true;
}
/**
 * @brief mct接口函数
 */
static bool cmd_PackSlaveFow(uint8_t* buf, size_t* len, void *para)
{
    ModbusFrame *frame = (ModbusFrame*)para;
    uint8_t *ptr = (uint8_t *)buf;
    size_t pack_len = 0;
    size_t frameLen = 0;
    // 写入从机地址
    *ptr++ = SLAVE_ADDRESS;    pack_len++;
    // 写入功能码
    *ptr++ = frame->func_code;  pack_len++;


    // 功能码分发
    switch(frame->func_code) 
    {
        case FUNC_READ_COIL:
        {
            if(!modbus_Handle01_ReadCoil_pack(ptr,&frameLen,frame))
            {
                // 读取寄存器失败，返回错误
                *len  = 0;
                return false;
            }
        }
        break;
        case FUNC_READ_HOLDING_REGISTER:
        {
                if(!modbus_Handle03_ReadReg_pack(ptr,&frameLen,frame))
                {
                // 读取寄存器失败，返回错误
                *len  = 0;
                return false;
                }
                
        }
        break;
        case FUNC_WRITE_COIL:  
        {
            if(!modbus_Handle05_writeCoil_pack(ptr,&frameLen,frame))
            {
                // 读取寄存器失败，返回错误
                *len  = 0;
                return false;
            }
        }
        break;
        case FUNC_WRITE_HOLDING_REGISTER:
        {
            if(!modbus_Handle06_writeSingleReg_pack(ptr,&frameLen,frame))
            {
                // 读取寄存器失败，返回错误
                *len  = 0;
                return false;
            }
        }
        break;
        default:
        {
        // 其他功能码不支持打包
        *len  = 0;
        return false;
        }
    }
    ptr += frameLen; // 更新指针位置
    pack_len += frameLen;

    // 计算CRC16
    uint16_t crc = DL_CRC16(buf, pack_len);
    // 写入CRC16
    *ptr++ = crc & 0xFF;  pack_len++;
    *ptr++ = crc >> 8;    pack_len++;
    *len = pack_len;


    if(frame->addr == ADDRESS_BROADCAST)
    {
        //广播地址不能回复，防止总线冲突
        *len = 0;
    }
    return true;
}

static bool cmd_AnalyzeSlaveFow(uint8_t* buf, size_t len, void *para)
{
    ModbusFrame *frame = (ModbusFrame*)para;
    frame->addr = SLAVE_ADDRESS;
    // 基础校验
    if(!modbus_ValidateLen(buf, len) || 
       !modbus_ValidateSlaveAddress(buf[0],frame) ||
       !modbus_ValidateCRC(buf, len)) 
    {
        return false;
    }

    frame->func_code = buf[1];

    // 功能码分发
    switch(frame->func_code) 
    {
        case FUNC_READ_COIL:
            return modbus_Handle01_ReadCoil(buf, frame);
        case FUNC_READ_HOLDING_REGISTER:
            return modbus_Handle03_ReadReg(buf, frame);

        case FUNC_WRITE_COIL:  
            return modbus_Handle05_writeCoil(buf, frame);
            
        case FUNC_WRITE_HOLDING_REGISTER:
            return modbus_Handle06_writeSingleReg(buf, frame);

        default:
            return false;
    }

    return false;
}
/*-----------------------------------------------------------------------------*/

/*-------------------------------Master Mode------------------------------------*/
/**
 * @brief Modbus功能码定义
 * @note 01:读线圈 03:读保持寄存器 05:写单个线圈 06:写单个寄存器
 */
static bool master_modbus_handle01_readCoil(uint8_t* buf, size_t *len,ModbusFrame* frame)
{
    // 打包线圈地址（2字节）
    *buf++ = frame->fun_params.read_coil_01.coil_addr >> 8; // 高字节
    *buf++ = frame->fun_params.read_coil_01.coil_addr & 0xFF; // 低字节
    // 打包线圈数量（2字节）
    *buf++ = frame->fun_params.read_reg_03.num >> 8; // 高字节
    *buf++ = frame->fun_params.read_reg_03.num & 0xFF; // 低字节
    *len += 4; // 更新长度
    return true;
}
static bool master_modbus_handle03_readReg(uint8_t* buf, size_t *len,ModbusFrame* frame)
{
    // 打包寄存器地址（2字节）
    *buf++ = frame->fun_params.read_reg_03.start_addr >> 8; // 高字节
    *buf++ = frame->fun_params.read_reg_03.start_addr & 0xFF; // 低字节
    // 打包寄存器数量（2字节）
    *buf++ = frame->fun_params.read_reg_03.num >> 8; // 高字节
    *buf++ = frame->fun_params.read_reg_03.num & 0xFF; // 低字节
    *len += 4; // 更新长度
    return true;
}
static bool mmaster_modbus_handle05_writeCoil(uint8_t* buf,size_t *len, ModbusFrame* frame)
{
    // 打包线圈地址（2字节）
    *buf++ = frame->fun_params.write_coil_05.coil_addr >> 8; // 高字节
    *buf++ = frame->fun_params.write_coil_05.coil_addr & 0xFF; // 低字节
    // 打包强制值（2字节）
    // 转换值
    uint16_t force_value = (frame->fun_params.write_coil_05.coil_state == 0xFF) ? 0xFF00 : 0x0000;
    *buf++ = force_value >> 8; // 高字节
    *buf++ = force_value & 0xFF; // 低字节
    *len += 4; // 更新长度
    return true;
}
static bool master_modbus_handle06_writeReg(uint8_t* buf,size_t *len, ModbusFrame* frame)
{
    // 打包寄存器地址（2字节）
    *buf++ = frame->fun_params.write_reg_06.reg_addr >> 8; // 高字节
    *buf++ = frame->fun_params.write_reg_06.reg_addr & 0xFF; // 低字节
    // 打包寄存器值 （2字节）
    *buf++ = frame->fun_params.write_reg_06.reg_value >> 8; // 高字节
    *buf++ = frame->fun_params.write_reg_06.reg_value & 0xFF; // 低字节
    *len += 4; // 更新长度
    return true;
}
/**
 * @brief mct接口函数
 */
static bool cmd_PackMsterFow(uint8_t* buf, size_t* len, void *para)
{
    ModbusFrame *frame = (ModbusFrame*)para;
    uint8_t *ptr = (uint8_t *)buf;
    size_t pack_len = 0;
    size_t frameLen = 0;

    // 写入从机地址
    *ptr++ = frame->addr;    pack_len++;
    // 写入功能码
    *ptr++ = frame->func_code;    pack_len++;
    // 写入功能码参数
    switch(frame->func_code) 
    {
        case FUNC_READ_COIL:
        {
            master_modbus_handle01_readCoil(ptr,&frameLen,frame);
        }
        break;  
        case FUNC_READ_HOLDING_REGISTER:
        {
            master_modbus_handle03_readReg(ptr,&frameLen,frame);
        }
        break;
        case FUNC_WRITE_COIL:  
        {
            mmaster_modbus_handle05_writeCoil(ptr,&frameLen,frame);
        }
        break;
        case FUNC_WRITE_HOLDING_REGISTER:
        {
        
            master_modbus_handle06_writeReg(ptr,&frameLen,frame);
        }
        break;

        default:
            return false;
    }
    ptr += frameLen; // 更新指针位置
    pack_len += frameLen;
    // 计算CRC16
    uint16_t crc = DL_CRC16(buf, pack_len);
    // 写入CRC16
    *ptr++ = crc >> 8;    pack_len++;
    *ptr++ = crc & 0xFF;    pack_len++;
    *len = pack_len;
    return true;
}

static bool cmd_AnalyzeMsterFow(uint8_t* buf, size_t len, void *para)
{
    ModbusFrame *frame = (ModbusFrame*)para;
    // 基础校验
    if(!modbus_ValidateLen(buf, len) || 
    !modbus_ValidateSlaveAddress(buf[0],frame) ||
    !modbus_ValidateFunctionCode(buf[1],frame) ||
    !modbus_ValidateCRC(buf, len)) 
    {
        return false;
    }

    //拷贝数据域
    memcpy(frame->fun_params.response.data,buf+2,len-2);
    frame->fun_params.response.len = len-2;

    return true;
}



static const tCmd cmdList[] =
{
  CMD_HEX_ADD(CMD_MODBUS_RTU_SLAVE,         0,  NULL,0,          NULL,0,       NULL,0,       RecvSend,       SlaveFow),
  CMD_HEX_ADD(CMD_MODBUS_RTU_MASTER,        1,  NULL,0,          NULL,0,       NULL,0,       SendRev,        MsterFow),
};
tCmd const *CMD_ModbusCmdGet(void)
{
    return cmdList;
}

uint16_t CMD_ModbusCmdNumGet(void)
{
    return (sizeof(cmdList) / sizeof(tCmd));
}
