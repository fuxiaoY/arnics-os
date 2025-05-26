#include "command_modbusAdapter.h"



static bool modbus_01_handle(MctInstance *inst,void *para)
{
    ModbusFrame *frame = (ModbusFrame*)para;
    frame->func_code = 0x01;

    if(mct_modbus_execute(inst,CMD_MODBUS_RTU_MASTER,frame))
    {
        return true;
    }
    else
    {
        return false;
    }

}
static bool modbus_03_handle(MctInstance *inst,void *para)
{
    ModbusFrame *frame = (ModbusFrame*)para;
    frame->func_code = 0x03;
    if(mct_modbus_execute(inst,CMD_MODBUS_RTU_MASTER,frame))
    {
        return true;
    }
    else
    {
        return false;
    }
}
static bool modbus_05_handle(MctInstance *inst,void *para)
{
    ModbusFrame *frame = (ModbusFrame*)para;
    frame->func_code = 0x05;
    if(mct_modbus_execute(inst,CMD_MODBUS_RTU_MASTER,frame))
    {
        return true;
    }
    else
    {
        return false;
    }
}

static bool modbus_06_handle(MctInstance *inst,void *para)
{
    ModbusFrame *frame = (ModbusFrame*)para;
    frame->func_code = 0x06;
    if(mct_modbus_execute(inst,CMD_MODBUS_RTU_MASTER,frame))
    {
        return true;
    }
    else
    {
        return false;
    }
}


static bool slave_modbusHandle(MctInstance *inst,void *para)
{
    ModbusFrame modbus = {0};
    memset(&modbus, 0, sizeof(ModbusFrame));
    mct_modbus_execute(inst,CMD_MODBUS_RTU_SLAVE,&modbus);
    return true;
}


static const tCmdApi funList[] =
{
    {.id = CMD_MODBUS_SLAVE,   .fun = slave_modbusHandle},
    {.id = CMD_MODBUS_01,   .fun = modbus_01_handle},
    {.id = CMD_MODBUS_03,   .fun = modbus_03_handle},
    {.id = CMD_MODBUS_05,   .fun = modbus_05_handle},
    {.id = CMD_MODBUS_06,   .fun = modbus_06_handle},
        
};

tCmdApi const *mctModbusApiGet(void)
{
    return funList;
}
/*
void modbus_test()
{
    ModbusFrame frame = {0};
    memset(&frame, 0, sizeof(ModbusFrame));
    frame.addr = 0x01;
    // 功能码01
    frame.fun_params.read_coil_01.coil_addr = 0x0002; //线圈地址
    frame.fun_params.read_coil_01.num = 0x0005; //线圈数量
    mctApiExecute(&consoleInstance,CMD_MODBUS_01,&frame);
    rtosThreadDelay(1000);
    // 功能码03
    frame.fun_params.read_reg_03.start_addr = 0x0002;
    frame.fun_params.read_reg_03.num = 0x0005; //寄存器数量
    mctApiExecute(&consoleInstance,CMD_MODBUS_03,&frame);
    rtosThreadDelay(1000);
    // 功能码05
    frame.fun_params.write_coil_05.coil_addr = 0x0002;
    frame.fun_params.write_coil_05.coil_state = 0xFF; //线圈状态
    mctApiExecute(&consoleInstance,CMD_MODBUS_05,&frame);
    rtosThreadDelay(1000);
    // 功能码06
    frame.fun_params.write_reg_06.reg_addr = 0x0002;
    frame.fun_params.write_reg_06.reg_value = 0x000A; //寄存器值
    mctApiExecute(&consoleInstance,CMD_MODBUS_06,&frame);


}
*/
