/**
 *******************************************************************************
 * @file     $command_Y7025.c$
 * @author
 * @version  ver 1
 * @date     July 18th 2019
 * @build 
 * @brief    command_Y7025 Operation
 * @attention
 *
 * @note
 *
 *
 *******************************************************************************
 */
#include "command_Y7025Adapter.h"
#include "../../../ulog/ulogDef.h"
static bool cmd_GetZTZEU(MctInstance* inst,void *para)
{
    tWanClock *clock = (tWanClock*)para;
    if (true == mct_y7025_execute(inst,  CMD_Y7025_BOOTZTZEU, clock))
    {
        return true;
    }
    else
    {
        memset(clock, 0, sizeof(tWanClock));
        ULOG_INFO("Y7025: GetZTZEU failed:");
        return false;
    }
}


static bool cmd_WorklockSet(MctInstance* inst,void *para)
{
    if (false == mct_y7025_execute(inst,  CMD_Y7025_WORKLOCK, para))
    {
        ULOG_INFO("Y7025: CMD_Y7025_WORKLOCK failed:");
        return false;
    }
    else
    {
        return true;
    }
}

// 关闭回显
static bool cmd_EchoClose(MctInstance* inst,void *para)
{
    if (false == mct_y7025_execute(inst,  CMD_Y7025_ECHO_OFF, NULL))
    {
        ULOG_INFO("Y7025: Cmd CMD_Y7025_ECHO_OFF failed:");
    }
    return true;
}

static bool cmd_ModemInfoGet(MctInstance* inst,void *para)
{

    memset(MCT_PTR(wan_imei), 0, sizeof(MCT_PTR(wan_imei)));
    if (false == mct_y7025_execute(inst,  CMD_Y7025_IMEI_GET, MCT_PTR(wan_imei)))
    {
        ULOG_INFO("Y7025: CMD_Y7025_IMEI_GET failed:");
    }
    else
    {
        ULOG_INFO("Y7025: IMEI:%s", MCT_PTR(wan_imei));
    }

    memset(MCT_PTR(wan_imsi), 0, sizeof(MCT_PTR(wan_imsi)));
    if (false == mct_y7025_execute(inst,  CMD_Y7025_IMSI_GET, MCT_PTR(wan_imsi)))
    {
        ULOG_INFO("Y7025: CMD_Y7025_IMEI_GET failed:");
    }
    else
    {
        ULOG_INFO("Y7025: IMSI:%s", MCT_PTR(wan_imsi));
    }
    memset(MCT_PTR(wan_ver), 0, sizeof(MCT_PTR(wan_ver)));
    if (false == mct_y7025_execute(inst,  CMD_Y7025_VERSION_GET, MCT_PTR(wan_ver)))
    {
        ULOG_INFO("Y7025: CMD_Y7025_IMEI_GET failed:");
    }
    else
    {
        ULOG_INFO("Y7025: ver:%s", MCT_PTR(wan_ver));
    }
    memset(MCT_PTR(wan_iccid), 0, sizeof(MCT_PTR(wan_iccid)));
    if (false == mct_y7025_execute(inst,  CMD_Y7025_NCCID, MCT_PTR(wan_iccid)))
    {
        ULOG_INFO("Y7025: CMD_Y7025_IMEI_GET failed:");
    }
    else
    {
        ULOG_INFO("Y7025: nccid:%s", MCT_PTR(wan_iccid));
    }
    return true;
}

