/**
 *******************************************************************************
 * @file     $command_Y7025.c$
 * @author
 * @version  ver 1
 * @date     July 18th 2019
 * @build
 * @brief    command_A7680C Operation
 * @attention
 *
 * cmd_Pack: cb->cmd：打包好的指令  *para 用于打包的参数
 * cmd_Analy:buf：模块回复的原始报文  *para解析后的出参
 * @note
 *
 * 
 *******************************************************************************
 */
#include "command_Y7025.h"
#include "GlobalData.h"
#include "GlobalDataManager.h"
static bool cmd_PackEchoOff(uint8_t* buf, size_t* len, void *para)
{
    *len = sprintf((char*)buf, "ATE0\r\n");
    return true;
}

static bool cmd_AnalyzeEchoOff(uint8_t* buf, size_t len, void *para)
{
    return true;
}
/*
send:
AT+CGSN=1
receive: len:31

+CGSN:866808067860378

OK
*/
static bool cmd_PackImeiGet(uint8_t* buf, size_t* len, void *para)
{
    *len = sprintf((char*)buf, "AT+CGSN=1\r\n");
    return true;
}
static bool cmd_AnalyzeImeiGet(uint8_t* buf, size_t len, void *para)
{
    char *imei = (char *)para;
    uint8_t *ptr = (uint8_t *)buf;

    // 找到 "+CGSN:" 的位置
    ptr = hexhex((uint8_t *)buf, (uint8_t *)"+CGSN:", len, strlen("+CGSN:"));
    if (ptr == NULL)
    {
        return false;
    }

    // 移动指针到 "CCLK:" 之后的位置
    ptr += strlen("+CGSN:");

    // 拷贝后续到字符串
    memcpy(imei,ptr, 15);
    return true;
}

/*
send:
AT+CIMI


receive: len:31

+CIMI:460046931313394

OK
*/
static bool cmd_PackImsiGet(uint8_t* buf, size_t* len, void *para)
{
    *len = sprintf((char*)buf, "AT+CIMI\r\n");
    return true;
}

static bool cmd_AnalyzeImsiGet(uint8_t* buf, size_t len, void *para)
{
    char *imsi = (char *)para;
    uint8_t *ptr = (uint8_t *)buf;

    // 找到 "+CIMI:" 的位置
    ptr = hexhex((uint8_t *)buf, (uint8_t *)"+CIMI:", len, strlen("+CIMI:"));
    if (ptr == NULL)
    {
        return false;
    }

    // 移动指针到 "CCLK:" 之后的位置
    ptr += strlen("+CIMI:");

    // 拷贝后续到字符串
    memcpy(imsi,ptr, 15);
    return true;
}
/*
send:
AT+CGMR


receive: len:20
2112B03Y7025

OK
 
*/
static bool cmd_PackVersionGet(uint8_t* buf, size_t* len, void *para)
{
    *len = sprintf((char*)buf, "AT+CGMR\r\n");
    return true;
}

static bool cmd_AnalyzeVersionGet(uint8_t* buf, size_t len, void *para)
{
    char *ver = (char *)para;
    uint8_t *ptr = (uint8_t *)buf;
    // 拷贝后续到字符串
    memcpy(ver,ptr, 12);
    return true;
}



/*
AT+NCCID
+NCCID:89861118216007272115
OK
*/
static bool cmd_PackICCIDGet(uint8_t* buf, size_t* len, void *para)
{
    *len = sprintf((char*)buf, "AT+NCCID\r\n");
    return true;
}

static bool cmd_AnalyzeICCIDGet(uint8_t* buf, size_t len, void *para)
{
    char *iccid = (char *)para;
    uint8_t *ptr = (uint8_t *)buf;

    // 找到 "+NCCID:" 的位置
    ptr = hexhex((uint8_t *)buf, (uint8_t *)"+NCCID:", len, strlen("+NCCID:"));
    if (ptr == NULL)
    {
        return false;
    }

    // 移动指针到 "+NCCID:" 之后的位置
    ptr += strlen("+NCCID:");

    // 拷贝后续到字符串
    memcpy(iccid,ptr, 20);
    return true;
}


