#ifndef _DRIVERS_VAR_H_
#define _DRIVERS_VAR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../../Inc/ProjDefine.h"
#include "../../Inc/typedef.h"
#include "bsp_def.h"

// 设备类型映射表
extern const device_type_map_t device_type_maps[];
extern const size_t device_map_size;


/* declaration --------------------------------------------------------*/

// 驱动映射
extern device_t led0_ds; // led0
extern device_t led1_ds; // led1
extern device_t debug_ds; // debug串口

// 驱动实例
extern uart_t uart1;
extern io_t led0;
extern io_t led1;
/* Lists--- -----------------------------------------------------------*/
// 设备总表的 X-macro
// {设备名称（可任意），设备实例，设备类型结构体名称}
#define DEVICE_MAP_X \
    X("void", NULL, "void") \
    X("uart1", &uart1, "uart_t") \
    X("led0", &led0, "io_t") \
    X("led1", &led1, "io_t")





#ifdef __cplusplus
}
#endif
#endif