static bool cmd_PincodeGet(MctInstance* inst,void *para)
{
    ePincode status;
    if (false == mct_y7025_execute(inst,  CMD_Y7025_PINCODE_INQUIRE, (void *)&status))
    {
        ULOG_INFO("Y7025: CMD_Y7025_PINCODE_INQUIRE failed:");
        return false;
    }
    else
    {
        ULOG_INFO("Y7025: pincode:%d", status);
        return true;
    }
}
static bool cmd_PincodeSet(MctInstance* inst,void *para)
{
    ePincode status;
    tPincode code;

    if (false == mct_y7025_execute(inst,  CMD_Y7025_PINCODE_INQUIRE, (void *)&status))
    {
        ULOG_INFO("Y7025: CMD_Y7025_PINCODE_INQUIRE failed:");
        return false;
    }
    if (PIN_ERROR == status)
    {
        return false;
    }
    if (PIN_INPUT == status)
    {
        // 设置值
        code.data = 1;
        return mct_y7025_execute(inst,  CMD_Y7025_PINCODE_SET, (void *)&code.data);
    }
    return true;
}

// 主动上报网络注册消息
static bool cmd_CEREGset(MctInstance* inst,void *para)
{
    if (false == mct_y7025_execute(inst,  CMD_Y7025_CEREG, para))
    {
        ULOG_INFO("Y7025: CMD_Y7025_CEREG failed:");
        return false;
    }
    else
    {
        return true;
    }
}
// 查看信号强度
static bool cmd_ModemUpdateSignal(MctInstance* inst,void *para)
{
    NetworkPara_t* networkPara = (NetworkPara_t*)para;

    tsignalQuality signal;

    memset((void *)&signal, 0x00, sizeof(tsignalQuality));
    if (false == mct_y7025_execute(inst,  CMD_Y7025_SIGNAL_GET, &signal))
    {
        ULOG_INFO("Y7025: CMD_Y7025_SIGNAL_GET failed:");
        return false;
    }
    else
    {
        networkPara->SignalStrength = signal.rssi;
    }

    return true;
}
// 查看注网状态
static bool cmd_CheckCEREG(MctInstance* inst,void *para)
{
    NetworkPara_t* networkPara = (NetworkPara_t*)para;
    tRegStus reg;
    if (false == mct_y7025_execute(inst,  CMD_Y7025_DATA_REGISTER, &reg.value))
    {
        ULOG_INFO("Y7025: CMD_Y7025_DATA_REGISTER failed:");
        return false;
    }
    else
    {
        networkPara->REGstatus = reg.value;
    }
    return true;
}
// 配置PSM模式下，终端设备会减少与网络的交互
// 设置PSM参数，此处将T3412设置为60小时
// 意思是只要在60小时内至少有一次上报，就不用重新注网。T3324设置为2秒，意思是解除DTR休眠锁后延迟2秒进入PSM休眠
static bool cmd_CPSMset(MctInstance* inst,void *para)
{

    if (false == mct_y7025_execute(inst,  CMD_Y7025_CPSMS, NULL))
    {
        ULOG_INFO("Y7025: CMD_Y7025_CPSMS failed:");
        return false;
    }
    else
    {
        return true;
    }
}

static bool cmd_powerOff(MctInstance* inst,void *para)
{
    if (false == mct_y7025_execute(inst,  CMD_Y7025_POWER_OFF, NULL))
    {
        ULOG_INFO("Y7025: CMD_Y7025_POWER_OFF failed:");
        return false;
    }
    else
    {
        return true;
    }
}

