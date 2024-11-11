#include "mctAdapter.h"
#include "../port/mctDriver.h"
#include "../list/mctList.h"


/**
 * @brief 模块识别函数
 * 
 * 本函数用于根据Modem自动识别的设置，来初始化模块并尝试识别模块型号。
 * 如果设置为自动识别，将尝试通过发送AT命令并分析响应来识别模块型号。
 * 如果不进行自动识别，则直接初始化为默认模块。
 * 
 * @param ModemAtuoRecogniton 模块自动识别的标志，TRUE表示自动识别，FALSE表示不自动识别。
 * @return bool 返回识别结果，TRUE表示识别成功，FALSE表示识别失败。
 */
bool mct_reg(const char* name,MctInstance *inst,bool ModemAtuoRecogniton)
{
    if (true == ModemAtuoRecogniton)
    {
        return false;
    }
    else
    {
        for(uint8_t i =0;i<mctModemLisNumGet();i++)
        {
            if(0 == strcmp(name,mctModemListGet()[i].name))
            {
                inst->api = mctModemListGet()[i].api;
                return true;
            }
        }
        return false;
    }


}

/**
 * @brief 执行命令API
 *
 * 此函数用于根据命令ID查找并执行相应的命令函数。它首先检查命令回调函数指针是否为空，
 * 然后在命令列表中查找匹配的命令ID。如果找到匹配的ID且对应的命令函数不为空，
 * 则调用该命令函数并返回执行结果。
 *
 * @param id 命令ID
 * @param para 命令参数指针，具体含义取决于命令函数的实现
 * @return 是否成功执行命令的布尔值
 */
bool mctApiExecute(MctInstance *inst,uint16_t id, void *para)
{
    uint16_t index = 0; // 初始化命令索引

    // 检查命令回调函数指针是否为空
    if (NULL == inst->api)
    {
        return false; // 如果为空，则返回执行失败
    }

    // 遍历命令列表，查找匹配的命令ID
    for (; (id != inst->api()[index].id) && (index < CMD_MAX); index++)
    {
    }

    // 检查是否找到匹配的命令ID
    if (index >= CMD_MAX)
    {
        return false; // 如果未找到，则返回执行失败
    }

    // 检查找到的命令函数指针是否为空
    if (NULL != inst->api()[index].fun)
    {
        // 如果不为空，则调用命令函数并返回执行结果
        return inst->api()[index].fun(inst,para);
    }
    else
    {
        return false; // 如果为空，则返回执行失败
    }
}
