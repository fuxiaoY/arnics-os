[English](list.en.md) | 中文

# mct实例接口适配
# 协议注册：
mct通过mctlist.c中的modemList[]协议列表进行注册，以识别出该实例所使用的协议。
```
const tModemList modemList[] =
{
    {.name = "CONSOLE",    .api = mctConsoleApiGet},
    {.name = "A7680C",     .api = mctA7680CApiGet},
    {.name = "Y7025",      .api = mctY7025ApiGet},
};
```
用户可添加任意数量协议到列表中供mct注册选择。
# 协议构造：
1.单个协议划分**协议层**和**协议适配层（Adapter）**  
2.添加到modmList[]中的为该协议的协议适配层。


![协议实现](../document/mctlist.svg)

新建协议文件夹到list目录，如某xprotocol协议：  
|-xprotocol  
&nbsp;&nbsp;|-command_xprotocol.c  
&nbsp;&nbsp;|-command_xprotocol.h  
&nbsp;&nbsp;|-command_xprotocolAdapter.c  
&nbsp;&nbsp;|-command_xprotocolAdapter.h  

## 协议层
协议层用于对指令格式添加打包和解析接口：  
假如拥有以下AT指令帧格式交互：
```
mcu关闭模块回显功能
mcu->mod : ATI0\r\n 

模块回复关闭回显功能成功
mod->mcu : OK\r\n ATI0:0\r\n
```

1.对指令格式添加打包和解析接口：
```
/**
 * @fn cmd_PackATcommand
 * @brief AT指令帧打包接口
 * 
 * @param [in] buf - 打包好的帧命令数据缓冲区指针
 * @param [in] len - 打包好的帧命令长度指针
 * @param [in] para - 来自用户的传参，用于参与帧格式打包流程
 * @retval None
 */
static bool cmd_PackATcommand(uint8_t* buf, size_t *len, void *para)
{
    uint8_t *echo = (uint8_t *)para;
    //对buf进行ATE<X>格式打包，其中X为echo
    return true;
}
```
```
/**
 * @fn cmd_PackATcommand
 * @brief AT指令帧打包接口
 * 
 * @param [in] buf - 打包好的帧命令数据缓冲区指针
 * @param [in] len - 打包好的帧命令长度指针
 * @param [in] para - 返回给用户的应用数据解析值
 * @retval None
 */
static bool cmd_AnalyzeATcommand(uint8_t* buf, size_t len, void *para)
{
    uint8_t *echo = (uint8_t *)para;
    //对buf中的len字节使用预期格式-ATE:<X>进行解析，将X赋值给echo
    return true;
}
```
2.构造协议层指令接口：  
协议使用**tCmd** 结构体来描述，单一指令参数定义如下：  
**ASCII协议**
帧ID |超时时间 | 帧头识别字段 | 帧尾识别字段 | 帧错误识别字段 | 帧类型 | 打包&解析函数名   

```
CMD_ADD(CMD_CONSOLE_ID_REV, 2,  "$$COMX$$","*#*#",  NULL,  RecvSend,  RevFlow),
```
**字节流协议**
帧ID |超时时间 | 帧头识别字段 | 帧头识别字长| 帧尾识别字段 |帧尾识别字长| 帧错误识别字段 |帧错误识别字长| 帧类型 | 打包&解析函数名
```
CMD_HEX_ADD(CMD_CONSOLE_ID_REV2,   2,  &header,sizeof(header),      &tail,sizeof(header),       NULL,0,   RecvSend RevHexFlow),
```
