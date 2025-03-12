# modContact

## 简介：
modContact，（以下简称mct）是一款专门为嵌入式平台开发的通用串口交互平台框架，它广泛的支持任何形如(request<->response)的主从机交互协议，且内置优雅的主从机切换机制和稳定可靠的多帧识别，沾帧切割，沾帧解析机制。


与其他开源交互软件不同的是，mct基于了一种更通用的嵌入式串口交互的思路而进行设计：串口交互协议如（模组：AT指令，自定义串口协议）等是形如 问<->答 机制(request<->response)，凡是与此机制相似的交互协议，都可使用modContact来实现。


## 串口交互协议共性：
几乎所有串口交互协议的都是形如(request<->response)的主从机交互协议：通常由主机发起请求，从机返回该请求的响应。
如：AT指令集中，单片机发起AT命令，从机返回OK。
```
mcu->modem : AT
modem->mcu : OK
```

## modContact设计思路：
request<->response的主从机交互，在程序内部可抽象为以下行为:  
![帧交互行为](document/abstract.svg)

modContact将上述行为完全接管，对用户提供出帧格式打包和帧格式解析的回调函数，并提供向应用层提供统一接口:
 
### 接口说明：
modContact可创建多个实例，每个实例可以独立控制某路串口或某一类型协议，实现多协议同时运行。

#### 一、使用前用户需准备:  
1.两个数据缓冲区:  
协议命令打包缓存区和协议数据解析缓存区,两个缓存区用于对帧格式进行临时打包和接受到的帧数据进行解析，缓冲区分配大小需根据具体应用需求进行确定。 ，通常由两个数组实现：如 
 <font color=#00FFFF>cmd_cache[100],   payload_cache[100] </font> 

2.两个回调函数:  
<font color=#FF0000>write_callback: </font> 
用于将打包好的帧数据写入串口，write_callback由用户实现，用户需实现将buf中的len字节写入串口，并返回实际发送的字节数。  
<font color=#FF0000>read_callback: </font> 
用于从串口读取数据，read_callback由用户实现，用户需实现将预期长度（通常使用缓存区最大空间）数据读取到buf中，并返回读取实际读取到的字节数。


#### 二、使用准备好的缓冲区和回调函数，进行初始化：
##### 初始化函数：

```
/**
 * @fn mct_init
 * @brief mct实例初始化
 * 
 * @param [in] pInstance - mct协议实例指针
 * @param [in] Cmd_cache - 命令缓存区指针
 * @param [in] cmd_max_size - 命令缓存区大小
 * @param [in] Payload_cache - 解析数据缓存区指针
 * @param [in] payload_max_size - 解析数据缓存区大小
 * @param [in] write_callback - 写回调函数指针，用于发送数据
 * @param [in] read_callback - 读回调函数指针，用于接收数据
 * @retval None
 */
void mct_init(MctInstance* pInstance,uint8_t *Cmd_cache,size_t cmd_max_size, \
                                    uint8_t* Payload_cahe,size_t payload_max_size, \
                                    int (*write_callback)(void *file, uint16_t len), \
                                    uint32_t (*read_callback)(uint8_t *buf, uint16_t maxlen))
```

#### 三、注册该modContact实例上使用的协议：
##### 注册函数：
```
/**
 * @fn mct_reg
 * @brief mct协议注册
 * 
 * @param [in] name - 协议类型，见modemList[]
 * @param [in] inst - mct协议实例指针
 * @param [in] ModemAtuoRecogniton - 是否启用自动识别
 * @retval true - 注册成功 false - 注册失败
 */
 bool mct_reg(const char* name,MctInstance *inst,bool ModemAtuoRecogniton)
 ```


##### 注册方式1：
自动识别实例协议，适用于类似于AT指令集等带有标签识别的协议，（如AT指令中通过ATI0可查询模块相关信息）。mct提供AT指令模块自动识别模板，其余协议识别可由用户自行实现。
```
mct_reg(NULL,&modemInstance,true);
```

##### 注册方式2：
手动指定该modContact实例的协议，适用于自定义协议或不需要动态识别实例协议的情景。如指定"CONSOLE"为该实例协议
```
mct_reg("CONSOLE",&consoleInstance,false);
```



#### 四、调用接口：
使用mct进行串口交互的统一接口:
```
/**
 * @fn mctApiExecute
 * @brief mct执行交互API
 * 此函数用于根据命令ID查找并执行相应的命令函数。
 * 
 * @param [in] inst - mct实例指针
 * @param [in] id - 命令ID
 * @param [in] para - 用户数据指针接口
 * @retval true - 成功执行命令
 * @retval false - 执行失败
 */
mctApiExecute(&consoleInstance,CMD_REV_FLOW,NULL);
```
#### 五、适配接口：

参考list文件夹下list.md适配说明,为mct增加自定义协议。