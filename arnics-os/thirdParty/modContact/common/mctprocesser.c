#include "mctProcesser.h"
#include "mctLib.h"
#include "../dataPlat/mctDefinition.h"
#include "../port/mctDriver.h"

/*-------------------------------------------------------------------------------------*/

/*响应列表-----------------------------------------------------------------------------*/

void initStaticFrameList(StaticFrameList *list)
{
    memset(list, 0, sizeof(StaticFrameList));
}

void addFrameToExpectedFrameList(StaticFrameList *list, uint16_t startOffset, uint16_t endOffset,uint16_t id)
{

    list->frames_expected.startOffset = startOffset;
    list->frames_expected.endOffset = endOffset;
    list->frames_expected.length = endOffset - startOffset;
    list->frames_expected.tcmd_id = id;
    list->frames_expected.status = FRAME_NEW;

    list->have_expected = true;
}
void addFrameToFrameList(StaticFrameList *list, uint16_t startOffset, uint16_t endOffset,uint16_t id)
{
    if (list->size >= MAX_FRAMES) 
    {
        return;
    }
    list->frames[list->size].startOffset = startOffset;
    list->frames[list->size].endOffset = endOffset;
    list->frames[list->size].length = endOffset - startOffset;
    list->frames[list->size].tcmd_id = id;
    list->frames[list->size].status = FRAME_NEW;

    list->size++;
}


/*-------------------------------------------------------------------------------------*/

/*帧匹配--------------------------------------------------------------------------------*/
static bool frame_mache(MctInstance *inst, const tCmd *expected_cmd,bool is_expected,uint16_t id,StaticFrameList *payloadlist,uint16_t *remain_len)
{
    uint16_t PhaseOffset = 0;
    uint16_t SubphaseOffset = 0;


    // 如果指定了错误阶段且响应符合错误阶段，则返回错误等待状态
    if (NULL != expected_cmd->errorPhase && true == cmd_ComformRes(inst->payload_cache, inst->payload_size, expected_cmd->errorPhase, NULL, &PhaseOffset, &SubphaseOffset))
    {
        *remain_len = 0;
        return false;
    }
    // 如果响应符合正确阶段，则返回成功等待状态  如果没有指定rightPhase 也视为正确响应
    if (true == cmd_ComformRes(inst->payload_cache, inst->payload_size, expected_cmd->rightPhase, expected_cmd->SubRightPhase, &PhaseOffset, &SubphaseOffset))
    {
        if(is_expected)
        {
            addFrameToExpectedFrameList(payloadlist, PhaseOffset, SubphaseOffset,id);
        }
        else
        {
            addFrameToFrameList(payloadlist, PhaseOffset, SubphaseOffset,id);
        }

        *remain_len -= (SubphaseOffset - PhaseOffset);
        return true;
    }
    //什么都没查询到 
    *remain_len -= (SubphaseOffset - PhaseOffset);
    return false;
}
/*-------------------------------------------------------------------------------------*/

/*预期帧流程-----------------------------------------------------------------------------*/
bool expected_cmd_send(MctInstance *inst,StaticFrameList *payloadlist,tCmd const *List, uint16_t cmdNum, uint16_t expected_tcmd_id,void *para)
{
    // 遍历命令列表，寻找匹配的命令
    for (uint16_t i = 0; i < cmdNum; i++)
    {
        // 检查当前命令项是否与目标ID匹配
        if (expected_tcmd_id == List[i].id)
        {

            // 打包命令，如果失败则返回false
            if (false == List[i].pack(inst, para))
            {
                return false;
            }
            mct_write(inst->cmd_cache,inst->cmd_size);  
            return true;
        }
    }
    return false;
}



