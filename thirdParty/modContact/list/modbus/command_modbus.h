
#ifndef _COMMAND_MODBUS_H
#define _COMMAND_MODBUS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../common/mctProcesser.h"



#define CMD_MODBUS_RTU_MASTER                           (int32_t)0
#define CMD_MODBUS_RTU_SLAVE                            (1+CMD_MODBUS_RTU_MASTER)

/* ----------------------- Defines ------------------------------------------*/
#define ADDRESS_BROADCAST                   ( 0 )   /*! Modbus broadcast address. */
#define FUNC_NONE                          (  0 )
#define FUNC_READ_COIL                     (  1 )
#define FUNC_READ_HOLDING_REGISTER         (  3 )

#define FUNC_WRITE_COIL                    (  5 )
#define FUNC_WRITE_HOLDING_REGISTER        (  6 )


typedef struct {
    uint8_t addr;           // 设备地址
    uint8_t func_code;      // 功能码
    union {
        /* 01功能码 - 读线圈 */
        struct 
        {
            uint16_t coil_addr;
            uint8_t num;
        } read_coil_01;

        /* 03功能码 - 读寄存器 */
        struct 
        {
            uint16_t start_addr;
            uint16_t num;
        } read_reg_03;
        /* 05功能码 - 写单线圈 */
        struct 
        {
            uint16_t coil_addr;
            uint8_t coil_state;
        } write_coil_05;
        /* 06功能码 - 写单个寄存器 */
        struct 
        {
            uint16_t reg_addr;
            uint16_t reg_value;
        } write_reg_06;
        struct command_modbus
        {
            uint8_t data[256]; // 响应数据
            uint8_t len; // 响应数据长度
        }response;
        

    } fun_params;
} ModbusFrame;
// 线圈与寄存器注册
typedef struct 
{
    uint16_t address;
    uint16_t (*read)(void);
    void (*write)(uint16_t value);
} ModbusRegister;

typedef struct 
{
    uint16_t address;
    bool (*read)(void);
    void (*write)(bool value);
} ModbusCoil;

extern tCmd const *CMD_ModbusCmdGet(void);
extern uint16_t CMD_ModbusCmdNumGet(void);
/**
 * @fn mct_modbus_execute
 * @brief 
 * 
 * @param [in] inst - mct实例

 * @param [in] expected_tcmd_id   - 期望的命令id   NULL_CMD_SEEK：无需匹配
 * @param [in] para - 回调参数
 * @retval None
 */
#define mct_modbus_execute(inst,expected_tcmd_id, para)                  CMD_Execute(inst,expected_tcmd_id,CMD_ModbusCmdGet(), CMD_ModbusCmdNumGet(), para)


#ifdef __cplusplus
}
#endif
#endif
