#include "mctLib.h"



/*-------------------------------------------------------------------------------------*/

/*内核辅助函数--------------------------------------------------------------------------*/
/**
 * @brief 比较两个数据块是否相同
 *
 * 该函数用于比较指定长度的数据块src和dst是否相同。如果相同，则返回true，并将匹配到的指针偏移位置传入offset参数中。
 * 如果不同，则根据特定条件返回false。
 * 主要用于在数据处理中进行快速比较，以确定两个数据块是否匹配。
 *
 * @param src 源数据块的指针
 * @param dst 目标数据块的指针
 * @param len_src 源数据块的长度
 * @param len_dest 目标数据块的长度
 * @param offset 匹配到的指针偏移位置
 * @return 是否找到匹配的数据块
 *         - true: 找到匹配的数据块
 *         - false: 没有找到匹配的数据块或者源数据块剩余长度小于目标数据块长度
 */
bool sd_Parse(const void *src, const void *dst, uint16_t len_src, uint16_t len_dest, uint16_t *offset)
{
    const uint8_t *pcmp1 = (uint8_t *)src;  // 指向源数据块的指针
    const uint8_t *pcmp2 = (uint8_t *)dst;  // 指向目标数据块的指针

    // 如果目标数据块长度大于源数据块长度，设置偏移位置为源数据块长度 直接返回false
    if (len_dest > len_src)
    {
        *offset = len_src;
        return false;
    }

    // 遍历源数据块的所有可能起始位置
    for (uint16_t i = 0; i <= len_src - len_dest; i++)
    {
        pcmp1 = (const uint8_t *)src + i;  // 设置源数据块的当前起始位置
        pcmp2 = (uint8_t *)dst;  // 重置目标数据块的指针

        // 逐字节比较目标数据块和源数据块的子串
        uint16_t j;
        for (j = 0; j < len_dest; j++)
        {
            if (*pcmp1 != *pcmp2)
            {
                break;  // 如果不匹配，跳出内层循环
            }
            pcmp1++;  // 移动源数据块指针
            pcmp2++;  // 移动目标数据块指针
        }

        // 如果内层循环完整执行，说明找到匹配
        if (j == len_dest)
        {
            *offset = i;  // 设置匹配到的偏移位置
            return true;  // 返回true表示找到匹配
        }
    }

    *offset = len_src;  // 如果没有找到匹配，设置偏移位置为源数据块长度
    return false;  // 返回false表示没有找到匹配
}

/**
 * @fn cmd_ComformRes
 * @brief 根据给定阶段确认资源
 * 
 * 此函数用于验证回调函数中的资源是否符合特定阶段。
 * 首先检查阶段参数是否为空。如果为空，则认为验证成功，并返回整个srcaddr的地址。
 * 如果阶段参数不为空，则使用sd_Parse函数解析资源。如果解析失败，
 * 验证失败。否则，认为验证成功。
 *
 * @param [in] srcaddr - 源地址指针
 * @param [in] src_len - 源地址长度
 * @param [in] phase - 用于验证的阶段字符串指针
 * @param [in] subphase - 用于验证的子阶段字符串指针
 * @param [out] PhaseOffset - 阶段偏移位置
 * @param [out] SubphaseOffset - 子阶段偏移位置
 * @retval 验证结果
 *         - true: 验证成功
 *         - false: 验证失败
 */
bool cmd_ComformRes(uint8_t *srcaddr, size_t src_len,
                            const char *phase, const char *subphase,
                            uint16_t *PhaseOffset, uint16_t *SubphaseOffset)
{
    uint16_t phaseOffset = 0;
    uint16_t subphaseOffset = 0;

    // 检查阶段参数是否为空
    if (NULL == phase)
    {
        *PhaseOffset = 0;
        *SubphaseOffset = src_len;
        return true; // no phase, return entire srcaddr
    }

    // 解析阶段
    if (false == sd_Parse(srcaddr, phase, src_len, strlen(phase), &phaseOffset))
    {
        return false;
    }

    // 检查阶段偏移是否超出范围
    if (phaseOffset + strlen(phase) > src_len)
    {
        return false;
    }

    // 检查子阶段参数是否为空
    if (NULL == subphase)
    {
        // 设置阶段偏移和子阶段偏移
        *PhaseOffset = phaseOffset;
        *SubphaseOffset = src_len; // set subphaseOffset to the end of srcaddr
        return true; // no subphase
    }

    // 解析子阶段
    if (false == sd_Parse(srcaddr + phaseOffset + strlen(phase), subphase, src_len - (phaseOffset + strlen(phase)), strlen(subphase), &subphaseOffset))
    {
        return false;
    }

    // 设置阶段偏移和子阶段偏移
    *PhaseOffset = phaseOffset;
    *SubphaseOffset = phaseOffset + strlen(phase) + subphaseOffset + strlen(subphase);

    return true;
}

