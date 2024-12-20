#ifndef __MCTLIB_H__
#define __MCTLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define ISSPACE(ch)                                             (ch == ' ' || \
                                                                    ch == '\n' || \
                                                                    ch == '\f' || \
                                                                    ch == '\f' || \
                                                                    ch == '\r' || \
                                                                    ch == '\t' || \
                                                                    ch == '\v')

#define CMD_ADD(index, time, right, subright, error, type, funName) {.id = index, \
                                                                    .timeout = time, \
                                                                    .rightPhase = right, \
                                                                    .SubRightPhase = subright, \
                                                                    .errorPhase = error, \
                                                                    .Type = type, \
                                                                    .format =AscII, \
                                                                    .pack = cmd_Pack##funName, \
                                                                    .analyze = cmd_Analyze##funName}

#define CMD_HEX_ADD(index, time, right,rightlen, subright,subrightlen, error,errorlen, type, funName)  {.id = index, \
                                                                    .timeout = time, \
                                                                    .rightPhase = right, \
                                                                    .rightPhaseLen = rightlen, \
                                                                    .SubRightPhase = subright, \
                                                                    .SubRightPhaseLen = subrightlen, \
                                                                    .errorPhase = error, \
                                                                    .errorPhaseLen = errorlen, \
                                                                    .Type = type, \
                                                                    .format =HeX, \
                                                                    .pack = cmd_Pack##funName, \
                                                                    .analyze = cmd_Analyze##funName}





/*内核辅助函数--------------------------------------------------------------------------*/
extern bool sd_Parse(const void *src, const void *dst, uint16_t len_src, uint16_t len_dest, uint16_t *offset);
extern bool cmd_ComformRes(uint8_t *srcaddr, size_t src_len,const char *phase, const char *subphase, uint16_t *PhaseOffset, uint16_t *SubphaseOffset);
extern bool cmd_ComformResUint8(uint8_t *srcaddr, size_t src_len, \
                            const uint8_t *phase, size_t phase_len, \
                            const uint8_t *subphase, size_t subphase_len, \
                            uint16_t *PhaseOffset, uint16_t *SubphaseOffset);

/*工具函数-----------------------------------------------------------------------------*/
/**
 * @fn hexhex
 * @brief 在一个字节序列中查找另一个字节序列的首次出现位置。
 *
 * @param [in] mom - 指向母序列的指针。
 * @param [in] son - 指向子序列的指针。
 * @param [in] mom_len - 母序列的长度。
 * @param [in] son_len - 子序列的长度。
 * @return uint8_t* - 如果找到子序列，则返回指向母序列中子序列起始位置的指针；否则返回NULL。
 *
 * 该函数用于在一段字节序列（mom）中查找另一段字节序列（son）的首次出现位置。
 * 如果子序列的长度大于母序列的长度，则直接返回NULL。
 * 如果子序列的长度小于或等于母序列的长度，则遍历母序列，逐个比较子序列与母序列中相应位置的字节是否相同。
 * 如果所有字节都相同，则返回当前母序列指针位置；如果遍历完母序列后仍未找到完全相同的子序列，则返回NULL。
 */
extern uint8_t *hexhex(uint8_t *mom, uint8_t *son, uint16_t mom_len, uint16_t son_len);
/**
 * @fn AsciiToHex
 * @brief 将ASCII字符串转换为HEX字符串
 *
 * @param [in] asciiStr - 输入的ASCII字符串
 * @param [out] hexStr - 输出的HEX字符串
 * @param [in] hexLen - 输出HEX字符串的最大长度
 * @retval None
 */
extern void AsciiToHex(const char *asciiStr, char *hexStr, size_t hexLen);

/**
 * @fn hexToAscii
 * @brief 将十六进制字符串转换为ASCII字符串
 *
 * @param [in] hexStr - 输入的十六进制字符串
 * @param [out] asciiStr - 输出的ASCII字符串
 * @param [in] asciiLen - 输出ASCII字符串的最大长度
 * @retval None
 */
extern void hexToAscii(const char *hexStr, char *asciiStr, size_t asciiLen);

/**
 * @fn HexChrToASCII
 * @brief 将两个字节组成的HEX字符串转换为ASCII字符
 *
 * @param [in] HexChr - 输入的两个字节组成的HEX字符串
 * @retval 返回转换后的ASCII字符
 */
extern uint8_t hexChrToAscii(const char *HexChr);


#ifdef __cplusplus
}
#endif
#endif