static bool cmd_PackPincodeInquire(uint8_t* buf, size_t* len, void *para)
{
    *len = sprintf((char*)buf, "AT+CPIN?\r\n");
    return true;
}

static bool cmd_AnalyzePincodeInquire(uint8_t* buf, size_t len, void *para)
{
    ePincode *code = (ePincode *)para;
    uint8_t *ptr = (uint8_t *)buf;
    uint8_t *str = NULL;

    if (cmd_TokenStart(&ptr, "CPIN:") < 0)
    {
        return false;
    }
    if (cmd_TokenNextStr(&ptr, &str) < 0)
    {
        return false;
    }
    if (0 == memcmp(str, "READY", strlen("READY")))
    {
        *code = PIN_OK;
    }
    else if (0 == memcmp(str, "SIM PIN", strlen("SIM PIN")))
    {
        *code = PIN_INPUT;
    }
    else if (0 == memcmp(str, "SIM PUK", strlen("SIM PUK")))
    {
        *code = PIN_INPUT;
    }
    else
    {
        *code = PIN_ERROR;
    }

    return true;
}

static bool cmd_PackPincodeSet(uint8_t* buf, size_t* len, void *para)
{
    uint16_t *data = (uint16_t *)para;
    *len = sprintf((char*)buf, "AT+CPIN=%d\r\n", *data);
    return true;
}

static bool cmd_AnalyzePincodeSet(uint8_t* buf, size_t len, void *para)
{
    return true;
}

static bool cmd_PackWorklockSet(uint8_t* buf, size_t* len, void *para)
{
    uint16_t *data = (uint16_t *)para;
    *len = sprintf((char*)buf, "AT+WORKLOCK=%d\r\n", *data);
    return true;
}

static bool cmd_AnalyzeWorklockSet(uint8_t* buf, size_t len, void *para)
{
    return true;
}

static bool cmd_PackCPSMSSet(uint8_t* buf, size_t* len, void *para)
{
    // 设置PSM参数，此处将T3412设置为60小时，意思是只要在60小时内至少有一次上报，就不用重新注网。T3324设置为2秒，意思是解除DTR休眠锁后延迟2秒进入PSM休眠
    *len = sprintf((char*)buf, "AT+CPSMS=1,,,00100100,01000011\r\n");
    return true;
}

static bool cmd_AnalyzeCPSMSSet(uint8_t* buf, size_t len, void *para)
{
    return true;
}

static bool cmd_PackCEREGSet(uint8_t* buf, size_t* len, void *para)
{
    uint16_t *data = (uint16_t *)para;
    *len = sprintf((char*)buf, "AT+CEREG=%d\r\n", *data);
    return true;
}

static bool cmd_AnalyzeCEREGSet(uint8_t* buf, size_t len, void *para)
{
    return true;
}
static bool cmd_PackRegister(uint8_t* buf, size_t* len, void *para)
{
    *len = sprintf((char*)buf, "AT+CEREG?\r\n");
    return true;
}

static bool cmd_AnalyzeRegister(uint8_t* buf, size_t len, void *para)
{
    uint8_t *data = (uint8_t *)para;
    uint8_t *ptr = (uint8_t *)buf;
    int value = 0;
    if (cmd_TokenStart(&ptr, "CEREG:") < 0)
    {
        return false;
    }
    if (cmd_TokenNextInt(&ptr, &value, 10) < 0)
    {
        return false;
    }
    if (cmd_TokenNextInt(&ptr, &value, 10) < 0)
    {
        return false;
    }
    *data = value;

    return true;
}

static bool cmd_PackSignalGet(uint8_t* buf, size_t* len, void *para)
{
    *len = sprintf((char*)buf, "AT+CSQ\r\n");
    return true;
}