static bool expected_cmd_seek(MctInstance *inst, tCmd const *cmdList,uint16_t cmdListNum, \
                                    uint16_t expected_tcmd_id, \
                                    StaticFrameList *payloadlist)
{
    uint32_t cnt = 0;
    uint16_t remain_len = 0;
    bool result = false;
    bool found = false;
    do
    {
        uint16_t single_len = 0;

        //收帧
        single_len = mct_read(inst->payload_cache + inst->payload_size, inst->PAYLOAD_MAX_SIZE - inst->payload_size);
        inst->payload_size += single_len;
        remain_len += single_len;
        //有数据更新，则进入一次判断
        if (single_len > 0)
        {
            for (uint8_t i = 0; i < cmdListNum; i++)
            {
                // 检查当前命令项是否与目标ID匹配
                if (expected_tcmd_id == cmdList[i].id)
                {
                    result = frame_mache(inst,&cmdList[i],true,expected_tcmd_id,payloadlist,&remain_len);
                     found = true;
                     break;
                }
            }
            if (found)
            {
                break; // 找到匹配的命令项，退出外层循环
            }
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
        cnt++;
    } while (cnt < (cmdList->timeout * 1000 / WAIT_SCHEDULE_TIME_MS));

    if(remain_len > 0)
    {
        //处理沾帧，加入链表
        for(uint8_t i = 0;i < cmdListNum;i++)
        {
            //此机制下，同一帧重复命令出现多次会被过滤，只处理第一次出现的重复命令

            if( RecvSend == cmdList[i].Type)
            {
                if(cmdList[i].id == expected_tcmd_id) //跳过预期帧
                continue;

                frame_mache(inst,&cmdList[i],false,cmdList[i].id,payloadlist,&remain_len); 
                //处理完毕，无需继续遍历
                if(remain_len  == 0) 
                {
                    break; 
                }
            }
            //只会扫描一次，如果扫描不到，也认为帧处理完毕
        }
    }
    else
    {
        //帧处理完毕
    }
    return result;
}

static bool unexpected_cmd_seek(MctInstance *inst, tCmd const *cmdList, uint16_t cmdListNum, StaticFrameList *payloadlist)
{
    uint16_t single_len = 0;
    uint16_t remain_len = 0;
    bool result = false;

    // 收帧
    single_len = mct_read(inst->payload_cache + inst->payload_size, inst->PAYLOAD_MAX_SIZE - inst->payload_size);
    inst->payload_size += single_len;
    remain_len += single_len;
    // 有数据更新，则进入一次判断
    if (single_len > 0) 
    {
        for (uint16_t i = 0; i < cmdListNum; i++)
        {
            //此机制下，同一帧重复命令出现多次会被过滤，只处理第一次出现的重复命令

            if( RecvSend == cmdList[i].Type)
            {
                bool frame_match_result = frame_mache(inst, &cmdList[i], false,cmdList[i].id, payloadlist, &remain_len);
                // 如果匹配成功，记录结果
                if (frame_match_result) 
                {
                    result = true;
                }
                // 处理完毕，无需继续遍历
                if (remain_len == 0) 
                {
                    break; 
                }
            }
            //只会扫描一次，如果扫描不到，也认为帧处理完毕
        }
    }

    // 返回最终结果
    return result;
}


/**
 * @fn payload_scan

 * @retval 是否有数据
 */
static bool payload_scan(MctInstance *inst,StaticFrameList *payloadlist, \
                    tCmd const *cmdList,uint16_t cmdListNum, \
                    bool is_expected,uint32_t expected_tcmd_id)
{
    if(true == is_expected)//有预期帧
    {
        //预期帧寻找
        return expected_cmd_seek(inst,cmdList,cmdListNum,expected_tcmd_id,payloadlist); 
    }
    else
    {
        return unexpected_cmd_seek(inst, cmdList,cmdListNum,payloadlist);
    }

}
/**************************************************************************************** */

static dealprocess singleframeListDeal(MctInstance *inst, StaticFrameList *payloadlist, \
                                       tCmd const *cmdList, uint16_t payloadlist_id, uint16_t cmdListNum, void *para)
{
    bool commandFound = false;
    dealprocess status = FRAME_FAILED;

    for (uint8_t i = 0; i < cmdListNum; i++)
    {
        // 检查当前命令项是否与目标ID匹配
        if (payloadlist->frames[payloadlist_id].tcmd_id == cmdList[i].id)
        {
            commandFound = true;

            if (!cmdList[i].analyze(inst, para))
            {
                break;
            }

            if (!cmdList[i].pack(inst, para))
            {
                break;
            }

            if (!mct_write(inst->cmd_cache, inst->cmd_size))
            {
                break;
            }

            status = FRAME_SUCCEED;
            break;
        }
    }

    if (!commandFound)
    {
        status = FRAME_FAILED;
    }

    payloadlist->frames[payloadlist_id].status = status;
    return status;
}

void frameListDeal(MctInstance *inst,StaticFrameList *payloadlist,tCmd const *cmdList,uint16_t cmdListNum,void *para)
{
  if(payloadlist->size > 0)
  { 
    for(uint8_t i = 0;i < payloadlist->size;i++)
    {
        if(payloadlist->frames[i].status ==    FRAME_NEW)
        {
            singleframeListDeal(inst,payloadlist,cmdList,i,cmdListNum,para);
        }
    }
  }
}



bool expectframeDeal(MctInstance *inst, StaticFrameList *payloadlist, tCmd const *cmdList, uint16_t cmdListNum, void *para)
{
    // 如果没有预期帧，直接返回 false
    if (!payloadlist->have_expected)
    {
        return false;
    }

    // 遍历命令列表
    for (uint8_t i = 0; i < cmdListNum; i++)
    {
        // 检查当前命令项是否与目标ID匹配
        if (payloadlist->frames_expected.tcmd_id == cmdList[i].id)
        {
            // 调用 analyze 函数
            if (cmdList[i].analyze(inst, para))
            {
                // 分析成功，设置状态为成功并返回 true
                payloadlist->frames_expected.status = FRAME_SUCCEED;
                return true;
            }
            else
            {
                // 分析失败，设置状态为失败并返回 false
                payloadlist->frames_expected.status = FRAME_FAILED;
                return false;
            }
        }
    }

    // 没有找到匹配的命令项，返回 false
    return false;
}





bool CMD_Execute(MctInstance *inst, \
                bool is_expected,uint32_t expected_tcmd_id, \
                tCmd const *List,uint16_t cmdNum,void *para)
{

    {
        initStaticFrameList(&inst->payload_list);
        mct_data_reset(inst);
    }


    bool result = false;
    //判断
    if(is_expected)
    {
        if(expected_cmd_send(inst,&inst->payload_list,List,cmdNum,expected_tcmd_id,para))
        {
            if(payload_scan(inst,&inst->payload_list,List,cmdNum,is_expected,expected_tcmd_id))
            {
                result = expectframeDeal(inst,&inst->payload_list,List,cmdNum,para); 
                
            }
            frameListDeal(inst,&inst->payload_list,List,cmdNum,para);
        }

    
    }
    else
    {
        payload_scan(inst,&inst->payload_list,List,cmdNum,is_expected,expected_tcmd_id);
        frameListDeal(inst,&inst->payload_list,List,cmdNum,para);
    }

    return result;
}
