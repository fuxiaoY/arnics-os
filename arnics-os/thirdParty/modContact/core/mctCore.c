#include "mctCore.h"


/*-------------------------------------------------------------------------------------*/
/*实例与缓冲管理 */
void mct_data_reset(MctInstance* pInstance)
{
    if(NULL != pInstance->cmd_cache)
    {
        memset(pInstance->cmd_cache,0,pInstance->CMD_MAX_SIZE);
    }
    if(NULL != pInstance->payload_cache)
    {
        memset(pInstance->payload_cache,0,pInstance->PAYLOAD_MAX_SIZE);
    }
    pInstance->cmd_size = 0;
    pInstance->payload_size = 0;
}


/*-------------------------------------------------------------------------------------*/
/*帧链表原语 */

void initStaticFrameList(StaticFrameList *list)
{
    memset(list, 0, sizeof(StaticFrameList));
}

static void clearFrameList(StaticFrameList *list)
{
    list->size = 0;
    memset(list->frames, 0, sizeof(list->frames));

}

static void clearFrameExpectedFrameList(StaticFrameList *list)
{
    list->have_expected = false;
    memset(&list->frames_expected, 0, sizeof(list->frames_expected));
}

static void addFrame(StaticFrameList *list, uint16_t startOffset, uint16_t endOffset, uint16_t id, bool is_expected)
{
    if (is_expected)
    {
        list->frames_expected.startOffset = startOffset;
        list->frames_expected.endOffset = endOffset;
        list->frames_expected.length = (uint16_t)(endOffset - startOffset);
        list->frames_expected.tcmd_id = id;
        list->frames_expected.status = FRAME_NEW;
        list->have_expected = true;
    }
    else
    {
        if (list->size >= MAX_FRAMES)
        {
            return;
        }
        list->frames[list->size].startOffset = startOffset;
        list->frames[list->size].endOffset = endOffset;
        list->frames[list->size].length = (uint16_t)(endOffset - startOffset);
        list->frames[list->size].tcmd_id = id;
        list->frames[list->size].status = FRAME_NEW;
        list->size++;
    }
}


/*-------------------------------------------------------------------------------------*/
/*底层读取 */

// 统一读取载荷并更新 remain_len 与 payload_size,返回本次读取长度
static int32_t read_payload_once(MctInstance *inst, int32_t *remain_len)
{
    int32_t single_len = 0;
    if (NULL == inst || NULL == inst->mct_read || NULL == inst->payload_cache)
    {
        return 0;
    }

    /* mct_read 原型返回 uint32_t,这里以有符号 int32_t 承接(实际读取量远小于 INT32_MAX) */
    single_len = (int32_t)inst->mct_read(inst->payload_cache + inst->payload_size, \
                                         inst->PAYLOAD_MAX_SIZE - inst->payload_size);
    if (single_len > 0)
    {
        inst->payload_size = (uint16_t)(inst->payload_size + single_len);
        if (remain_len)
        {
            /* 有符号累加：缓冲为静态有界,不会触及 int32_t 上限 */
            *remain_len = *remain_len + single_len;
        }
    }
    return single_len;
}


/*-------------------------------------------------------------------------------------*/
/*切帧策略层*/

/* 类型1 ｜ DELIMITER：头 + 尾(legacy)
 *
 * 匹配顺序与原 frame_mache 完全一致：先错误帧,再“头+尾”。
 * 说明：对 legacy 而言 NEED_MORE 与 NO_HEAD 经引擎处理后可观察行为完全相同
 *       (frame_mache 都映射为 match_null；粘帧侧只采纳 MATCH),
 *       故未完整匹配时统一返回 NO_HEAD,保证与旧实现逐位一致、不做多余搜索。 */