/**
 * @fn cmd_ComformResUint8
 * @brief 根据给定阶段确认资源（使用uint8_t数组）
 * 
 * 此函数用于验证回调函数中的资源是否符合特定阶段。
 * 首先检查阶段参数是否为空。如果为空，则认为验证成功，并返回整个srcaddr的地址。
 * 如果阶段参数不为空，则使用sd_Parse函数解析资源。如果解析失败，
 * 验证失败。否则，认为验证成功。
 *
 * @param [in] srcaddr - 源地址指针
 * @param [in] src_len - 源地址长度
 * @param [in] phase - 用于验证的阶段uint8_t数组指针
 * @param [in] phase_len - 阶段数组长度
 * @param [in] subphase - 用于验证的子阶段uint8_t数组指针
 * @param [in] subphase_len - 子阶段数组长度
 * @param [out] PhaseOffset - 阶段偏移位置
 * @param [out] SubphaseOffset - 子阶段偏移位置
 * @retval 验证结果
 *         - true: 验证成功
 *         - false: 验证失败
 */
bool cmd_ComformResUint8(uint8_t *srcaddr, size_t src_len,
                            const uint8_t *phase, size_t phase_len,
                            const uint8_t *subphase, size_t subphase_len,
                            uint16_t *PhaseOffset, uint16_t *SubphaseOffset)
{
    uint16_t phaseOffset = 0;
    uint16_t subphaseOffset = 0;

    // 检查阶段参数是否为空
    if (NULL == phase || phase_len == 0)
    {
        *PhaseOffset = 0;
        *SubphaseOffset = src_len;
        return true; // no phase, return entire srcaddr
    }

    // 解析阶段
    if (false == sd_Parse(srcaddr, phase, src_len, phase_len, &phaseOffset))
    {
        return false;
    }

    // 检查阶段偏移是否超出范围
    if (phaseOffset + phase_len > src_len)
    {
        return false;
    }

    // 检查子阶段参数是否为空
    if (NULL == subphase || subphase_len == 0)
    {
        // 设置阶段偏移和子阶段偏移
        *PhaseOffset = phaseOffset;
        *SubphaseOffset = src_len; // set subphaseOffset to the end of srcaddr
        return true; // no subphase
    }

    // 解析子阶段
    if (false == sd_Parse(srcaddr + phaseOffset + phase_len, subphase, src_len - (phaseOffset + phase_len), subphase_len, &subphaseOffset))
    {
        return false;
    }

    // 设置阶段偏移和子阶段偏移
    *PhaseOffset = phaseOffset;
    *SubphaseOffset = phaseOffset + phase_len + subphaseOffset + subphase_len;

    return true;
}

/*-------------------------------------------------------------------------------------*/

/*工具函数-----------------------------------------------------------------------------*/
/**
 * @brief 在一个字节序列中查找另一个字节序列的首次出现位置。
 *
 * @param mom 指向母序列的指针。
 * @param son 指向子序列的指针。
 * @param mom_len 母序列的长度。
 * @param son_len 子序列的长度。
 * @return uint8_t* 如果找到子序列，则返回指向母序列中子序列起始位置的指针；否则返回NULL。
 *
 * 该函数用于在一段字节序列（mom）中查找另一段字节序列（son）的首次出现位置。
 * 如果子序列的长度大于母序列的长度，则直接返回NULL。
 * 如果子序列的长度小于或等于母序列的长度，则遍历母序列，逐个比较子序列与母序列中相应位置的字节是否相同。
 * 如果所有字节都相同，则返回当前母序列指针位置；如果遍历完母序列后仍未找到完全相同的子序列，则返回NULL。
 */
