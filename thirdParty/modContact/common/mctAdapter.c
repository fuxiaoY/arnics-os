#include "mctAdapter.h"


bool AT_found(MctInstance *inst)
{
    bool cmd_use_malloc, payload_use_malloc = false;
    if(inst->cmd_cache == NULL)
    {
        cmd_use_malloc = true;
        MCT_MALLOC((void **)&inst->cmd_cache, inst->CMD_MAX_SIZE);
    }
    if(inst->payload_cache == NULL)
    {
        payload_use_malloc = true;
        MCT_MALLOC((void **)&inst->payload_cache, inst->PAYLOAD_MAX_SIZE);
    }

    inst->cmd_size = sprintf((char*)inst->cmd_cache, "\r\nATI\r\n");
    inst->mct_write(inst->cmd_cache, inst->cmd_size);
    uint32_t cnt = 0;
    uint16_t total_len = 0;
    do
    {
        uint16_t single_len = 0;

        //收帧
        single_len = inst->mct_read(inst->payload_cache + inst->payload_size, inst->PAYLOAD_MAX_SIZE - inst->payload_size);
        inst->payload_size += single_len;
        total_len += single_len;
        //有数据更新，则进入一次判断
        if (single_len > 0)
        {
            for(uint8_t i =0;i<mctModemLisNumGet();i++)
            {
                if (NULL != hexhex(inst->payload_cache, (uint8_t*)mctModemListGet()[i].name, total_len, strlen(mctModemListGet()[i].name)))
                {
                    inst->api = mctModemListGet()[i].api;
                    if(cmd_use_malloc)
                    {
                        MCT_FREE((void **)&inst->cmd_cache);
                    }
                    if(payload_use_malloc)
                    {
                        MCT_FREE((void **)&inst->payload_cache);
                    }
                    return true;

                }
            }
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
        cnt++;
    } while (cnt < (3000 / WAIT_SCHEDULE_TIME_MS));
    if(cmd_use_malloc)
    {
        MCT_FREE((void **)&inst->cmd_cache);
    }
    if(payload_use_malloc)
    {
        MCT_FREE((void **)&inst->payload_cache);
    }
   return false;
}



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
    mct_data_reset(inst);
    if (true == ModemAtuoRecogniton)
    {
        return AT_found(inst);
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
    bool cmd_use_malloc = false, payload_use_malloc = false;
    if(inst->cmd_cache == NULL)
    {
        cmd_use_malloc = true;
        // 使用动态内存缓存
        MCT_MALLOC((void **)&inst->cmd_cache, inst->CMD_MAX_SIZE);
    }
    if(inst->payload_cache == NULL)
    {
        payload_use_malloc = true;
        MCT_MALLOC((void **)&inst->payload_cache, inst->PAYLOAD_MAX_SIZE);
    }
    bool result = false;
    // 检查找到的命令函数指针是否为空
    if (NULL != inst->api()[index].fun)
    {
        // 如果不为空，则调用命令函数并返回执行结果
        result =  inst->api()[index].fun(inst,para);
        if(cmd_use_malloc)
        {
            MCT_FREE((void **)&inst->cmd_cache);
        }
        if(payload_use_malloc)
        {
            MCT_FREE((void **)&inst->payload_cache);
        }
        return result;
    }
    else
    {
        return result; // 如果为空，则返回执行失败
    }
}
/* define ------------------------------------------------------------*/

void mct_init(MctInstance* pInstance,uint8_t *Cmd_cache,size_t cmd_max_size, \
                                    uint8_t* Payload_cahe,size_t payload_max_size, \
                                    int (*write_callback)(void *file, uint16_t len), \
                                    uint32_t (*read_callback)(uint8_t *buf, uint16_t maxlen))
{
    //instance init
    pInstance->cmd_cache = Cmd_cache;
    pInstance->payload_cache = Payload_cahe;
    pInstance->CMD_MAX_SIZE = cmd_max_size;
    pInstance->PAYLOAD_MAX_SIZE = payload_max_size;
    pInstance->mct_write = write_callback; // 设置写回调函数
    pInstance->mct_read = read_callback;   // 设置读回调函数

    pInstance->cmd_size = 0;
    pInstance->payload_size = 0;
    initStaticFrameList(&pInstance->payload_list);
}    