static frame_scan_result_t mct_framing_delimiter(const frame_input_t *in,
                                                 const uint8_t *window, uint16_t len,
                                                 uint16_t *start, uint16_t *end)
{
    uint16_t phase_off = 0;
    uint16_t end_off = 0;
    bool ok = false;

    /* 1) 错误帧优先：与原 frame_mache 的错误判定一致(子阶段传 NULL) */
    if (NULL != in->err)
    {
        if (FRAMING_FMT_ASCII == in->format)
        {
            ok = cmd_ComformRes(window, len, (const char *)in->err, NULL, &phase_off, &end_off);
        }
        else
        {
            ok = cmd_ComformResUint8(window, len, in->err, in->err_len, NULL, 0, &phase_off, &end_off);
        }
        if (ok)
        {
            return FRAME_SCAN_ERROR;
        }
    }

    /* 2) 正确帧：头 + 尾 */
    phase_off = 0;
    end_off = 0;
    if (FRAMING_FMT_ASCII == in->format)
    {
        ok = cmd_ComformRes(window, len, (const char *)in->head, (const char *)in->tail, &phase_off, &end_off);
    }
    else
    {
        ok = cmd_ComformResUint8(window, len, in->head, in->head_len, \
                                in->tail, in->tail_len, &phase_off, &end_off);
    }

    if (ok && end_off >= phase_off)
    {
        *start = phase_off;
        *end = end_off;
        return FRAME_SCAN_MATCH;
    }
    return FRAME_SCAN_NO_HEAD;
}

/* 类型2 ｜ CALLBACK：框架先定位帧头,再把“帧头指针+帧头起长度”交给用户回调。
 * 帧头按原始字节查找(sd_Parse),ASCII 字符串头与 Hex 字节头通用。
 * 用户只面对“帧头=偏移0”这一个坐标系；head_pos 换算在此内部完成,不暴露给用户。 */
static frame_scan_result_t mct_framing_callback(const frame_input_t *in,
                                                const uint8_t *window, uint16_t len,
                                                uint16_t *start, uint16_t *end)
{
    uint16_t head_pos = 0;
    bool head_found = true;
    uint16_t rel_s = 0;   /* 用户回填：相对帧头 */
    uint16_t rel_e = 0;
    frame_scan_result_t r;

    if (NULL == in->spec || NULL == in->spec->user_cb)
    {
        return FRAME_SCAN_NO_HEAD;
    }

    /* 定位帧头：未配置帧头时帧头即窗口起点(0) */
    if (NULL != in->head && in->head_len > 0)
    {
        head_found = sd_Parse(window, in->head, len, in->head_len, &head_pos);
    }
    if (false == head_found)
    {
        return FRAME_SCAN_NO_HEAD; /* 窗口内无帧头,不打扰用户回调 */
    }

    /* 调用用户回调：buffer 顶在帧头第一字节；cb_len 为帧头起可用长度。
     * 回填的 rel_s/rel_e 相对帧头，再由本函数加回 head_pos 换算成 window 坐标，
     * 供引擎与边界守卫使用（对用户完全透明）。 */
    r = in->spec->user_cb(window + head_pos, (uint16_t)(len - head_pos), \
                          &rel_s, &rel_e, in->spec->user_arg);
    if (FRAME_SCAN_MATCH == r)
    {
        *start = (uint16_t)(head_pos + rel_s);
        *end   = (uint16_t)(head_pos + rel_e);
    }
    return r;
}

/* 函数表：下标=切帧类型,元素=对应策略函数；仅本文件使用 */
static const framing_scan_fn framing_strategy_table[FRAMING_TYPE_MAX] =
{
    [FRAMING_DELIMITER] = mct_framing_delimiter,
    [FRAMING_CALLBACK]  = mct_framing_callback,
};

/* 统一入口：分派 + 唯一边界守卫
 * 即使策略/回调写错,也无法返回越界或零长区间；非法结果统一按 NO_HEAD 处理,
 * 调用方不会就同一窗口重算,从而杜绝死循环。仅本文件使用。 */
static frame_scan_result_t mct_frame_scan(framing_kind_t kind,
                                         const frame_input_t *in,
                                         const uint8_t *window, uint16_t len,
                                         uint16_t *start, uint16_t *end)
{
    frame_scan_result_t r;

    if (NULL == in || NULL == window || NULL == start || NULL == end)
    {
        return FRAME_SCAN_NO_HEAD;
    }
    if ((uint32_t)kind >= (uint32_t)FRAMING_TYPE_MAX || NULL == framing_strategy_table[kind])
    {
        return FRAME_SCAN_NO_HEAD;
    }

    r = framing_strategy_table[kind](in, window, len, start, end);

    if (FRAME_SCAN_MATCH == r)
    {
        if (*start >= *end || *end > len)
        {
            return FRAME_SCAN_NO_HEAD;
        }
    }
    return r;
}


/*-------------------------------------------------------------------------------------*/
/*帧匹配 */

/* 防腐适配：把 tCmd 装配成策略层认识的中立 frame_input_t。
 * 这是引擎内唯一“懂 tCmd”的地方；策略层因此与 tCmd 解耦。 */