static bool cmd_AnalyzeSignalGet(uint8_t* buf, size_t len, void *para)
{
    tsignalQuality *data = (tsignalQuality *)para;
    uint8_t *ptr = (uint8_t *)buf;
    int value = NULL;

    if (cmd_TokenStart(&ptr, "CSQ:") < 0)
    {
        return false;
    }
    if (cmd_TokenNextInt(&ptr, &value, 10) < 0)
    {
        return false;
    }
    data->rssi = value;
    if (cmd_TokenNextInt(&ptr, &value, 10) < 0)
    {
        return false;
    }
    data->ber = value;

    return true;
}

static bool cmd_PackPowerOff(uint8_t* buf, size_t* len, void *para)
{
    *len = sprintf((char*)buf, "AT+FASTOFF=0\r\n");
    return true;
}

static bool cmd_AnalyzePowerOff(uint8_t* buf, size_t len, void *para)
{
    return true;
}

static bool cmd_PackClockGet(uint8_t* buf, size_t* len, void *para)
{
    *len = sprintf((char*)buf, "AT+CCLK?\r\n");
    return true;
}
//+CCLK:24/08/28,10:42:56+32
static bool cmd_AnalyzeClockGet(uint8_t* buf, size_t len, void *para)
{
    tWanClock *clock = (tWanClock *)para;
    uint8_t *ptr = (uint8_t *)buf;
    uint8_t *str = NULL;
    int tep[7];

    // 清空 tep 数组
    memset(tep, 0, sizeof(int) * 7);

    // 找到 "CCLK:" 的位置
    ptr = hexhex((uint8_t *)buf, (uint8_t *)"CCLK:", len, strlen("CCLK:"));
    if (ptr == NULL)
    {
        return false;
    }

    // 移动指针到 "CCLK:" 之后的位置
    ptr += strlen("CCLK:");

    // 获取日期和时间字符串
    if (cmd_TokenNextStr(&ptr, &str) < 0)
    {
        return false;
    }

    // 解析日期和时间字符串
    char *date_time_str = (char *)str;
    int year, month, day, hour, minute, second, zone;

    // 解析年份
    sscanf(date_time_str, "%02d", &year);
    date_time_str += 3; // 跳过 "/"

    // 解析月份
    sscanf(date_time_str, "%02d", &month);
    date_time_str += 3; // 跳过 "/"

    // 解析日
    sscanf(date_time_str, "%02d", &day);
    date_time_str += 3; // 跳过 ","

    // 解析小时
    sscanf(date_time_str, "%02d", &hour);
    date_time_str += 3; // 跳过 ":"

    // 解析分钟
    sscanf(date_time_str, "%02d", &minute);
    date_time_str += 3; // 跳过 ":"

    // 解析秒
    sscanf(date_time_str, "%02d", &second);
    date_time_str += 3; // 跳过 "+"

    // 解析时区
    sscanf(date_time_str, "%d", &zone);

    // 检查各个字段是否在合理范围内
    if (year < 0 || year > 99) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    if (hour < 0 || hour > 23) return false;
    if (minute < 0 || minute > 59) return false;
    if (second < 0 || second > 59) return false;


    // 设置时间数据
    clock->clock.year_h = (year / 100); // 年份高位
    clock->clock.year_l = (year % 100); // 年份低位
    clock->clock.month = (uint8_t)month;
    clock->clock.day = (uint8_t)day;
    clock->clock.week = 0xFF; // 星期暂不处理
    clock->clock.hour = (uint8_t)hour + 8;//+8时区
    clock->clock.min = (uint8_t)minute;
    clock->clock.sec = (uint8_t)second;
    clock->clock.hund = 0xFF; // 百分之一秒暂不处理
    clock->clock.dev_h = 0x80;
    clock->clock.dev_l = 0x00;
    clock->clock.status = 0x00;
    clock->zone = zone;

    return true;
}
/*
 *MQTT打包
 */

/*
 * cmd_Pack: cb->cmd：打包好的指令  *para 用于打包的参数
 * cmd_Analy:buf：模块回复的原始报文  *para解析后的出参
 */
