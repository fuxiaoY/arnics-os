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
    const uint8_t *pcmp1 = src;
    const uint8_t *pcmp2 = dst;
    uint16_t tmp_len1 = len_src;
    uint16_t found_len = 0;
    uint16_t pcmp1_len = 0;

    while (tmp_len1--)
    {
        if (*pcmp1 != *pcmp2)
        {
            if (tmp_len1 + 1 < len_dest)
            {
                *offset = pcmp1_len;
                return false;
            }
            else
            {
                pcmp1_len++, pcmp1++, pcmp2 = dst;
                found_len = 0;
            }
        }
        else
        {
            pcmp1_len++, pcmp1++, pcmp2++, found_len++;
            if (found_len == len_dest)
            {
                *offset = pcmp1_len - len_dest;
                return true;
            }
        }
    }
    *offset = pcmp1_len;
    return false;
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
static uint8_t hexCharToValue(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

// 将十六进制字符串转换为 ASCII 字符串
void hexToAscii(const char *hexStr, char *asciiStr, size_t asciiLen) {
    // 确保目标缓冲区足够大
    if (asciiLen == 0) return;

    char *out = asciiStr;
    const char *end = hexStr + strlen(hexStr);

    while (hexStr < end && out - asciiStr < asciiLen) {
        if (hexStr + 1 < end) {
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




/********************************** End of file ************************/