static void build_frame_input(const tCmd *cmd, frame_input_t *in)
{
    memset(in, 0, sizeof(*in));

    in->format = (HeX == cmd->format) ? FRAMING_FMT_HEX : FRAMING_FMT_ASCII;
    in->head   = (const uint8_t *)cmd->rightPhase;
    in->tail   = (const uint8_t *)cmd->SubRightPhase;
    in->err    = (const uint8_t *)cmd->errorPhase;

    if (AscII == cmd->format)
    {
        /* ASCII：字符串长度现场求,不依赖长度字段 */
        in->head_len = (NULL != cmd->rightPhase)   ? (uint16_t)strlen((const char *)cmd->rightPhase)   : 0;
        in->tail_len = (NULL != cmd->SubRightPhase)? (uint16_t)strlen((const char *)cmd->SubRightPhase): 0;
        in->err_len  = (NULL != cmd->errorPhase)   ? (uint16_t)strlen((const char *)cmd->errorPhase)   : 0;
    }
    else
    {
        in->head_len = cmd->rightPhaseLen;
        in->tail_len = cmd->SubRightPhaseLen;
        in->err_len  = cmd->errorPhaseLen;
    }

    /* 仅 CALLBACK 需要附加参数；DELIMITER 传 NULL,走纯 legacy */
    if (FRAMING_CALLBACK == cmd->framing_kind)
    {
        in->spec = &cmd->framing_spec;
    }
    else
    {
        in->spec = NULL;
    }
}

/* 通过统一切帧入口完成匹配。预期帧路径把整段 payload_cache 作为窗口,
 * 错误帧处理、addFrame、remain_len 扣减等对外语义保持不变。 */
static frameMacheType frame_mache(MctInstance *inst, const tCmd *expected_cmd,bool is_expected,uint16_t id,StaticFrameList *payloadlist,int32_t *remain_len)
{
    frame_input_t in;
    uint16_t start = 0;
    uint16_t end = 0;
    frame_scan_result_t scan;

    build_frame_input(expected_cmd, &in);
    scan = mct_frame_scan(expected_cmd->framing_kind, &in, \
                          inst->payload_cache, (uint16_t)inst->payload_size, &start, &end);

    if (FRAME_SCAN_ERROR == scan)
    {
        if (remain_len)
        {
            *remain_len = 0;
        }
        return match_error;
    }
    if (FRAME_SCAN_MATCH == scan)
    {
        /* 边界守卫已保证 end > start,无需再做偏移有序判断 */
        addFrame(payloadlist, start, end, id, is_expected);
        if (remain_len)
        {
            /* 有符号扣减已匹配帧长,不足归零,杜绝无符号下溢回绕 */
            int32_t left = *remain_len - (int32_t)(end - start);
            *remain_len = (left > 0) ? left : 0;
        }
        return match_sucess;
    }

    /* NEED_MORE / NO_HEAD 均视为本拍无完整帧 */
    return match_null;
}


/*-------------------------------------------------------------------------------------*/
/*命令发送 */

static bool expected_cmd_send(MctInstance *inst,StaticFrameList *payloadlist,tCmd const *List, uint16_t cmdlist_seq_i, int32_t expected_tcmd_id,void *para)
{
    // 打包命令,如果失败则返回false
    if (false == List[cmdlist_seq_i].pack(inst->cmd_cache,&inst->cmd_size, para))
    {
        return false;
    }
    if(0 > inst->mct_write(inst->cmd_cache,inst->cmd_size))
    {
        return false;
    }
    return true;

}


/*-------------------------------------------------------------------------------------*/
/*粘帧切分 */

/**
 * @fn   sticky_overlap_expected
 * @brief 判断一条 RecvSend 帧是否与已匹配的预期帧在区间上重叠。
 *
 *        半开区间重叠条件：frame_start < expected_end && expected_start < frame_end。
 *        用于避免预期帧被粘帧逻辑重复切割,同时不影响同 id 但位于别处的帧。
 *
 * @param[in] list        帧链表(含预期帧信息)
 * @param[in] frame_start 该 RecvSend 帧的起始偏移
 * @param[in] frame_end   该 RecvSend 帧的结束偏移
 * @return true 与预期帧重叠；false 无预期帧或不重叠
 */