static bool cmd_autoConfig(MctInstance* inst,void *para)
{
    uint8_t MAX_RETRIES = 3;

    // 设置worklock
    uint16_t WORKLOCK_status = 0;
    bool worklockSuccess = false;
    for (int i = 0; i < MAX_RETRIES; i++) {
        worklockSuccess = cmd_WorklockSet(inst,&WORKLOCK_status);
        if (worklockSuccess) {
            break;
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }
    if (!worklockSuccess) {
        return false;
    }

    // 设置CPSM
    bool cpsmSuccess = false;
    for (int i = 0; i < MAX_RETRIES; i++) {
        cpsmSuccess = cmd_CPSMset(inst,NULL);
        if (cpsmSuccess) {
            break;
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }
    if (!cpsmSuccess) {
        return false;
    }

    // 关闭回显
    bool echoCloseSuccess = false;
    for (int i = 0; i < MAX_RETRIES; i++) {
        echoCloseSuccess = cmd_EchoClose(inst,NULL);
        if (echoCloseSuccess) {
            break;
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }
    if (!echoCloseSuccess) {
        return false;
    }

    // 获取PIN码状态
    bool pincodeGetSuccess = false;
    for (int i = 0; i < MAX_RETRIES; i++) {
        pincodeGetSuccess = cmd_PincodeGet(inst,NULL);
        if (pincodeGetSuccess) {
            break;
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }
    if (!pincodeGetSuccess) {
        return false;
    }

    // 禁止主动上报网络注册消息
    uint16 CEREG_status = 0;
    bool ceregSuccess = false;
    for (int i = 0; i < MAX_RETRIES; i++) {
        ceregSuccess = cmd_CEREGset(inst,&CEREG_status);
        if (ceregSuccess) {
            break;
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }
    if (!ceregSuccess) {
        return false;
    }

    // 获取模块信息
    //cmd_ModemInfoGet(inst,NULL);

    return true;
}

static bool cmd_NetworkCheck(MctInstance* inst,void *para)
{
    cmd_ModemUpdateSignal(inst,para);
    cmd_CheckCEREG(inst,para);
    return true;
}

static bool cmd_mqttflow(MctInstance* inst,void *para)
{
    mqttConnet_t mqttConnetpara;
    memset(&mqttConnetpara, 0, sizeof(mqttConnet_t));
    mqttConnetpara.Serverip = MCT_PTR(server_url);
    MCT_GET(server_port,&mqttConnetpara.server_port,sizeof(mqttConnetpara.server_port));
    mqttConnetpara.mqtt_key = MCT_PTR(mqtt_key);
    mqttConnetpara.DeviceName = MCT_PTR(device_name);
    mqttConnetpara.DeviceSecret = MCT_PTR(mqtt_secret);
    MCT_GET(mqtt_connect_timeout,&mqttConnetpara.mqtt_connect_timeout,sizeof(mqttConnetpara.mqtt_connect_timeout));
    MCT_GET(mqtt_keep_alive,&mqttConnetpara.mqtt_keep_alive,sizeof(mqttConnetpara.mqtt_keep_alive));
    
    if (false == mct_y7025_execute(inst,  CMD_Y7025_MQTTNEW, &mqttConnetpara))
    {
        ULOG_INFO("Y7025: CMD_Y7025_MQTTNEW failed:");
        return false;
    }
    if (false == mct_y7025_execute(inst,  CMD_Y7025_MQTTCON, &mqttConnetpara))
    {
        ULOG_INFO("Y7025: CMD_Y7025_MQTTCON failed:");
        return false;
    }
    if (false == mct_y7025_execute(inst,  CMD_Y7025_MQTTSUB, MCT_PTR(mqtt_command_topic)))
    {
        ULOG_INFO("Y7025: CMD_Y7025_MQTTSUB g_mqtt_topic.response failed:");
        return false;
    }
    if (false == mct_y7025_execute(inst,  CMD_Y7025_MQTTSUB, MCT_PTR(mqtt_config_down_topic)))
    {
        ULOG_INFO("Y7025: CMD_Y7025_MQTTSUB g_mqtt_topic.config_down failed:");
        return false;
    }
    return true;
}
static bool cmd_mqttstop(MctInstance* inst,void *para)
{
    mct_y7025_execute(inst,  CMD_Y7025_MQTTCLOSE, NULL);
    return true;
}

static bool cmd_mqttpublish(MctInstance* inst,void *para)
{
    
    if (false == mct_y7025_execute(inst,  CMD_Y7025_MQTTPUB, para))
    {
        ULOG_INFO("Y7025: CMD_Y7025_MQTTPub failed:");
        return false;
    }
    return true;
}

static bool cmd_ClockGet(MctInstance* inst,void *para)
{
    tWanClock *clock = (tWanClock*)para;
/*
    if (true == mct_y7025_execute(inst,  CMD_Y7025_CMNTP, clock))
    {
        return true;
    }
    else
    {
        memset(clock, 0, sizeof(tWanClock));
        ULOG_INFO("Y7025: CMD_Y7025_CMNTP failed:");
    }
*/
    if (true == mct_y7025_execute(inst,  CMD_Y7025_CLOCK_GET, clock))
    {
        return true;
    }
    else
    {
        memset(clock, 0, sizeof(tWanClock));
        ULOG_INFO("Y7025: CMD_Y7025_CLOCK_GET failed:");
        return false;
    }
}

static bool cmd_revHandle(MctInstance *inst,void *para)
{

    mct_y7025_execute(inst,CMD_Y7025_MQTTREV,NULL);
    return true;
}
static bool cmd_httpConnect(MctInstance *inst,void *para)
{
    httpURL httpurL;
    memset(&httpurL, 0, sizeof(httpURL));
    httpurL.ip = MCT_PTR(update_url);
    MCT_GET(update_port,&httpurL.port,sizeof(httpurL.port));
    mct_y7025_execute(inst,CMD_Y7025_HTTPCREATE,&httpurL);
    return true;
}


static bool cmd_httpGet(MctInstance *inst,void *para)
{
    http_t *httppara = (http_t*)para;
    char range[64] = {0};
    uint32_t startoffset =  httppara->BlockNum *httppara->fileLen;
    uint32_t endoffset = startoffset + httppara->fileLen - 1;
    mct_y7025_execute(inst,CMD_Y7025_HTTPHEADSET,range);
    //Range:bytes=0-255\r\n
    //application/octet-stream
    //text/plain
    sprintf(range, "Content-Type:application/octet-stream\r\nRange:bytes=%d-%d\r\n", startoffset,endoffset);
    mct_y7025_execute(inst,CMD_Y7025_HTTPHEADSET,range);
    mct_y7025_execute(inst,CMD_Y7025_HTTPHEADGET,NULL);

    mct_y7025_execute(inst,CMD_Y7025_HTTPSEND,httppara);
    return true;
}
static bool cmd_httpClose(MctInstance *inst,void *para)
{
    http_t *httppara = (http_t*)para;
    mct_y7025_execute(inst,CMD_Y7025_HTTPCLOSE,httppara);
    return true;
}

static const tCmdApi funList[] =
{
    {.id = CMD_BOOTUPCLOCK, .fun = cmd_GetZTZEU},
    {.id = CMD_ECHO_CLOSE, .fun = cmd_EchoClose},
    {.id = CMD_WORKLOCKSET, .fun = cmd_WorklockSet},
    {.id = CMD_MODEM_INFO, .fun = cmd_ModemInfoGet},
    {.id = CMD_PINCODE_SET, .fun = cmd_PincodeSet},
    {.id = CMD_POWER_OFF, .fun = cmd_powerOff},
    {.id = CMD_AUTO_CONFIG, .fun = cmd_autoConfig},
    {.id = CMD_MODEM_SIGNAL, .fun = cmd_ModemUpdateSignal},
    {.id = CMD_NETWORK_CHECK, .fun = cmd_NetworkCheck},
    {.id = CMD_COLDSTARTUP_CHECK, .fun = NULL},
    {.id = CMD_MQTTFLOW, .fun = cmd_mqttflow},
    {.id = CMD_MQTTSTOP, .fun = cmd_mqttstop},
    {.id = CMD_MQTTPUBLISH, .fun = cmd_mqttpublish},
    {.id = CMD_MODEM_CLOCK_GET, .fun = cmd_ClockGet},
    {.id = CMD_REV_FLOW,   .fun = cmd_revHandle},
    {.id = CMD_HTTPCONNECT,   .fun = cmd_httpConnect},
    {.id = CMD_HTTPGET,   .fun = cmd_httpGet},
    {.id = CMD_HTTPCLOSE,   .fun = cmd_httpClose},
        
};

tCmdApi const *CMD_Y7025ApiGet(void)
{
    return funList;
}

/*********** End of file **/