uint8_t *hexhex(uint8_t *mom, uint8_t *son, uint16_t mom_len, uint16_t son_len)
{
    if(mom == NULL || son == NULL)
    {
        return NULL;
    }
    if(0 == mom_len || 0 == son_len)
    {
        return NULL;
    }
    uint8_t *mom_cur = mom;
    uint16_t i = 0, j = 0;
    if (son_len > mom_len)
    {
        return NULL;
    }
    if (mom_len >= son_len)
    {
        for (i = 0; i <= mom_len - son_len; i++)
        {
            mom_cur = mom + i;
            for (j = 0; j < son_len; j++)
            {
                if (*(mom_cur + j) != *(son + j))
                {
                    break;
                }
            }
            if (j == son_len)
            {
                return mom_cur;
            }
        }
    }

    return NULL;
}

// 将ASCII字符串转换为HEX字符串
void AsciiToHex(const char *asciiStr, char *hexStr, size_t hexLen)
{
    const char *hexChars = "0123456789abcdef";
    for (size_t i = 0, j = 0; i < strlen(asciiStr) && j < hexLen; i++, j += 2)
    {
        unsigned char ch = (unsigned char)asciiStr[i];

        // 将高四位转换为十六进制字符
        hexStr[j] = hexChars[(ch >> 4) & 0x0F];

        // 将低四位转换为十六进制字符
        hexStr[j + 1] = hexChars[ch & 0x0F];
    }
    hexStr[hexLen] = '\0';
}



// 将单个十六进制字符转换为对应的数值
static uint8_t hexCharToValue(char c) 
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

// 将十六进制字符串转换为 ASCII 字符串
void hexToAscii(const char *hexStr, char *asciiStr, size_t asciiLen) 
{
    // 确保目标缓冲区足够大
    if (asciiLen == 0) return;

    char *out = asciiStr;
    const char *end = hexStr + strlen(hexStr);

    while (hexStr < end && out - asciiStr < asciiLen) 
    {
        if (hexStr + 1 < end) 
        {
            uint8_t val = (hexCharToValue(hexStr[0]) << 4) + hexCharToValue(hexStr[1]);
            *out++ = val;
        }
        hexStr += 2;
    }

    // 确保字符串以空字符结尾
    *out = '\0';
}
// 转换示例用法
/*
{
    const char *asciiStr = "Hello, World!";
    char hexStr[256];
    AsciiToHex(asciiStr, hexStr, sizeof(hexStr));
    printf("ASCII: %s\n", asciiStr);
    printf("HEX: %s\n", hexStr);

    const char *hexStr2 = "48656C6C6F2C20576F726C6421";
    char asciiStr2[256];
    hexToAscii(hexStr2, asciiStr2, sizeof(asciiStr2));
    printf("HEX: %s\n", hexStr2);
    printf("ASCII: %s\n", asciiStr2);

    return 0;
}
*/


// 解析命令字，两个字节组成的HEX，可以表示从01到FF的255种命令
uint8_t hexChrToAscii(const char *HexChr)
{
  if ((HexChr[0] < '0' || HexChr[0] > '9') && (HexChr[0] < 'A' || HexChr[0] > 'F'))
  {
    return 0;
  }
  if ((HexChr[1] < '0' || HexChr[1] > '9') && (HexChr[1] < 'A' || HexChr[1] > 'F'))
  {
    return 0;
  }

  uint8_t HexTmp[2] = {0};
  HexTmp[0] = (HexChr[0] <= '9') ? (HexChr[0] - '0') : (HexChr[0] + 10 - 'A');
  HexTmp[1] = (HexChr[1] <= '9') ? (HexChr[1] - '0') : (HexChr[1] + 10 - 'A');
  return ((uint8_t)HexTmp[0] << 4) + (uint8_t)HexTmp[1];
}
static void cmd_SkipWhiteSpace(uint8_t **p_cur)
{
    if (*p_cur == NULL)
    {
        return;
    }
    while (**p_cur != '\0' && ISSPACE(**p_cur))
    {
        (*p_cur)++;
    }
}
/**
 * @brief 查找并提取下一个令牌
 * 
 * 本函数用于从给定的字符串中查找并提取下一个令牌。如果令牌被引号包围，则提取直到下一个引号的位置。
 * 如果令牌不是被引号包围，则提取直到下一个逗号或行结束的位置。
 * 
 * @param p_cur 指向当前解析位置的指针，该指针会更新以指向下一个令牌之后的位置
 * @return char* 返回提取的令牌，如果找不到则返回NULL
 */