static bool sticky_overlap_expected(StaticFrameList *list, uint16_t frame_start, uint16_t frame_end)
{
    uint16_t expected_start = 0;
    uint16_t expected_end = 0;

    if (NULL == list || false == list->have_expected)
    {
        return false; /* 本次没有预期帧,不存在冲突 */
    }

    expected_start = list->frames_expected.startOffset;
    expected_end = list->frames_expected.endOffset;
    return (frame_start < expected_end && expected_start < frame_end);
}

/**
 * @fn   sticky_frames_extract
 * @brief 游标式贪心切分缓冲区中的所有粘帧。
 *
 *        每轮在窗口 payload_cache[cursor, 末尾) 内,从所有 RecvSend 命令中选出
 *        起始偏移最早的一帧加入链表,并将游标推进到该帧结尾；重复直至窗口内无
 *        任何匹配(剩余为脏数据或残缺尾帧)、链表满或切分轮数达到 MAX_FRAMES。
 *
 *        特性：同类型重复帧可全部切出；顺序严格遵循字节流；帧间脏数据自动跳过；
 *        游标每轮严格增加,保证收敛、不会死循环。
 *
 * @param[in]    inst        协议实例
 * @param[in]    cmdList     命令表
 * @param[in]    cmdListNum  命令表条数
 * @param[inout] payloadlist 帧链表,切出的帧追加到其 frames[]
 */
static void sticky_frames_extract(MctInstance *inst, tCmd const *cmdList, uint16_t cmdListNum, \
                                  StaticFrameList *payloadlist)
{
    uint16_t cursor = 0;    /* 游标：此前所有字节已处理完毕,只增不减 */
    uint8_t round_count = 0;/* 已切分轮数,与游标构成双重防死循环保障 */

    if (NULL == inst || NULL == cmdList || NULL == payloadlist || 0 == inst->payload_size)
    {
        return;
    }

    /* 循环三道条件：尚有未处理字节；链表未满；切分轮数未越限 */
    while (cursor < inst->payload_size && payloadlist->size < MAX_FRAMES && round_count < MAX_FRAMES)
    {
        bool recv_frame_found = false;    /* 本轮是否已扫描到 RecvSend 帧 */
        uint16_t earliest_start = 0;      /* 本轮已扫到的 RecvSend 帧中,起始最早者的绝对起点 */
        uint16_t earliest_end = 0;        /* 对应绝对终点 */
        uint16_t earliest_cmd_id = 0;     /* 对应命令id */
        uint16_t window_len = (uint16_t)(inst->payload_size - cursor); /* 当前窗口长度 */

        round_count++;

        /* ---- 第一步：扫描所有 RecvSend 命令,保留窗口内起始最早者 ---- */
        for (uint16_t i = 0; i < cmdListNum; i++)
        {
            uint16_t candidate_start = 0;
            uint16_t candidate_end = 0;
            frame_input_t in;
            uint16_t rel_s = 0;   /* 帧起点：相对当前窗口 */
            uint16_t rel_e = 0;   /* 帧终点：相对当前窗口 */
            frame_scan_result_t scan;

            /* 只切设备主动上报帧；SendRev 等其它类型不参与粘帧切分 */
            if (RecvSend != cmdList[i].Type)
            {
                continue;
            }

            /* 在当前窗口 payload_cache[cursor..] 上走统一切帧入口(与预期帧匹配同源),
             * 返回偏移相对该窗口,随后换算为整段缓冲区的绝对偏移 */
            build_frame_input(&cmdList[i], &in);
            scan = mct_frame_scan(cmdList[i].framing_kind, &in, \
                                  inst->payload_cache + cursor, window_len, &rel_s, &rel_e);
            if (FRAME_SCAN_MATCH != scan)
            {
                continue; /* ERROR/NEED_MORE/NO_HEAD 在粘帧侧均不切 */
            }

            candidate_start = (uint16_t)(cursor + rel_s);
            candidate_end = (uint16_t)(cursor + rel_e);

            /* 防御性校验：该帧须位于窗口内且为正长度,确保游标严格向前
             * (边界守卫已保证 rel_s<rel_e 且 rel_e<=window_len,此处为双保险) */
            if (candidate_start < cursor || candidate_end <= candidate_start)
            {
                continue;
            }
            /* 该帧若落在预期帧区间内则跳过(预期帧已单独处理) */
            if (sticky_overlap_expected(payloadlist, candidate_start, candidate_end))
            {
                continue;
            }

            /* 本轮还没扫到 RecvSend 帧时,先把这一帧记下；
             * 已经扫到过,则只在这一帧起始更早时替换 */
            if (false == recv_frame_found || candidate_start < earliest_start)
            {
                recv_frame_found = true;
                earliest_start = candidate_start;
                earliest_end = candidate_end;
                earliest_cmd_id = cmdList[i].id;
            }
        }

        /* ---- 第二步：本轮没扫到任何 RecvSend 帧 -> 剩余为脏数据或残缺尾帧,停止扫描 ---- */
        if (false == recv_frame_found)
        {
            break;
        }

        /* ---- 第三步：切出选中帧 ----
         * [cursor, earliest_start) 是帧起点之前未被任何帧覆盖的字节,即帧间脏数据,
         * 不加入链表、随游标推进一并丢弃；这里只把有效帧 [earliest_start,earliest_end) 入表。 */
        addFrame(payloadlist, earliest_start, earliest_end, earliest_cmd_id, false);

        /* 游标推进到该帧结尾：因 earliest_end > earliest_start >= cursor,游标严格增加,
         * 保证算法必然收敛(同类型的下一帧只会在新窗口中被再次命中) */
        cursor = earliest_end;
    }
}


