
#ifndef __TYPEDEF_H
#define __TYPEDEF_H
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <cstdarg> 
#include <stdarg.h>
#include <math.h> // 用于fabs函数
/* \brief Portable data type defines */
/* 申明存储联合体定义:t-数据类型 n-成员名 l-预留数据长度,如果预留过小,会编译错误 */
#define SMALLOC(t,n,l) union{t n; uint8 n##_buf[l*((l/sizeof(t))/(l/sizeof(t)))];}

typedef signed char        int8, S8;
typedef unsigned char      uint8, U8;
typedef signed short       int16, S16;
typedef unsigned short     uint16, U16;
typedef signed long        int32, S32;
typedef unsigned long      uint32, U32;
typedef float              fp32, F32;
typedef double             fp64, F64;


typedef long long INT64;
typedef unsigned long long UINT64;
typedef unsigned long UINT32;
typedef signed long INT32;
typedef unsigned short  UINT16;
typedef unsigned char UINT8;

typedef unsigned int UINT;
typedef unsigned short USHORT;
typedef unsigned char BYTE;

#ifndef BOOL
typedef unsigned char BOOL;
#endif

#if ((!defined FALSE) && (!defined TRUE))
enum Enum_BOOLValue
{
  FALSE = 0,
  TRUE = 1
};
#endif
#if (!defined __cplusplus) && (!defined bool)
#define bool BYTE
#endif

#if ((!defined false) && (!defined true))
#define false 0
#define true 1
#endif
#ifndef TRUE
#define TRUE          true
#endif

#ifndef FALSE
#define FALSE         false
#endif

#ifndef NULL
#define NULL   (void*)0
#endif

#define IS_FLAG_SET(flag) ((flag) ? 1 : 0)
#define SET_FLAG(flag) (flag = TRUE)
#define RESET_FLAG(flag) (flag = FALSE)

#ifndef st
    #define st(x)         do { x } while (__LINE__ == -1)
#endif

/* \brief Get bit mask value from the bit no. ect. the mask of third bit is 0x0008 */
#ifndef bitmask
  #define bitmask(x)     (1u << (x))
#endif

#ifndef lbitmask
  #define lbitmask(x)    (1UL << (x))
#endif

/**
  * \brief  Translate expression statement or variable to bool value.
  * \param  [in]  x-the statement.
  * \return The result TRUE(1) or FALSE(0) in \a x.
  */
#ifndef boolof
    #define boolof(x)    ((bool)(x) ? TRUE : FALSE)
#endif

/**
  * \brief  Returns the number of elements in an array.
  * \param  [in]  x-the defined array name.
  * \return The count of elements in \a x.
  */
#ifndef eleof
    #define eleof(x)     (sizeof(x) / sizeof((x)[0]))
#endif

/**
  * \brief  Returns the offset of member in a structure.
  * \param  [in]  T-the defined structure's name.
  * \param  [in]  member-the member name in defined structure.
  * \return The offset of the specified member in defined structure.
  */
#ifndef offsetof
  #define offsetof(T, member)   (size_t((&((T*)0)->member)))
#endif

/* \brief  Returns the value of x based alignment size
  * \param  [in]  x-the actual data length.
  * \param  [in]  size-hope to have an alignment unit.
  * \return The allocated alignment length.
  */
#ifndef alignof
#define alignof(x,size)  ((size)*(((uint32)(x)+(size)-1)/(size)))
#endif

/**
  * \brief  Counts the trailing zero bits of the given value considered as a 32-bit integer.
  * \param  [in]  u-Value of which to count the trailing zero bits.
  * \return The count of trailing zero bits in \a u.
  */

