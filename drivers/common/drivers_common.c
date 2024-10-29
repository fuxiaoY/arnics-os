
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



// 设备描述符注册
bool dev_reg(const char *name, device_t *device)
{
    for (size_t i = 0; i < device_map_size; ++i)
    {
        if (strcmp(device_map[i].name, name) == 0)
        {
            device->ds = i; // 设置设备描述符索引
            device->device = device_map[i].device; //设备实例注册
            return true; // 注册成功
        }
    }
    return false; // 未找到设备 注册失败
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


/* demo *
void configure_devices()
{
    int new_baudrate = 9600;
    param_value_t uart_params[] = {
        {"baudrate", &new_baudrate},
    };
    dev_params_set("uart1", uart_params, sizeof(uart_params) / sizeof(param_value_t));

    GPIO_PinState new_pin_state = GPIO_PIN_SET;
    param_value_t io_params[] = {
        {"PinState", &new_pin_state},
    };
    dev_params_set("led", io_params, sizeof(io_params) / sizeof(param_value_t));
}

 * demo */
 