/*-------------------------------------------------------------------------------------*/
/*寻帧/接收引擎 */

static bool expected_cmd_seek(MctInstance *inst, tCmd const *cmdList,uint16_t cmdListNum, \
                                    int32_t expected_tcmd_id, \
                                    StaticFrameList *payloadlist,uint16_t cmdlist_seq_i)
{
    uint32_t cnt = 0;
    int32_t remain_len = 0;
    bool result = false;

    /* 保护性检查：避免除以零 */
    uint32_t wait_ms = (WAIT_SCHEDULE_TIME_MS == 0) ? 1U : (uint32_t)WAIT_SCHEDULE_TIME_MS;
    uint32_t loop_max = 0;
    if (cmdList[cmdlist_seq_i].timeout > 0)
    {
        uint32_t t_ms = (uint32_t)cmdList[cmdlist_seq_i].timeout * 1000U;
        loop_max = t_ms / wait_ms;
    }
    else
    {
        loop_max = 0;
    }

    do
    {
        int32_t single_len = 0;

        // 收帧
        single_len = read_payload_once(inst, &remain_len);

        //有数据更新,则进入一次判断
        if (single_len > 0)
        {

            frameMacheType matcheResult = frame_mache(inst,&cmdList[cmdlist_seq_i],true,expected_tcmd_id,payloadlist,&remain_len);
            //匹配到正确字段和匹配到错误字段都直接退出
            if(match_sucess == matcheResult)
            {
                result = true;
                break;
            }
            else if(match_error == matcheResult)
            {
                result = false;
                break;
            }
        }
        MCT_DELAY(wait_ms);
        cnt++;
    } while (cnt < loop_max);

    /* 匹配结束后,若还有未被预期帧覆盖的字节且开启了粘帧处理,则切分粘帧。
     * remain_len 在命中预期帧时只扣减了预期帧自身长度,故此处：
     *   remain_len = 帧前字节数 + 帧后字节数
     * 任一侧存在粘帧即 >0 -> 预期帧之前/之后的粘帧都会进入切分。 */
    if(remain_len > 0 && inst->sticky_frame_enable)
    {
        /* 游标式贪心切分：同类型重复帧(AAB/ABA)可全部按字节流顺序切出,
         * 自动跳过帧间无匹配的脏数据,并保证收敛、不死循环 */
        sticky_frames_extract(inst, cmdList, cmdListNum, payloadlist);
    }
    else
    {
        //帧处理完毕
    }
    return result;
}

static bool all_cmd_seek(MctInstance *inst, tCmd const *cmdList, uint16_t cmdListNum, StaticFrameList *payloadlist)
{
    int32_t single_len = 0;
    int32_t remain_len = 0;
    bool result = false;

    // 收帧
    single_len = read_payload_once(inst, &remain_len);

    // 有数据更新,则进入一次判断
    if (single_len > 0)
    {
        for (uint16_t i = 0; i < cmdListNum; i++)
        {
            //此机制下,同一帧重复命令出现多次会被过滤,只处理第一次出现的重复命令

            if( RecvSend == cmdList[i].Type)
            {
                frameMacheType matcheResult = frame_mache(inst, &cmdList[i], false,cmdList[i].id, payloadlist, &remain_len);
                // 如果匹配到,记录结果
                if(match_sucess == matcheResult)
                {
                    result = true;
                }
                else if(match_error == matcheResult)
                {
                    result = false;
                }


                // 处理完毕,无需继续遍历
                if (remain_len == 0)
                {
                    break;
                }
            }
            //只会扫描一次,如果扫描不到,也认为帧处理完毕
        }
    }

    // 返回最终结果
    return result;
}

