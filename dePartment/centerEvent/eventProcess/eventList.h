
#ifndef _EVENLIST_H
#define _EVENLIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../Inc/projDefine.h"
#include "../../../Inc/typedef.h"
#include "../../../core/coreInclude.h"
#pragma arm section code="._entry_event_api"

typedef union
{
    uint32_t byte;
    struct
    {
        uint32_t F_led_action : 1;         // 1 bit
        uint32_t F_battery_check : 1;      // 1 bit
        uint32_t F_B2 : 1;                 // 1 bit
        uint32_t F_B3 : 1;                 // 1 bit
        uint32_t F_B4 : 1;                 // 1 bit
        uint32_t F_B5 : 1;                 // 1 bit
        uint32_t F_B6 : 1;                 // 1 bit
        uint32_t F_B7 : 1;                 // 1 bit

        uint32_t F_B8 : 1;                 // 1 bit
        uint32_t F_B9 : 1;                 // 1 bit
        uint32_t F_B10 : 1;                // 1 bit
        uint32_t F_B11 : 1;                // 1 bit
        uint32_t F_B12 : 1;                // 1 bit
        uint32_t F_B13 : 1;                // 1 bit
        uint32_t F_B14 : 1;                // 1 bit
        uint32_t F_B15 : 1;                // 1 bit

        uint32_t F_B16 : 1;                // 1 bit
        uint32_t F_B17 : 1;                // 1 bit
        uint32_t F_B18 : 1;                // 1 bit
        uint32_t F_B19 : 1;                // 1 bit
        uint32_t F_B20 : 1;                // 1 bit
        uint32_t F_B21 : 1;                // 1 bit
        uint32_t F_B22 : 1;                // 1 bit
        uint32_t F_B23 : 1;                // 1 bit

        uint32_t F_B24 : 1;                // 1 bit
        uint32_t F_B25 : 1;                // 1 bit
        uint32_t F_B26 : 1;                // 1 bit
        uint32_t F_B27 : 1;                // 1 bit
        uint32_t F_B28 : 1;                // 1 bit
        uint32_t F_B29 : 1;                // 1 bit
        uint32_t F_B30 : 1;                // 1 bit
        uint32_t F_B31 : 1;                // 1 bit
    } BITS;
} EVENT_BYTE_T; // event事件处理标志


#ifdef __cplusplus
}
#endif
#endif
