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
 *
 *
 * @note
 *
 *           %2    %4     %3     brief
 *******************************************************************************
 */
#include "command_Y7025Adapter.h"
#include "command_Y7025.h"
#include <stdlib.h>
#include "string.h"
#include "GlobalDataManager.h"
#define LOG_MODULE "CMD_Y7025"
static bool cmd_GetZTZEU(void *para)
{
    tWanClock *clock = (tWanClock*)para;
    if (TRUE == CMD_Y7025Execute(CMD_Y7025_BOOTZTZEU, clock))
    {
        return TRUE;
    }
    else
    {
        memset(clock, 0, sizeof(tWanClock));
        ULOG_INFO("Y7025: GetZTZEU failed:");
        return FALSE;
    }
}
static bool cmd_StringParaGet(uint16 cmdId)
{
    uint8 tep[50];
    tStringReq req;

    memset((char *)tep, 0x00, sizeof(tep));
    req.addr = tep;
    req.maxLen = sizeof(tep);
    if (FALSE == CMD_Y7025Execute(cmdId, (void *)&req))
    {
        return FALSE;
    }
    else
    {
        ULOG_INFO("Y7025:%s", tep);
        return TRUE;
    }
}

static bool cmd_WorklockSet(void *para)
{
    if (FALSE == CMD_Y7025Execute(CMD_Y7025_WORKLOCK, para))
    {
        ULOG_INFO("Y7025: CMD_Y7025_WORKLOCK failed:");
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

// 关闭回显
static bool cmd_EchoClose(void *para)
{
    if (FALSE == CMD_Y7025Execute(CMD_Y7025_ECHO_OFF, NULL))
    {
        ULOG_INFO("Y7025: Cmd CMD_Y7025_ECHO_OFF failed:");
    }
    return TRUE;
}
static bool cmd_ModemInfoGet(void *para)
{
    if (FALSE == cmd_StringParaGet(CMD_Y7025_IMEI_GET))
    {
        ULOG_INFO("Y7025: CMD_Y7025_IMEI_GET failed");
        return FALSE;
    }
    if (FALSE == cmd_StringParaGet(CMD_Y7025_VERSION_GET))
    {
        ULOG_INFO("Y7025: CMD_Y7025_VERSION_GET failed");
        return FALSE;
    }
    return TRUE;
}

static bool cmd_PincodeGet(void *para)
{
    ePincode status;
    if (FALSE == CMD_Y7025Execute(CMD_Y7025_PINCODE_INQUIRE, (void *)&status))
    {
        ULOG_INFO("Y7025: CMD_Y7025_PINCODE_INQUIRE failed:");
        return FALSE;
    }
    else
    {
        ULOG_INFO("Y7025: pincode:%d", status);
        return TRUE;
    }
}
static bool cmd_PincodeSet(void *para)
{
    ePincode status;
    tPincode code;

    if (FALSE == CMD_Y7025Execute(CMD_Y7025_PINCODE_INQUIRE, (void *)&status))
    {
        ULOG_INFO("Y7025: CMD_Y7025_PINCODE_INQUIRE failed:");
        return FALSE;
    }
    if (PIN_ERROR == status)
    {
        return FALSE;
    }
    if (PIN_INPUT == status)
    {
        // 设置值
        code.data = 1;
        return CMD_Y7025Execute(CMD_Y7025_PINCODE_SET, (void *)&code.data);
    }
    return TRUE;
}

// 主动上报网络注册消息
static bool cmd_CEREGset(void *para)
{
    if (FALSE == CMD_Y7025Execute(CMD_Y7025_CEREG, para))
    {
        ULOG_INFO("Y7025: CMD_Y7025_CEREG failed:");
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}
// 查看信号强度
static bool cmd_ModemUpdateSignal(void *para)
{
    tsignalQuality signal;

    memset((void *)&signal, 0x00, sizeof(tsignalQuality));
    if (FALSE == CMD_Y7025Execute(CMD_Y7025_SIGNAL_GET, (void *)&signal))
    {
        ULOG_INFO("Y7025: CMD_Y7025_SIGNAL_GET failed:");
        return FALSE;
    }
    else
    {
        WAN_ParaSet(WAN_PARA_SIGNAL, (void *)&signal.rssi, sizeof(int8));
    }

    return TRUE;
}
// 查看注网状态
static bool cmd_CheckCEREG(void *para)
{
    tRegStus reg;
    if (FALSE == CMD_Y7025Execute(CMD_Y7025_DATA_REGISTER, (void *)&reg.value))
    {
        ULOG_INFO("Y7025: CMD_Y7025_DATA_REGISTER failed:");
        return FALSE;
    }
    else
    {
        WAN_ParaSet(WAN_PARA_REGSTATUS, (void *)&reg.value, sizeof(eRegStatus));
    }
    return TRUE;
}
// 配置PSM模式下，终端设备会减少与网络的交互
// 设置PSM参数，此处将T3412设置为60小时
// 意思是只要在60小时内至少有一次上报，就不用重新注网。T3324设置为2秒，意思是解除DTR休眠锁后延迟2秒进入PSM休眠
static bool cmd_CPSMset(void *para)
{

    if (FALSE == CMD_Y7025Execute(CMD_Y7025_CPSMS, NULL))
    {
        ULOG_INFO("Y7025: CMD_Y7025_CPSMS failed:");
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

static bool cmd_powerOff(void *para)
{
    if (FALSE == CMD_Y7025Execute(CMD_Y7025_POWER_OFF, NULL))
    {
        ULOG_INFO("Y7025: CMD_Y7025_POWER_OFF failed:");
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

static bool cmd_autoConfig(void *para)
{
    uint8_t MAX_RETRIES = 3;

    // 设置worklock
    uint16_t WORKLOCK_status = 1;
    bool worklockSuccess = false;
    for (int i = 0; i < MAX_RETRIES; i++) {
        worklockSuccess = cmd_WorklockSet(&WORKLOCK_status);
        if (worklockSuccess) {
            break;
        }
        CMD_Delay(WAIT_SCHEDULE_TIME);
    }
    if (!worklockSuccess) {
        return FALSE;
    }

    // 设置CPSM
    bool cpsmSuccess = false;
    for (int i = 0; i < MAX_RETRIES; i++) {
        cpsmSuccess = cmd_CPSMset(NULL);
        if (cpsmSuccess) {
            break;
        }
        CMD_Delay(WAIT_SCHEDULE_TIME);
    }
    if (!cpsmSuccess) {
        return FALSE;
    }

    // 关闭回显
    bool echoCloseSuccess = false;
    for (int i = 0; i < MAX_RETRIES; i++) {
        echoCloseSuccess = cmd_EchoClose(NULL);
        if (echoCloseSuccess) {
            break;
        }
        CMD_Delay(WAIT_SCHEDULE_TIME);
    }
    if (!echoCloseSuccess) {
        return FALSE;
    }

    // 获取PIN码状态
    bool pincodeGetSuccess = false;
    for (int i = 0; i < MAX_RETRIES; i++) {
        pincodeGetSuccess = cmd_PincodeGet(NULL);
        if (pincodeGetSuccess) {
            break;
        }
        CMD_Delay(WAIT_SCHEDULE_TIME);
    }
    if (!pincodeGetSuccess) {
        return FALSE;
    }

    // 禁止主动上报网络注册消息
    uint16 CEREG_status = 0;
    bool ceregSuccess = false;
    for (int i = 0; i < MAX_RETRIES; i++) {
        ceregSuccess = cmd_CEREGset(&CEREG_status);
        if (ceregSuccess) {
            break;
        }
        CMD_Delay(WAIT_SCHEDULE_TIME);
    }
    if (!ceregSuccess) {
        return FALSE;
    }

    return TRUE;
}

static bool cmd_NetworkCheck(void *para)
{
    cmd_ModemUpdateSignal(NULL);
    CMD_Delay(500);
    cmd_CheckCEREG(NULL);
    return TRUE;
}

//采集5s log
static bool cmd_coldstartcheck(void *para)
{
    uint32 Len = 0;
    uint32 cnt = 0;
    //const char *RecvBuf = (char *)pHandle_Rec;
    do
    {
        uint32_t single_recv_len = 0;
        single_recv_len = Modem_Read(Len);
        Len += single_recv_len;

        if (Len > 0)
        {
            //只要有输出就认为已启动
            //if (strstr(RecvBuf, "^SIMST:") != NULL)
            {
                return TRUE;
            }
        }

        CMD_Delay(WAIT_SCHEDULE_TIME);
        cnt++;
    } while (cnt < (5000 / WAIT_SCHEDULE_TIME));


    return FALSE;
        
}
static bool cmd_mqttflow(void *para)
{
    mqttConnet_t mqttConnetpara;
    mqttConnetpara.Serverip = g_RTCfg.ServerURL;
    mqttConnetpara.ServerPort = g_RTCfg.ServerPort;
    mqttConnetpara.ConnectTimeOut = g_RTCfg.ConnectTimeOut;
    if (FALSE == CMD_Y7025Execute(CMD_Y7025_MQTTNEW, &mqttConnetpara))
    {
        ULOG_INFO("Y7025: CMD_Y7025_MQTTNEW failed:");
        return FALSE;
    }
    mqttconnnack_t mqttconnnackpara;
    if (FALSE == CMD_Y7025Execute(CMD_Y7025_MQTTCON, &mqttconnnackpara))
    {
        ULOG_INFO("Y7025: CMD_Y7025_MQTTCON failed:");
        return FALSE;
    }
    else
    {
        if (0 != mqttconnnackpara.return_code || 0 != mqttconnnackpara.socket_id)
        {
            ULOG_INFO("Y7025: CMD_Y7025_MQTTCON para failed: %d, %d", mqttconnnackpara.socket_id, mqttconnnackpara.return_code);
            return FALSE;
        }
    }

    if (FALSE == CMD_Y7025Execute(CMD_Y7025_MQTTSUB, g_MQTopic.Resp))
    {
        ULOG_INFO("Y7025: CMD_Y7025_MQTTSUB g_MQTopic.Resp failed:");
        return FALSE;
    }
    if (FALSE == CMD_Y7025Execute(CMD_Y7025_MQTTSUB, g_MQTopic.ConfigDn))
    {
        ULOG_INFO("Y7025: CMD_Y7025_MQTTSUB g_MQTopic.ConfigDn failed:");
        return FALSE;
    }
    return TRUE;
}
static bool cmd_mqttpublish(void *para)
{
    
    if (FALSE == CMD_Y7025Execute(CMD_Y7025_MQTTPUB, para))
    {
        ULOG_INFO("Y7025: CMD_Y7025_MQTTPub failed:");
        return FALSE;
    }
    return TRUE;
}
static bool cmd_mqttrev(void *para)
{
    return TRUE;
}

static bool cmd_ClockGet(void *para)
{
    tWanClock *clock = (tWanClock*)para;
    if (TRUE == CMD_Y7025Execute(CMD_Y7025_CLOCK_GET, clock))
    {
        return TRUE;
    }
    else
    {
        memset(clock, 0, sizeof(tWanClock));
        ULOG_INFO("Y7025: CMD_Y7025_CLOCK_GET failed:");
        return FALSE;
    }
}

static bool cmd_mqttRevPush(void *para)
{
    ptWanData WanData = (ptWanData)para;
    CMD_Y7025Execute(CMD_Y7025_MQTTREV, para);
    //接受有效
    if(WanData->RevCmdValidFlag)
    {
        MqttPulish_t mattPub = {0};
        //执行命令
        mattPub.PublishData = UintyWanInterface(WanData->RevCmd);
        mattPub.PublishLen = strlen(mattPub.PublishData);
        switch(WanData->mqttType)
        {
            case topic_ConfigDn:
            mattPub.Topic = g_MQTopic.ConfigUp;
            break;
            case topic_Recv:
            mattPub.Topic = g_MQTopic.Resp;
            break;
            default:
            mattPub.Topic = g_MQTopic.Resp;
            break;
        }
        mattPub.QOS = 1;
        cmd_mqttpublish(&mattPub);
        vPortFree(mattPub.PublishData);//释放堆空间
    }
    return TRUE;
}

static bool cmd_stickframHandle(void *para)
{
    ptWanControl Control = (ptWanControl)para;
    switch (Control->sitckFramAction.StickFrameMirco)
    {
    case CMD_Y7025_MQTTREV:
        {
            tWanData mqttData;
            memset(&mqttData,0,sizeof(mqttData));
            cmd_mqttRevPush(&mqttData);
        }
        break;
    
    default:
        break;
    }



    return TRUE;
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
        {.id = CMD_COLDSTARTUP_CHECK, .fun = cmd_coldstartcheck},
        {.id = CMD_MQTTFLOW, .fun = cmd_mqttflow},
        {.id = CMD_MQTTPUBLISH, .fun = cmd_mqttpublish},
        {.id = CMD_MQTTREV, .fun = cmd_mqttrev},
        {.id = CMD_MODEM_CLOCK_GET, .fun = cmd_ClockGet},
        {.id = CMD_MQTTREVPUSH, .fun = cmd_mqttRevPush},
        {.id = CMD_STICKFRAME, .fun = cmd_stickframHandle},
        
};

tCmdApi const *CMD_Y7025ApiGet(void)
{
    return funList;
}

/*********** End of file **/