static bool cmd_seek_with_pecify(MctInstance *inst, tCmd const *cmdList, uint16_t cmdListNum, \
                                                                            int32_t expected_tcmd_id, \
                                                                            StaticFrameList *payloadlist)
{
    int32_t single_len = 0;
    int32_t remain_len = 0;
    bool result = false;

    // 收帧
    single_len = read_payload_once(inst, &remain_len);

    // 有数据更新,则进入一次判断
    if (single_len > 0)
    {
        for (uint16_t i = 0; i < cmdListNum; i++)
        {
            //此机制下,同一帧重复命令出现多次会被过滤,只处理第一次出现的重复命令
            //只匹配预期
            if((RecvSend == cmdList[i].Type)&&(expected_tcmd_id == cmdList[i].id))
            {
                frameMacheType matcheResult = frame_mache(inst, &cmdList[i], true,cmdList[i].id, payloadlist, &remain_len);
                // 如果匹配到,记录结果
                if(match_sucess == matcheResult)
                {
                    result = true;
                }
                else if(match_error == matcheResult)
                {
                    result = false;
                }

                // 处理完毕,无需继续遍历
                if (remain_len == 0)
                {
                    break;
                }
            }
            //只会扫描一次,如果扫描不到,也认为帧处理完毕
        }
    }

    // 返回最终结果
    return result;
}

/**
 * @fn payload_scan
 * @brief 寻帧入口分派：按场景转发到对应的 seek 例程。
 * @retval 是否收到/匹配到数据
 */
static bool payload_scan(MctInstance *inst,StaticFrameList *payloadlist, \
                    tCmd const *cmdList,uint16_t cmdListNum, \
                    bool is_send_rev,int32_t expected_tcmd_id,uint16_t cmdlist_seq_i)
{
    if(true == is_send_rev)
    {
        //预期帧寻找
        return expected_cmd_seek(inst,cmdList,cmdListNum,expected_tcmd_id,payloadlist,cmdlist_seq_i);
    }
    else
    {
        if(NULL_CMD_SEEK == expected_tcmd_id)
        {
            return all_cmd_seek(inst, cmdList,cmdListNum,payloadlist);
        }
        else
        {
            return cmd_seek_with_pecify(inst, cmdList,cmdListNum,expected_tcmd_id,payloadlist);
        }

    }

}


/*-------------------------------------------------------------------------------------*/
/*帧结果处理 */

/* ---- 8.1 预期帧结果 ---- */

static dealprocess singleframeListDeal_expected(MctInstance *inst, StaticFrameList *payloadlist, \
                                       tCmd const *cmdList, uint16_t cmdListNum, void *para)
{
    bool commandFound = false;
    dealprocess status = FRAME_FAILED;

    for (uint8_t i = 0; i < cmdListNum; i++)
    {
        // 检查当前命令项是否与目标ID匹配
        if (payloadlist->frames_expected.tcmd_id == cmdList[i].id)
        {
            commandFound = true;

            if (!cmdList[i].analyze(inst->payload_cache + payloadlist->frames_expected.startOffset,payloadlist->frames_expected.length, para))
            {
                break;
            }

            if (!cmdList[i].pack(inst->cmd_cache,&inst->cmd_size,para))
            {
                break;
            }

            if (0 > inst->mct_write(inst->cmd_cache, inst->cmd_size))
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
    return status;
}

static bool expectframeDeal(MctInstance *inst, StaticFrameList *payloadlist, tCmd const *cmdList, uint16_t cmdListNum, void *para)
{
    // 如果没有预期帧,直接返回 false
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
            if (cmdList[i].analyze(inst->payload_cache + payloadlist->frames_expected.startOffset,payloadlist->frames_expected.length, para))
            {
                // 分析成功,设置状态为成功并返回 true
                payloadlist->frames_expected.status = FRAME_SUCCEED;
                return true;
            }
            else
            {
                // 分析失败,设置状态为失败并返回 false
                payloadlist->frames_expected.status = FRAME_FAILED;
                return false;
            }
        }
    }

    // 没有找到匹配的命令项,返回 false
    return false;
}

