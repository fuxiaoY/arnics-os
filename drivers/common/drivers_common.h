#ifndef _DRIVERS_COMMON_H_
#define _DRIVERS_COMMON_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../../Inc/ProjDefine.h"
#include "../../Inc/typedef.h"


typedef struct
{
    int ds;       /* 设备号 */
    void *device; /* 设备指针 */
} device_t;


// 参数值结构体
typedef struct {
    const char *param_name; // 参数名称
    const void *value;      // 参数新值指针
} param_value_t;

// 设备参数映射表结构体
typedef struct {
    const char *param_name;
    size_t offset;
    size_t size;
} param_map_t;

// 设备类型映射结构体
typedef struct {
    const char *device_type;
    const param_map_t *param_map;
    size_t param_map_size;
} device_type_map_t;

// 设备条目结构体
typedef struct {
    const char *name;
    void *device;
    const char *device_type;
} device_map_t;




extern void *dev_get(int index);
extern bool dev_reg(const char *name, device_t *device);
extern int dev_params_set(device_t *device, const param_value_t *params, size_t param_count);

#ifdef __cplusplus
}
#endif
#endif