static uint8_t MQTT_ID = 0; // 默认客户端
static bool cmd_PackMqttNew(uint8_t* buf, size_t* len, void *para)
{
    // AT+MQNEW=mqtt.ctwing.cn,1883,5000,200
    uint16_t buffersize = 1000;
    mqttConnet_t *mqttConnetpara = (mqttConnet_t *)para;
    *len = sprintf((char*)buf, "AT+MQNEW=%s,%u,%u,%u\r\n", mqttConnetpara->Serverip, mqttConnetpara->ServerPort, mqttConnetpara->ConnectTimeOut, buffersize);
    return true;
}

//+MQNEW:0 解析出冒号后面的0

static bool cmd_AnalyzeMqttNew(uint8_t* buf, size_t len, void *para)
{
    // 定义字符串前缀
    const char *prefix = "+MQNEW:";
    size_t prefixLen = strlen(prefix);

    // 获取接收到的命令字符串
    const char *ptr = (const char *)buf;

    // 查找前缀在字符串中的位置
    const char *prefixPos = strstr(ptr, prefix);

    if (prefixPos != NULL)
    {
        // 前缀找到了，获取冒号后面的内容
        const char *dataStart = strstr(prefixPos, ":") + 1; // 定位到冒号后面的第一个字符

        // 提取冒号后面的第一位数字
        char firstDigit[2];
        firstDigit[0] = dataStart[0];
        firstDigit[1] = '\0'; // 终止符

        // 使用 atoi 提取整数
        int value = atoi(firstDigit);

        // 判断冒号后的值是否与 MQTT_ID 相同
        if (value == MQTT_ID)
        {
            return true;
        }
    }

    // 如果前缀不匹配或值不相同，返回 false
    return false;
}

static bool cmd_PackMqttConnect(uint8_t* buf, size_t* len, void *para)
{

    // mqtt链接id  //客户端ID 字符串  //keepalive  //用户名  //密码
    *len = sprintf((char*)buf, "AT+MQCON=%u,4,\"%s\",%u,1,0,,,,,,\"%s\",\"%s\"\r\n",
                         MQTT_ID,
                         g_RTCfg.ProductKey,
                         g_RTCfg.KeepAlive,
                         g_RTCfg.DevName,
                         g_RTCfg.DevSecret);

    return true;
}
//+MQCONNACK:0,0
static bool cmd_AnalyzeMqttConnect(uint8_t* buf, size_t len, void *para)
{
    // 定义字符串前缀
    const char *prefix = "+MQCONNACK:";
    size_t prefixLen = strlen(prefix);

    // 获取接收到的命令字符串
    const char *ptr = (const char *)buf;

    // 查找前缀在字符串中的位置
    const char *prefixPos = strstr(ptr, prefix);

    if (prefixPos != NULL)
    {
        // 前缀找到了，获取冒号后面的内容
        const char *dataStart = strstr(prefixPos, ":") + 1; // 定位到冒号后面的第一个字符

        // 提取冒号后面的第一位数字
        char firstDigit[2];
        firstDigit[0] = dataStart[0];
        firstDigit[1] = '\0'; // 终止符

        // 使用 atoi 提取整数
        int param1 = atoi(firstDigit);

        // 提取逗号后面的第一位数字
        firstDigit[0] = dataStart[2];
        firstDigit[1] = '\0'; // 终止符
        // 使用 atoi 提取整数
        int param2 = atoi(firstDigit);

        // 判断两个参数是否都等于0
        if (param1 == MQTT_ID && param2 == 0)
        {

            return true;
        }
        printf("Y7025: MQTT refuse: %d\r\n", param2);
    }


// 如果前缀不匹配或值不相同，返回 false
return false;
}

/*
    QoS 0，最多交付一次。
    QoS 1，至少交付一次。
    QoS 2，只交付一次。
*/
static bool cmd_PackMqttSubscrib(uint8_t* buf, size_t* len, void *para)
{
    char *Topic = (char *)para;
    uint8_t QOS = 1;
    *len = sprintf((char*)buf, "AT+MQSUB=%u,\"%s\",%d\r\n", MQTT_ID, Topic, QOS);
    return true;
}