#define ctz(u)             ((u) & (1UL <<  0) ?  0 : \
                            (u) & (1UL <<  1) ?  1 : \
                            (u) & (1UL <<  2) ?  2 : \
                            (u) & (1UL <<  3) ?  3 : \
                            (u) & (1UL <<  4) ?  4 : \
                            (u) & (1UL <<  5) ?  5 : \
                            (u) & (1UL <<  6) ?  6 : \
                            (u) & (1UL <<  7) ?  7 : \
                            (u) & (1UL <<  8) ?  8 : \
                            (u) & (1UL <<  9) ?  9 : \
                            (u) & (1UL << 10) ? 10 : \
                            (u) & (1UL << 11) ? 11 : \
                            (u) & (1UL << 12) ? 12 : \
                            (u) & (1UL << 13) ? 13 : \
                            (u) & (1UL << 14) ? 14 : \
                            (u) & (1UL << 15) ? 15 : \
                            (u) & (1UL << 16) ? 16 : \
                            (u) & (1UL << 17) ? 17 : \
                            (u) & (1UL << 18) ? 18 : \
                            (u) & (1UL << 19) ? 19 : \
                            (u) & (1UL << 20) ? 20 : \
                            (u) & (1UL << 21) ? 21 : \
                            (u) & (1UL << 22) ? 22 : \
                            (u) & (1UL << 23) ? 23 : \
                            (u) & (1UL << 24) ? 24 : \
                            (u) & (1UL << 25) ? 25 : \
                            (u) & (1UL << 26) ? 26 : \
                            (u) & (1UL << 27) ? 27 : \
                            (u) & (1UL << 28) ? 28 : \
                            (u) & (1UL << 29) ? 29 : \
                            (u) & (1UL << 30) ? 30 : \
                            (u) & (1UL << 31) ? 31 : \
                            32)

/**
  * \brief Calls the routine at address \a addr. It generates a long call opcode.
  * \param [in]  addr-Address of the routine to call.
  * \note  It may be used as a long jump opcode in some special cases.
  */
#define long_call(addr)     ((*(void (*)(void))(addr))())

#ifndef Bit
#define Bit(x) (1UL << (x))
#endif
#ifndef BIT0
#define BIT0                (0x0001u)
#define BIT1                (0x0002u)
#define BIT2                (0x0004u)
#define BIT3                (0x0008u)
#define BIT4                (0x0010u)
#define BIT5                (0x0020u)
#define BIT6                (0x0040u)
#define BIT7                (0x0080u)
#define BIT8                (0x0100u)
#define BIT9                (0x0200u)
#define BITA                (0x0400u)
#define BITB                (0x0800u)
#define BITC                (0x1000u)
#define BITD                (0x2000u)
#define BITE                (0x4000u)
#define BITF                (0x8000u)

#define BIT10               0x0400
#define BIT11               0x0800
#define BIT12               0x1000
#define BIT13               0x2000
#define BIT14               0x4000
#define BIT15               0x8000
#endif
/*
#define GETBIT(x, bit)      ((x) & (bit))
#define SETBIT(x, bit)      ((x) |= (bit))
#define IS_BIT_SET(x, bit)  ((GETBIT(x,bit) == bit)? TRUE:FALSE)
*/
/* below for get the rand data */
//#define ULONG_MAX  ((uint32)(~0L)) /* 0xFFFFFFFF*/
//#define LONG_MAX   ((int32)(ULONG_MAX >> 1))/* 0x7FFFFFFF*/

/*---------------------函数接口---------------------------*/
//#pragma arm section code="._entry_app_api"
//#pragma arm section

// 段定义宏
#define PLACE_IN_SECTION(section_name) __attribute__((section(section_name)))

// 定义结构体函数
#define T_TYPEDEF(name) (*p##name)
#define T_STRUCT_MEMBER(name) p##name t_##name
// 定义结构体变量
#define T_STRUCT_VAR(name,TYPE) TYPE *t_##name

// 宏定义来初始化结构体成员
#define INIT_MEMBER(name) .t_##name =(&name)
/** @} */
/** @} */

#endif /* __TYPEDEF_H */

