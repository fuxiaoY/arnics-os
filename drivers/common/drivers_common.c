
#include "drivers_common.h"
#include "drivers_list.h"
/*--------------------------------------------------------------------*/
/*-向南接口------------------------------------------------------------*/
/*--------------------------------------------------------------------*/


/*---------------------------------------------------------------------*/
/*-向北接口-------------------------------------------------------------*/
/*---------------------------------------------------------------------*/

// 通用设置函数
static int set_params(void *device, const param_map_t *param_map, size_t map_size, const param_value_t *params, size_t param_count)
{
    for (size_t j = 0; j < param_count; ++j)
    {
        int found = 0;
        for (size_t i = 0; i < map_size; ++i)
        {
            if (strcmp(param_map[i].param_name, params[j].param_name) == 0)
            {
                // 根据偏移量设置参数值
                void *param_ptr = (uint8_t *)device + param_map[i].offset;
                memcpy(param_ptr, params[j].value, param_map[i].size);
                found = 1;
                break;
            }
        }
        if (!found)
            return -1; // 参数未找到
    }
    return 0; // 所有参数设置成功
}






// 设备参数设置函数
int dev_params_set(device_t *dev, const param_value_t *params, size_t param_count)
{
    if((0 == dev->ds) || (dev->ds >= device_map_size))
    {
        return -1; // 设备错误
    }

    void *device = device_map[dev->ds].device;
    const char *device_type = device_map[dev->ds].device_type;

    // 查找相应的参数映射表
    for (size_t j = 0; j < device_type_maps_size; ++j)
    {
        if (strcmp(device_type_maps[j].device_type, device_type) == 0)
        {
            return set_params(device, device_type_maps[j].param_map,
                                device_type_maps[j].param_map_size, params, param_count);
        }
    }
    return -1; // 未找到设备类型

}
/*---------------------------------------------------------------------*/
/*-API-----------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
// 参数设置
/* demo *
    // 使用宏
    GPIO_PinState new_pin_state = GPIO_PIN_SET;
    uint32_t new_pin_mode = GPIO_MODE_OUTPUT_OD;


    DEV_PARAMS_SET(&led1_ds,
        {"PinState", &new_pin_state},
        {"GPIO_Mode", &new_pin_mode}
    );
 * demo */

// 设备描述符注册
bool dev_reg(const char *name, device_t *device)
{
    for (size_t i = 0; i < device_map_size; ++i)
    {
        if (strcmp(device_map[i].name, name) == 0)
        {
            device->ds = i; // 设置设备描述符索引
            device->device = device_map[i].device; //设备实例注册
            // 设备操作函数注册
            {
                // 查找相应的参数映射表
                for (size_t j = 0; j < device_type_maps_size; ++j)
                {
                    if (strcmp(device_type_maps[j].device_type, device_map[i].device_type) == 0)
                    {
                        device->dev_ops = device_type_maps[j].dev_ops;
                        break;
                    }
                } 
            }
            return true; // 注册成功
        }
    }
    return false; // 未找到设备 注册失败
}

/**
 * @fn dev_open
 * @brief 打开设备
 * 
 * @param dev 设备结构体指针
 * @return 返回操作结果，0表示成功，负值表示错误
 */
int dev_open(device_t *dev)
{
    return dev->dev_ops->ds_open(dev);
}

/**
 * @fn dev_close
 * @brief 关闭设备
 * 
 * @param dev 设备结构体指针
 * @return 返回操作结果，0表示成功，负值表示错误
 */
int dev_close(device_t *dev)
{
    return dev->dev_ops->ds_close(dev);
}

/**
 * @fn dev_read
 * @brief 从设备读取数据
 * 
 * @param dev 设备结构体指针
 * @param buf 存储读取数据的缓冲区指针
 * @param count 要读取的数据长度
 * @return 返回实际读取的数据长度，负值表示错误
 */
int dev_read(device_t *dev, void *buf, size_t count)
{
    return dev->dev_ops->ds_read(dev, buf, count);
}

/**
 * @fn dev_write
 * @brief 向设备写入数据
 * 
 * @param dev 设备结构体指针
 * @param buf 要写入的数据缓冲区指针
 * @param count 要写入的数据长度
 * @return 返回实际写入的数据长度，负值表示错误
 */
int dev_write(device_t *dev, void *buf, size_t count)
{
    return dev->dev_ops->ds_write(dev, buf, count);
}

/**
 * @fn dev_ctl
 * @brief 控制设备
 * 
 * @param dev 设备结构体指针
 * @param cmd 控制命令
 * @param args 控制参数
 * @return 返回操作结果，0表示成功，负值表示错误
 */
int dev_ctl(device_t *dev, int cmd, void *args)
{
    return dev->dev_ops->ds_ctl(dev, cmd, args);
}