//+MQSUBACK:<MQTT_ID>,<Topic>,<Qos>
static bool cmd_AnalyzeMqttSubscrib(uint8_t* buf, size_t len, void *para)
{
    // 定义字符串前缀
    const char *prefix = "+MQSUBACK:";
    size_t prefixLen = strlen(prefix);

    // 获取接收到的命令字符串
    const char *ptr = (const char *)buf;

    // 查找前缀在字符串中的位置
    const char *prefixPos = strstr(ptr, prefix);

    if (prefixPos != NULL)
    {
        // 前缀找到了，获取冒号后面的内容
        const char *dataStart = strstr(prefixPos, ":") + 1; // 定位到冒号后面的第一个字符

        // 提取冒号后面的第一位数字
        char firstDigit[2];
        firstDigit[0] = dataStart[0];
        firstDigit[1] = '\0'; // 终止符

        // 使用 atoi 提取整数
        int param1 = atoi(firstDigit);

        // 将MQTT_ID存储到传递的变量中
        // *((int *)para) = mqttID;
        if (param1 == MQTT_ID)
            return true;
        
    }

    // 如果前缀不匹配或解析失败，返回false
    return false;
}
/*
para->
typedef struct
{
char* Topic;
int QOS;
int PublishLen;
char* PublishData;
}MqttPulish_t;

*/
/*
static bool cmd_PackMqttPublish(uint8_t* buf, size_t* len, void *para)
{
    MqttPulish_t *mattPub = (MqttPulish_t *)para;
    *len = sprintf((char*)buf, "AT+MQPUB=%u,\"%s\",%u,0,0,%u,%s\r\n", MQTT_ID, mattPub->Topic, mattPub->QOS, mattPub->PublishLen, mattPub->PublishData);
    return true;
}
*/

static bool cmd_PackMqttPublish(uint8_t* buf, size_t* len, void *para)
{
    MqttPulish_t *mattPub = (MqttPulish_t *)para;

    // Format the command with the PublishData directly
    uint16_t startLen = sprintf((char*)buf, "AT+MQPUB=%u,\"%s\",%u,0,0,%u,",
                         MQTT_ID, mattPub->Topic, mattPub->QOS, mattPub->PublishLen);
    // Convert and append the data as hexadecimal
    const char *hexChars = "0123456789abcdef";
    for (size_t i = 0; i < mattPub->PublishLen; ++i) {
        unsigned char byte = ((unsigned char*)mattPub->PublishData)[i];
        buf[startLen++] = hexChars[byte >> 4];
        buf[startLen++] = hexChars[byte & 0x0f];
    }

    // Null terminate the command
    buf[startLen] = '\r';
    buf[startLen + 1] = '\n';
    startLen+=2;
    *len = startLen;
    //buf[*len + 2] = '\0';

    return true;
}
//MQPUBACK:0
static bool cmd_AnalyzeMqttPublish(uint8_t* buf, size_t len, void *para)
{

    // 定义字符串前缀
    const char *prefix = "+MQPUBACK:";
    size_t prefixLen = strlen(prefix);

    // 获取接收到的命令字符串
    const char *ptr = (const char *)buf;

    // 查找前缀在字符串中的位置
    const char *prefixPos = strstr(ptr, prefix);

    if (prefixPos != NULL)
    {
        // 前缀找到了，获取冒号后面的内容
        const char *dataStart = strstr(prefixPos, ":") + 1; // 定位到冒号后面的第一个字符
        // 提取冒号后面的第一位数字
        char firstDigit[2];
        firstDigit[0] = dataStart[0];
        firstDigit[1] = '\0'; // 终止符

        // 使用 atoi 提取整数
        int param1 = atoi(firstDigit);

        // 将MQTT_ID存储到传递的变量中
        if (param1 == MQTT_ID)
            return true;
    }
    return false;
}


