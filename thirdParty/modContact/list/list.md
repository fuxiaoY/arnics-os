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
帧ID |超时时间 | 帧头识别字段 | 帧尾识别字段 | 帧错误识别字段 | 帧类型 | 打包&解析函数名  | 数据处理指针

```
CMD_ADD(CMD_CONSOLE_ID_REV, 2,  "$$COMX$$","*#*#",  NULL,  RecvSend,  RevFlow ,STICKY_VAR(&echo)),
```
**字节流协议**  
帧ID |超时时间 | 帧头识别字段 | 帧头识别字长| 帧尾识别字段 |帧尾识别字长| 帧错误识别字段 |帧错误识别字长| 帧类型 | 打包&解析函数名
```
CMD_HEX_ADD(CMD_CONSOLE_ID_REV2,   2,  &header,sizeof(header),      &tail,sizeof(header),       NULL,0,   RecvSend RevHexFlow),
```

## 粘帧处理
由于 mct 库存在粘帧自动处理机制，所以在处理粘帧时，pack / Analyze 函数会自动收到在 CMD_ADD 中定义的 STICKY 参数（如 STICKY_VAR(&echo) 或 STICKY_CB(get_echo)）作为数据指针，而不是主流程中传入的参数。
强烈建议拥有粘帧风险的帧均添加该字段！

粘帧处理提供以下两种获得粘帧数据指针的接口：

1. 变量模式：
```
static uint8_t echo;
CMD_ADD(CMD_CONSOLE_ID_REV, 2, "$$COMX$$", "*#*#", NULL, RecvSend, RevFlow, STICKY_VAR(&echo)),
```

2. 函数回调模式：
```
void* get_echo(void)
{
    return &echo;
}
CMD_ADD(CMD_CONSOLE_ID_REV, 2, "$$COMX$$", "*#*#", NULL, RecvSend, RevFlow, STICKY_CB(get_echo)),
```

说明：STICKY_VAR 和 STICKY_CB 在 CMD_HEX_ADD 中同样适用，用法一致。

## 协议适配层
1.构建tCmdApi结构体列表  
```
static bool cmd_revHandle(MctInstance *inst,void *para)
{
    command_t console_cmd = {0};
    return mct_console_execute(inst,NULL_CMD_SEEK,&console_cmd);
}

static const tCmdApi funList[] =
{
    {.id = CMD_REV_FLOW,   .fun = cmd_revHandle},
        
};

tCmdApi const *mctConsoleApiGet(void)
{
    return funList;
}
```
2.注册到mctList中  
const tModemList modemList[] =
    {
        {.name = "CONSOLE",    .api = mctConsoleApiGet},
    }

## 特别注意
1.在适配层中制定RevSend命令ID，将关闭粘帧处理机制，只处理请求的命令ID，自动忽略其他消息。
2.系统保留命令ID：NULL_CMD_SEEK,该命令适用于RevSend指令，调用该指令，则系统在RevSend模型中，将开启粘帧处理，并返回该指令的STICKY数据指针。