static char *cmd_FindNextToken(uint8_t **p_cur)
{
    char *ret = NULL; // 用于存储提取的令牌
    char *ptr = NULL; // 用于遍历和截断令牌

    // 跳过开始的空白字符
    cmd_SkipWhiteSpace(p_cur);
    if (*p_cur == NULL) // 如果当前指针为空，则没有更多令牌可提取
    {
        return NULL;
    }

    // 如果当前指针指向的是一个引号包围的令牌
    if ('"' == **p_cur)
    {
        ret = (char *)(*p_cur + 1); // 跳过引号，指向令牌实际开始位置
        ptr = strchr(ret, '"'); // 查找结束引号的位置
        if (NULL == ptr) // 如果找不到结束引号
        {
            return NULL;
        }
        *ptr = '\0'; // 用空字符结束令牌
        // 如果结束引号后面是逗号，则移动到逗号位置
        if (',' == *(ptr + 1))
        {
            ptr++;
        }
    }
    else // 如果当前指针指向的是一个非引号包围的令牌
    {
        ret = (char *)*p_cur; // 指向令牌开始位置
        ptr = (char *)*p_cur; // 用于遍历令牌
        // 遍历直到回车符、逗号或字符串结束
        for (; (('\r' != *ptr) && (',' != *ptr) && ('\0' != *ptr)); ptr++)
        {
        }
        if (NULL == ptr) // 如果ptr为空，则表示异常情况
        {
            return NULL;
        }
    }

    *ptr = '\0'; // 用空字符截断令牌
    *p_cur = (uint8_t *)(ptr + 1); // 更新当前解析位置指针到下一个令牌之后的位置
    return ret; // 返回提取的令牌
}


int cmd_TokenNextStr(uint8_t **p_cur, uint8_t **p_out)
{
    if (*p_cur == NULL)
    {
        return -1;
    }
    *p_out = (uint8_t *)cmd_FindNextToken(p_cur);
    if (*p_out == NULL)
    {
        return -1;
    }

    return 0;
}
/**
 * 寻找令牌的起始位置。
 * 
 * 本函数用于在给定的字符串中查找特定令牌的起始位置。如果找到了令牌，则返回0，
 * 否则返回-1。令牌可以是一个子字符串，本函数将返回子字符串在原字符串中的位置。
 * 
 * @param p_cur 指向当前处理的字符串位置的指针的指针。
 * @param phase 需要查找的子字符串。
 * @return 如果找到令牌返回0，否则返回-1。
 */
int cmd_TokenStart(uint8_t **p_cur, const char *phase)
{
    if (*p_cur == NULL)
    {
        return -1;
    }
    if (phase == NULL)
    {
        cmd_SkipWhiteSpace(p_cur);
        if (*p_cur == NULL)
        {
            return -1;
        }
    }
    else
    {
        // 将 phase 转换为 uint8 * 类型
        *p_cur = (uint8_t *)strstr((const char *)*p_cur, phase);
        if (*p_cur == NULL)
        {
            return -1;
        }
        (*p_cur) += strlen(phase);
    }
    return 0;
}
static int cmd_TokenNextIntBase(uint8_t **p_cur, int *p_out, int base, int uns)
{
    char *ret;

    if (*p_cur == NULL)
    {
        return -1;
    }
    ret = cmd_FindNextToken(p_cur);
    if (ret == NULL)
    {
        return -1;
    }
    else
    {
        long l;
        char *end;
        l = uns ? strtoul(ret, &end, base) : strtol(ret, &end, base);
        *p_out = (int)l;
        if (end == ret)
        {
            return -1;
        }
    }

    return 0;
}

int cmd_TokenNextInt(uint8_t **p_cur, int *p_out, uint8_t format)
{
    return cmd_TokenNextIntBase(p_cur, p_out, format, 0);
}



/********************************** End of file ************************/