static bool cmd_PackCTZEUGet_Receive(uint8_t* buf, size_t* len, void *para)
{
    *len = 0;
    //不打包，只接受数据
    return true;
}


// +CTZEU:+32,0,2024/08/29,09:06:08
static bool cmd_AnalyzeCTZEUGet_Receive(uint8_t* buf, size_t len, void *para)
{
    tWanClock *clock = (tWanClock *)para;
    uint8_t *ptr = (uint8_t *)buf;
    uint8_t *str = NULL;

    // 找到 "+CTZEU:" 的位置
    ptr = hexhex((uint8_t *)buf, (uint8_t *)"+CTZEU:", len, strlen("+CTZEU:"));
    if (ptr == NULL)
    {
        return false;
    }

    // 移动指针到 "+CTZEU:" 之后的位置
    ptr += strlen("+CTZEU:");

    // 获取日期和时间字符串
    if (cmd_TokenNextStr(&ptr, &str) < 0)
    {
        return false;
    }

    // 解析日期和时间字符串
    char *date_time_str = (char *)str;
    int year, month, day, hour, minute, second;
    date_time_str += 8; // 跳到后年两位
    // 解析年份
    sscanf(date_time_str, "%d", &year);
    date_time_str += 3; // 

    // 解析月份
    sscanf(date_time_str, "%d", &month);
    date_time_str += 3; // 跳过 "/"

    // 解析日
    sscanf(date_time_str, "%d", &day);
    date_time_str += 3; // 跳过 ","

    // 解析小时
    sscanf(date_time_str, "%d", &hour);
    date_time_str += 3; // 跳过 ":"

    // 解析分钟
    sscanf(date_time_str, "%d", &minute);
    date_time_str += 3; // 跳过 ":"

    // 解析秒
    sscanf(date_time_str, "%d", &second);

    // 检查各个字段是否在合理范围内
    if (year < 0 || year > 99) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    if (hour < 0 || hour > 23) return false;
    if (minute < 0 || minute > 59) return false;
    if (second < 0 || second > 59) return false;

    // 设置时间数据
    clock->clock.year_h = (year / 100); // 年份高位
    clock->clock.year_l = (year % 100); // 年份低位
    clock->clock.month = (uint8_t)month;
    clock->clock.day = (uint8_t)day;
    clock->clock.week = 0xFF; // 星期暂不处理
    clock->clock.hour = (uint8_t)hour + 8;//+8时区
    clock->clock.min = (uint8_t)minute;
    clock->clock.sec = (uint8_t)second;
    clock->clock.hund = 0xFF; // 百分之一秒暂不处理
    clock->clock.dev_h = 0x80;
    clock->clock.dev_l = 0x00;
    clock->clock.status = 0x00;
    clock->zone = 0xFF;

    return true;
}

tWanData g_WanData = {0};
static bool cmd_PackMqttReceive(uint8_t* buf, size_t* len, void *para)
{
    *len = 0;    //不打包，只接受数据
    tWanData *WanData = &g_WanData;

    if (1 == WanData->RevCmdValidFlag)
    {
        printf("\nwan:\r\n"); 
        printf("%.*s\n",256,(char *)WanData->RevCmd);
     
        switch(WanData->mqttType)
        {
            case topic_ConfigDn:
            {
                //拿走数据，此处不做打包处理，防止栈溢出
                memcpy(g_mqttWanBuf, WanData->RevCmd, strlen(WanData->RevCmd));
                g_mqttWanBuf_Ready = true;
                mqttWanPub.Topic = g_MQTopic.ConfigUp;
                mqttWanPub.QOS = 1;
            }
            break;
            case topic_Command:
            {
                //拿走数据，此处不做打包处理，防止栈溢出
                memcpy(g_mqttWanBuf, WanData->RevCmd, strlen(WanData->RevCmd));
                g_mqttWanBuf_Ready = true;
                mqttWanPub.Topic = g_MQTopic.Resp;
                mqttWanPub.QOS = 1;
            }
            break;
            default:
            break;
        }
    }

    
    return true;
}

