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
                                                                    .pack = cmd_Pack##funName, \
                                                                    .analyze = cmd_Analyze##funName}




extern void AsciiToHex(const char *asciiStr, char *hexStr, size_t hexLen);
extern void hexToAscii(const char *hexStr, char *asciiStr, size_t asciiLen);
extern bool sd_Parse(const void *src, const void *dst, uint16_t len_src, uint16_t len_dest, uint16_t *offset);
extern bool cmd_ComformRes(uint8_t *srcaddr, size_t src_len,const char *phase, const char *subphase, uint16_t *PhaseOffset, uint16_t *SubphaseOffset);


#ifdef __cplusplus
}
#endif
#endif