/* ---- 8.2 粘帧列表结果 ---- */

static dealprocess singleframeListDeal(MctInstance *inst, StaticFrameList *payloadlist, \
                                       tCmd const *cmdList, uint16_t payloadlist_id, uint16_t cmdListNum)
{
    bool commandFound = false;
    dealprocess status = FRAME_FAILED;

    for (uint8_t i = 0; i < cmdListNum; i++)
    {
        // 检查当前命令项是否与目标ID匹配
        if (payloadlist->frames[payloadlist_id].tcmd_id == cmdList[i].id)
        {
            commandFound = true;
            void *para = NULL;
            if(USE_VAR == cmdList[i].stickytype)
            {
                para = cmdList[i].para;
            }
            else if(USE_CB == cmdList[i].stickytype)
            {
                if (cmdList[i].get_para)
                {
                    para = cmdList[i].get_para();
                }
            }
            else
            {
                //do nothing
            }
            if (!cmdList[i].analyze(inst->payload_cache + payloadlist->frames[payloadlist_id].startOffset,
                                    payloadlist->frames[payloadlist_id].length, para))
            {
                break;
            }

            if (!cmdList[i].pack(inst->cmd_cache,&inst->cmd_size,para))
            {
                break;
            }

            if (0 > inst->mct_write(inst->cmd_cache, inst->cmd_size))
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

static dealprocess frameListDeal(MctInstance *inst,StaticFrameList *payloadlist,tCmd const *cmdList,uint16_t cmdListNum)
{
  dealprocess status = FRAME_FAILED;
  if(payloadlist->size > 0)
  {
    for(uint8_t i = 0;i < payloadlist->size;i++)
    {
        if(payloadlist->frames[i].status == FRAME_NEW)
        {
            if(FRAME_SUCCEED == singleframeListDeal(inst,payloadlist,cmdList,i,cmdListNum))
            {
                status = FRAME_SUCCEED;
            }
        }
    }
  }
  return status;
}


/*-------------------------------------------------------------------------------------*/
/*对外公共 API */

bool CMD_Execute(MctInstance *inst, \
                int32_t expected_tcmd_id, \
                tCmd const *List,uint16_t cmdNum,void *para)
{

    {
        initStaticFrameList(&inst->payload_list);
        mct_data_reset(inst);
    }
    uint16_t i = 0;
    bool is_send_rev = false;
    bool result = false;
    uint16_t cmdlist_seq_i = 0;

    // 遍历命令列表,寻找匹配的命令
    for (i = 0; i < cmdNum; i++)
    {
        // 检查当前命令项是否与目标ID匹配
        if (expected_tcmd_id == List[i].id)
        {
            cmdlist_seq_i = i;
            if(List[i].Type == SendRev)
            {
                is_send_rev = true;
            }
            break;
        }
    }
    if((i == cmdNum)&&(expected_tcmd_id != NULL_CMD_SEEK))
    {
        //未定义命令
        return false;
    }

    //判断是否是SendRev类型的命令
    if(is_send_rev)
    {
        if(expected_cmd_send(inst,&inst->payload_list,List,cmdlist_seq_i,expected_tcmd_id,para))
        {
            if(payload_scan(inst,&inst->payload_list,List,cmdNum,is_send_rev,expected_tcmd_id,cmdlist_seq_i))
            {
                result = expectframeDeal(inst,&inst->payload_list,List,cmdNum,para);

            }
            frameListDeal(inst,&inst->payload_list,List,cmdNum);
        }
    }
    else
    {
        if(payload_scan(inst,&inst->payload_list,List,cmdNum,is_send_rev,expected_tcmd_id,cmdlist_seq_i))
        {
            if(inst->payload_list.have_expected)
            {
                if(FRAME_SUCCEED == singleframeListDeal_expected(inst,&inst->payload_list,List,cmdNum,para))
                {
                    result = true;
                }
            }
            if(inst->payload_list.size > 0)
            {
                if(FRAME_SUCCEED == frameListDeal(inst,&inst->payload_list,List,cmdNum))
                {
                    result = true;
                }
            }
        }
    }
    clearFrameList(&inst->payload_list);
    clearFrameExpectedFrameList(&inst->payload_list);

    return result;
}