/*
struct tWanData
{
    uint8_t   RevCmdValidFlag; //接收命令是否有效
    MqTType    mqttType; //MQTT类型
    uint16_t  RevCmdLen; //接收命令长度
    char   RevCmd[256]; //数据缓存
    MqttPulish_t mqttPub; //发布数据
};
*/
//+MQPUB:0,$PERSISTENCE/configDn/8212345678100343,0,0,0,17,7B224D656964615472616365223A20317D
static bool cmd_AnalyzeMqttReceive(uint8_t* buf, size_t len, void *para)
{
    tWanData *WanData = &g_WanData;
    memset(WanData,0,sizeof(tWanData));

    uint8_t *ptr = (uint8_t *)buf;
    uint8_t *str = NULL;
    // 找到 "+MQPUB:" 的位置
    ptr = hexhex((uint8_t *)buf, (uint8_t *)"+MQPUB:", len, strlen("+MQPUB:"));
    if (ptr == NULL)
    {
        return false;
    }

    // 移动指针到 "+MQPUB:" 之后的位置
    ptr += strlen("+MQPUB:");

    // 获取主题字符串 只做判断
    if (cmd_TokenNextStr(&ptr, &str) < 0)
    {
        return false;
    }
    str = ptr;
    // 提取主题
    uint8_t i= 0;
    uint8_t *strTopic = str;
   // 查找下一个逗号，偏移指针到数据位置
    while (*strTopic != ',')
    {
        strTopic++;
        i++;
        if(i > 100)
        {
            WanData->RevCmdValidFlag = 0; // 标记接收命令无效
            return false;//防止死循环，最多偏移3次
        }

    }

    // 提取主题
    char cache[256] = {0};
    memcpy(cache, str, strTopic - str);
    cache[strTopic - str] = '\0';

    // 检查主题中是否包含 "configDn" 或 "/command/"
    if (strstr(cache, "/configDn/"))
    {
        WanData->mqttType = topic_ConfigDn; // 下行配置主题
    }
    else if (strstr(cache, "/command/"))
    {
        WanData->mqttType = topic_Command; // 命令主题
    }
    else
    {
        WanData->mqttType = topic_Other; // 其他主题
        WanData->RevCmdValidFlag = 0; // 标记接收命令无效
        return false;
    }




    // 偏移指针到数据长度位置
    ptr += strlen(cache) + 1; // 移动到主题后面的逗号后
		ptr += 6; //跳到数据len
    char *data_len_pos = (char*)ptr;

    // 读取数据长度
    int data_length = 0;
    sscanf(data_len_pos, "%d", &data_length);

    // 检查数据长度是否正确
    if (data_length <= 0 || data_length > sizeof(WanData->RevCmd)-1)
    {
        WanData->RevCmdValidFlag = 0; // 标记接收命令无效
        return false;
    }

    // 存储数据长度
    WanData->RevCmdLen = data_length;

    // 偏移指针到数据位置
    i= 0;
   // 查找下一个逗号，偏移指针到数据位置
    while (*ptr != ',')
    {
        ptr++;
        i++;
        if(i>3)
        {
            WanData->RevCmdValidFlag = 0; // 标记接收命令无效
            return false;//防止死循环，最多偏移3次
        }

    }
    ptr++; // 跳过逗号

    // 提取并转换最后一个 HEX 字段为 ASCII
    memset(cache, 0, sizeof(cache));
    memcpy(cache, (const char *)ptr, data_length * 2);
    cache[data_length * 2] = '\0';

    char data_ascii[256];
    hexToAscii(cache, data_ascii, sizeof(data_ascii));

    // 存储转换后的 ASCII 数据
    snprintf(WanData->RevCmd, sizeof(WanData->RevCmd), "%s", data_ascii);
    WanData->RevCmd[data_length] = '\0';

    WanData->RevCmdValidFlag = 1; // 标记接收命令有效
    return true;
}
// 从buf指针头部开始匹配 顺序： errorPhase匹配->rightPhase匹配->SubRightPhase匹配->analyze解析指令是否成功
//                                            rightPhase    SubRightPhase  errorPhase
static const tCmd cmdList[] =
{
    CMD_ADD(CMD_Y7025_ECHO_OFF,             5,  "OK",          NULL,         "ERROR",     SendRev,       EchoOff),//关闭回显
    CMD_ADD(CMD_Y7025_IMEI_GET,             5,  "+CGSN:",     "\r\n",        "ERROR",     SendRev,       ImeiGet), //imsi
    CMD_ADD(CMD_Y7025_IMSI_GET,             5,  "+CIMI:",     "\r\n",        "ERROR",     SendRev,       ImsiGet), //imei
    CMD_ADD(CMD_Y7025_NCCID,                5,  "+NCCID:",    "\r\n",        "ERROR",     SendRev,       ICCIDGet), //iCCID
    CMD_ADD(CMD_Y7025_VERSION_GET,          5,  NULL,          NULL,        "ERROR",     SendRev,       VersionGet),
    CMD_ADD(CMD_Y7025_PINCODE_INQUIRE,      5,  "CPIN:READY",  NULL,         "ERROR",     SendRev,       PincodeInquire),//检查SIM卡的PIN码是否锁定
    CMD_ADD(CMD_Y7025_PINCODE_SET,          5,  "OK",          "\r\n",       "ERROR",     SendRev,       PincodeSet), //设置SIM卡的PIN码
    CMD_ADD(CMD_Y7025_WORKLOCK,             5,  "OK",          NULL,         "ERROR",     SendRev,       WorklockSet),//芯片工作锁
    CMD_ADD(CMD_Y7025_CPSMS,                5,  "OK",          NULL,         "ERROR",     SendRev,       CPSMSSet),  //在PSM模式下，终端设备会减少与网络的交互，从而降低功耗
    CMD_ADD(CMD_Y7025_CEREG,                5,  "OK",          NULL,         "ERROR",     SendRev,       CEREGSet),   //CEREG主动上报网络注册消息
    CMD_ADD(CMD_Y7025_DATA_REGISTER,        5,  NULL,          NULL,         "ERROR",     SendRev,       Register),   //网络注册情况
    CMD_ADD(CMD_Y7025_SIGNAL_GET,           5,  NULL,          NULL,         "ERROR",     SendRev,       SignalGet),  //rssi
    CMD_ADD(CMD_Y7025_POWER_OFF,            5,  NULL,          NULL,         "ERROR",     SendRev,       PowerOff),  //休眠
    CMD_ADD(CMD_Y7025_CLOCK_GET,            5, "CCLK:",        NULL,         "ERROR",     SendRev,       ClockGet),  //获取基站时钟
    CMD_ADD(CMD_Y7025_MQTTNEW,              10, "+MQNEW:",     NULL,         "ERROR",     SendRev,       MqttNew),  //MQTT
    CMD_ADD(CMD_Y7025_MQTTCON,              10, "+MQCONNACK:", "\r\n",       "ERROR",     SendRev,       MqttConnect),  //MQTT
    CMD_ADD(CMD_Y7025_MQTTSUB,              10, "+MQSUBACK:",  "\r\n",       "ERROR",     SendRev,       MqttSubscrib),  //MQTT
    CMD_ADD(CMD_Y7025_MQTTPUB,              10, "+MQPUBACK:",  "\r\n",       "ERROR",     SendRev,       MqttPublish),  //MQTT
    CMD_ADD(CMD_Y7025_MQTTREV,              1,  "+MQPUB:",     "\r\n",       NULL,        RecvSend,      MqttReceive),  //MQTT接收

};


tCmd const *CMD_Y7025CmdGet(void)
{
    return cmdList;
}

uint16_t CMD_Y7025CmdNumGet(void)
{
    return (sizeof(cmdList) / sizeof(tCmd));
}

/*********** End of file **/
