/* information */
/**
  ******************************************************************************
  * @file           : drivers_common.h
  * @brief          : 设备驱动通用头文件
  * 
  * 该文件定义了设备驱动相关的宏定义、变量和函数声明。
  * 主要用于设备的打开、关闭、读写和控制操作。
  * 
  * @version        : 1.0.0
  * @date           : 2023-10-05
  ******************************************************************************

  * @attention
  *
  * Copyright (c) 2023 ARSTUDIO.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#ifndef _DRIVERS_COMMON_H_
#define _DRIVERS_COMMON_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include "../../Inc/ProjDefine.h"
#include "../../Inc/typedef.h"
typedef struct 
{
    int (*ds_open)(void *dev);
    int (*ds_close)(void *dev);
    int (*ds_read)(void *dev, void *buf, size_t count);
    int (*ds_write)(void *dev, void *buf, size_t count);
    int (*ds_ctl)(void *dev, int cmd, void *args);
    void (*ds_irq)(void *dev);
}dev_operations;

typedef struct 
{
    int ds;       /* 设备号 */
    void *device; /* 设备指针 */
    dev_operations *dev_ops; /* 设备操作函数 */
} device_t;



// 参数值结构体
typedef struct
{
    const char *param_name; // 参数名称
    const void *value;      // 参数新值指针
} param_value_t;

// 设备参数映射表结构体
typedef struct
{
    const char *param_name;
    size_t offset;
    size_t size;
} param_map_t;

// 设备类型映射结构体
typedef struct
{
    const char *device_type;
    const param_map_t *param_map;
    size_t param_map_size;
    dev_operations *dev_ops; /* 设备操作函数 */
} device_type_map_t;

// 设备条目结构体
typedef struct
{
    const char *name;
    void *device;
    const char *device_type;
} device_map_t;

extern void *dev_get(int index);
extern bool dev_reg(const char *name, device_t *device);
extern int dev_params_set(device_t *device, const param_value_t *params, size_t param_count);

// 用于生成唯一变量名的宏
#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)



/**
 * @macro DEV_PARAMS_SET
 * @brief 多个参数设置宏，使用局部变量的方式创建数组
 * 
 * @param dev 设备结构体指针
 * @param ... 可变参数列表，用于传递参数值
 */
#define DEV_PARAMS_SET(dev, ...)                                          \
    do {                                                                  \
        param_value_t CONCATENATE(params_, __LINE__)[sizeof((param_value_t[]){__VA_ARGS__}) / sizeof(param_value_t)] = {__VA_ARGS__}; \
        dev_params_set(dev, CONCATENATE(params_, __LINE__), sizeof(CONCATENATE(params_, __LINE__)) / sizeof(param_value_t)); \
    } while (0)
/**
 * @fn dev_open
 * @brief 打开设备
 * 
 * @param dev 设备结构体指针
 * @return 返回操作结果，0表示成功，负值表示错误
 */
extern int dev_open(device_t *dev);

/**
 * @fn dev_close
 * @brief 关闭设备
 * 
 * @param dev 设备结构体指针
 * @return 返回操作结果，0表示成功，负值表示错误
 */
extern int dev_close(device_t *dev);

/**
 * @fn dev_read
 * @brief 从设备读取数据
 * 
 * @param dev 设备结构体指针
 * @param buf 存储读取数据的缓冲区指针
 * @param count 要读取的数据长度
 * @return 返回实际读取的数据长度，负值表示错误
 */
extern int dev_read(device_t *dev, void *buf, size_t count);

/**
 * @fn dev_write
 * @brief 向设备写入数据
 * 
 * @param dev 设备结构体指针
 * @param buf 要写入的数据缓冲区指针
 * @param count 要写入的数据长度
 * @return 返回实际写入的数据长度，负值表示错误
 */
extern int dev_write(device_t *dev, void *buf, size_t count);

/**
 * @fn dev_ctl
 * @brief 控制设备
 * 
 * @param dev 设备结构体指针
 * @param cmd 控制命令
 * @param args 控制参数
 * @return 返回操作结果，0表示成功，负值表示错误
 */
extern int dev_ctl(device_t *dev, int cmd, void *args);
/**
 * @fn dev_irq
 * @brief 设备中断
 * 
 * @param dev 设备结构体指针
 */
extern void dev_irq(device_t *dev);

#ifdef __cplusplus
}
#endif
#endif
/************************ (C) COPYRIGHT ARSTUDIO *****END OF FILE***************************/
