#ifndef _DRIVERS_VAR_H_
#define _DRIVERS_VAR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Inc/ProjDefine.h"
#include "Inc/typedef.h"
#include "bsp_def.h"

// 设备类型映射表
extern const device_type_map_t device_type_maps[];
extern const size_t device_map_size;


/* declaration --------------------------------------------------------*/
// 驱动映射
extern device_t delay_ds;

// 设备实例
extern delay_t delay;


/* Lists--- -----------------------------------------------------------*/
// 设备树 - 注册接口
// 设备名称（可任意），  设备实例，   设备类型结构体名称
#define DEVICE_MAP_X \
    X("void" ,            NULL,              "void") \
    X("delay",        delay_ds,           "delay_t")

#ifdef __cplusplus
}
#endif
#endif

