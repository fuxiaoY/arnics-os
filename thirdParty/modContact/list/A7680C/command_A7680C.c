/**
 *******************************************************************************
 * @file     $command_A7680C.c$
 * @author
 * @version  ver 1
 * @date     July 18th 2019
 * @build
 * @brief    command_A7680C Operation
 * @attention
 *
 * cmd_Pack: cb->cmd：打包好的指令  *para 用于打包的参数
 * cmd_Analy:cb->rec：模块回复的原始报文  *para解析后的出参
 * @note
 *
 *           %2    %4     %3     brief
 *******************************************************************************
 */
#include "command_A7680C.h"

// AT+CGDCONT?
static bool cmd_PackPdpRead(uint8_t* buf, size_t* len, void *para)
{
    *len = sprintf((char*)buf, "AT+CGDCONT?\r\n");
    return true;
}

// +CGDCONT: 1,"IP","cmnet","10.7.70.160,"",0,0,,,,C11
static bool cmd_AnalyzePdpRead(uint8_t* buf, size_t len, void *para)
{
    PDP_Context *pdp_context = (PDP_Context *)para;
    const char *prefix = "+CGDCONT:";
    size_t prefixLen = strlen(prefix);
    uint8_t *ptr = (uint8_t *)buf;
    ptr = hexhex((uint8_t *)buf, (uint8_t *)prefix, len, prefixLen);
    if (ptr == NULL)
    {
        return false;
    }
    char *char_ptr = (char *)ptr;
    char_ptr += prefixLen;
    uint16 i = 0, index = 0, loop_count = 0;
    uint16 resp_len = len - prefixLen;
    char *strTopic = char_ptr;
    while (index < resp_len)
    {
        do
        {
            strTopic++;
            i++;
            index++;
            if (index == resp_len)
                break;
        } while (*strTopic != ',');
        i -= 1;
        switch (loop_count)
        {
        case 0:
            sscanf(char_ptr, "%02u", (unsigned int *)&(pdp_context->cid));
            break;
        case 1:
            strncpy(pdp_context->PDP_type, char_ptr, i);
            break;
        case 2:
            strncpy(pdp_context->APN, char_ptr, i);
            break;
        case 3:
            strncpy(pdp_context->PDP_addr, char_ptr, i);
            break;
        case 4:
            strncpy(pdp_context->d_comp, char_ptr, i);
            break;
        case 5:
            strncpy(pdp_context->h_comp, char_ptr, i);
            break;
        case 6:
            strncpy(pdp_context->ipv4_ctrl, char_ptr, i);
            break;
        case 7:
            strncpy(pdp_context->request_type, char_ptr, i);
            break;
        case 8:
            strncpy(pdp_context->PCSCF_discovery, char_ptr, i);
            break;
        case 9:
            strncpy(pdp_context->IM_CN_Signalling_Flag_Ind, char_ptr, i);
            break;
        case 10:
            strncpy(pdp_context->CR, char_ptr, i);
            break;
        case 11:
            strncpy(pdp_context->LF, char_ptr, i);
            break;
        }
        char_ptr = strTopic + 1;
        i = 0;
        loop_count++;
    }
    return true;
}

// AT+CGDCONT=1,"IP","cnnet"
static bool cmd_PackPdpConfig(uint8_t* buf, size_t *len, void *para)
{
    PDP_Context_set *pdp_context = (PDP_Context_set *)para;
    *len = sprintf((char*)buf, "AT+CGDCONT=%u,%s,%s\r\n", pdp_context->cid, pdp_context->PDP_type, pdp_context->APN);
    return true;
}

static bool cmd_AnalyzePdpConfig(uint8_t* buf, size_t len, void *para)
{
    return true;
}

// AT+CGAUTH=1,0
static bool cmd_PackPdpIpSet(uint8_t* buf, size_t *len, void *para)
{
    PDPIP_Auth_Type *pdp_auth_type = (PDPIP_Auth_Type *)para;
    *len = sprintf((char*)buf, "AT+CGAUTH=%u,%u\r\n", pdp_auth_type->cid, pdp_auth_type->auth_type);
    return true;
}

static bool cmd_AnalyzePdpIpSet(uint8_t* buf, size_t len, void *para)
{
    return true;
}

// ATE0
static bool cmd_PackEchoSwitch(uint8_t* buf, size_t *len, void *para)
{
    EchoSwitch *echosw = (EchoSwitch *)para;
    *len = sprintf((char*)buf, "ATE%u\r\n", *echosw);
    return true;
}

static bool cmd_AnalyzeEchoSwitch(uint8_t* buf, size_t len, void *para)
{
    return true;
}

// AT+AUTOCSQ=0,0
static bool cmd_PackCsqReport(uint8_t* buf, size_t *len, void *para)
{
    CsqReport *Topic = (CsqReport *)para;
    *len = sprintf((char*)buf, "AT+AUTOCSQ=%u,%u\r\n", Topic->auto_sw, Topic->mode);
    return true;
}

static bool cmd_AnalyzeCsqReport(uint8_t* buf, size_t len, void *para)
{
    return true;
}

// AT+CPIN?
static bool cmd_PackCpinRead(uint8_t* buf, size_t *len, void *para)
{
    *len = sprintf((char*)buf, "AT+CPIN?\r\n");
    return true;
}

/* +CPIN: READY */
static bool cmd_AnalyzeCpinRead(uint8_t* buf, size_t len, void *para)
{
    char *code_buffer = (char *)para;
    const char *prefix = "+CPIN:";
    uint8_t *ptr = (uint8_t *)buf;
    ptr = hexhex((uint8_t *)buf, (uint8_t *)prefix, len, strlen(prefix));
    if (ptr == NULL)
    {
        return false;
    }
    char *char_ptr = (char *)ptr;
    char_ptr += strlen(prefix);
    memcpy(code_buffer, char_ptr, len - strlen(prefix));
    return true;
}

// AT+CTZU=1
static bool cmd_PackNitzSwitch(uint8_t* buf, size_t *len, void *para)
{
    NitzSwitch *Topic = (NitzSwitch *)para;
    *len = sprintf((char*)buf, "AT+CTZU=%u\r\n", *Topic);
    return true;
}

static bool cmd_AnalyzeNitzSwitch(uint8_t* buf, size_t len, void *para)
{
    return true;
}

// AT+CIMI
static bool cmd_PackImsiRead(uint8_t* buf, size_t *len, void *para)
{
    *len = sprintf((char*)buf, "AT+CIMI\r\n");
    return true;
}

// 460111114439969
static bool cmd_AnalyzeImsiRead(uint8_t* buf, size_t len, void *para)
{
    // no match
    char *IMSI_code = (char *)para;
    memcpy(IMSI_code, buf, len);
    return true;
}

// AT+CGSN
static bool cmd_PackSnRead(uint8_t* buf, size_t *len, void *para)
{
    *len = sprintf((char*)buf, "AT+CGSN\r\n");
    return true;
}

// 869665065350134
static bool cmd_AnalyzeSnRead(uint8_t* buf, size_t len, void *para)
{
    // no match
    char *SN_code = (char *)para;
    memcpy(SN_code, buf, len);
    return true;
}

// AT+CICCID
static bool cmd_PackIccidRead(uint8_t* buf, size_t *len, void *para)
{
    *len = sprintf((char*)buf, "AT+CICCID\r\n");
    return true;
}

//+ICCID: 89860322492005332847
static bool cmd_AnalyzeIccidRead(uint8_t* buf, size_t len, void *para)
{
    char *ICCID_code = (char *)para;
    const char *prefix = "+ICCID:";
    uint8_t *ptr = (uint8_t *)buf;
    ptr = hexhex((uint8_t *)buf, (uint8_t *)prefix, len, strlen(prefix));
    if (ptr == NULL)
    {
        return false;
    }
    char *char_ptr = (char *)ptr;
    char_ptr += strlen(prefix);
    memcpy(ICCID_code, char_ptr, len - strlen(prefix));
    return true;
}

// AT+CGMM
static bool cmd_PackModIdRead(uint8_t* buf, size_t *len, void *para)
{
    *len = sprintf((char*)buf, "AT+CGMM\r\n");
    return true;
}

// A7680C-LANV
static bool cmd_AnalyzeModIdRead(uint8_t* buf, size_t len, void *para)
{
    // no match
    char *modle_id_text = (char *)para;
    memcpy(modle_id_text, buf, len);
    return true;
}

// AT+CGMR
static bool cmd_PackFwVerRead(uint8_t* buf, size_t *len, void *para)
{
    *len = sprintf((char*)buf, "AT+CGMR\r\n");
    return true;
}

//+CGMR: 2269B03A7680M5A
static bool cmd_AnalyzeFwVerRead(uint8_t* buf, size_t len, void *para)
{
    char *FW_version_text = (char *)para;
    const char *prefix = "+CGMR:";
    uint8_t *ptr = (uint8_t *)buf;
    ptr = hexhex((uint8_t *)buf, (uint8_t *)prefix, len, strlen(prefix));
    if (ptr == NULL)
    {
        return false;
    }
    char *char_ptr = (char *)ptr;
    char_ptr += strlen(prefix);
    memcpy(FW_version_text, char_ptr, len - strlen(prefix));
    return true;
}

// AT+CSQ
static bool cmd_PackMeInfoRead(uint8_t* buf, size_t *len, void *para)
{
    *len = sprintf((char*)buf, "AT+CSQ\r\n");
    return true;
}

//+CSQ: 25,99
static bool cmd_AnalyzeMeInfoRead(uint8_t* buf, size_t len, void *para)
{
    
    ME_Info *me_info = (ME_Info *)para;
    const char *prefix = "+CSQ:";
    size_t prefixLen = strlen(prefix);
    uint8_t *ptr = (uint8_t *)buf;
    ptr = hexhex((uint8_t *)buf, (uint8_t *)prefix, len, prefixLen);
    if (ptr == NULL)
    {
        return false;
    }
    char *char_ptr = (char *)ptr;
    char_ptr += prefixLen;

    sscanf(char_ptr, "%2hhu", &(me_info->rssi));
    uint16 i = 0;
    char *strTopic = char_ptr;
    while (*strTopic != ',')
    {
        strTopic++;
        i++;
        if (i > 10)
        {
            me_info->RevCmdValidFlag = 0;
            return false;
        }
    }
    char_ptr = strTopic + 1;
    sscanf(char_ptr, "%2hhu", &(me_info->ber));
    return true;
}

// AT+CGREG?
static bool cmd_PackStatusRead(uint8_t* buf, size_t *len, void *para)
{
    *len = sprintf((char*)buf, "AT+CGREG?\r\n");
    return true;
}

//+CGREG: 0,1
static bool cmd_AnalyzeStatusRead(uint8_t* buf, size_t len, void *para)
{
    Network_Info *net_info = (Network_Info *)para;
    const char *prefix = "+CGREG:";
    size_t prefixLen = strlen(prefix);
    uint8_t *ptr = (uint8_t *)buf;
    ptr = hexhex((uint8_t *)buf, (uint8_t *)prefix, len, prefixLen);
    if (ptr == NULL)
    {
        return false;
    }
    char *char_ptr = (char *)ptr;
    char_ptr += prefixLen;
    uint16 i = 0, index = 0, loop_count = 0;
    uint16 resp_len = len - prefixLen;
    char *strTopic = char_ptr;
    while (index < resp_len)
    {
        do
        {
            strTopic++;
            i++;
            index++;
            if (index == resp_len)
                break;
        } while (*strTopic != ',');
        i -= 1;
        switch (loop_count)
        {
        case 0:
            sscanf(char_ptr, "%02u", &(net_info->n));
            break;
        case 1:
            sscanf(char_ptr, "%02u", &(net_info->stat));
            break;
        case 2:
            sscanf(char_ptr, "%02u", &(net_info->lac));
            break;
        case 3:
            memcpy(net_info->ci, char_ptr, i);
            break;
        }
        char_ptr = strTopic + 1;
        i = 0;
        loop_count++;
    }
    return true;
}

/*
 *   MQTT PACK
 *   Client_ID : 0->MQTT_CLIENT_INDEX
 */


// AT+CMQTTSTART
static bool cmd_PackMqttStart(uint8_t* buf, size_t *len, void *para)
{
    *len = sprintf((char*)buf, "AT+CMQTTSTART\r\n");
    return true;
}

//+CMQTTSTART: 0
static bool cmd_AnalyzeMqttStart(uint8_t* buf, size_t len, void *para)
{
    uint32_t errcode = 0;
    const char *prefix = "+CMQTTSTART:";
    uint8_t *ptr = (uint8_t *)buf;
    ptr = hexhex((uint8_t *)buf, (uint8_t *)prefix, len, strlen(prefix));
    if (ptr == NULL)
    {
        return false;
    }
    char *char_ptr = (char *)ptr;
    char_ptr += strlen(prefix);
    sscanf(char_ptr, "%02u", &errcode);
    if (errcode != 0)
    {
        printf("MQTT Start Failed, ErrorCode = %02u\r\n", errcode);
        return false;
    }
    else
    {
        printf("MQTT Start Successfully!\r\n");
        return true;
    }
}

// AT+CMQTTACCQ
static bool cmd_PackMqttAccq(uint8_t* buf, size_t *len, void *para)
{
    MqttAccq_Info *Topic = (MqttAccq_Info *)para;
    *len = sprintf((char*)buf, "AT+CMQTTACCQ=%u,\"%s\",%u\r\n", Topic->client_index, Topic->clientID, Topic->server_type);
    return true;
}
// OK
static bool cmd_AnalyzeMqttAccq(uint8_t* buf, size_t len, void *para)
{
    return true;
}

// AT+CMQTTCONNECT
// [ATCmd: AT+CMQTTCONNECT=0,"tcp://arong.xicp.net:15321",120,1,"123&DefaultProductKey","EF78D2329E1E387DCB5CA110E71E3839A134C3CF"]
static bool cmd_PackMqttConnect(uint8_t* buf, size_t *len, void *para)
{
    MqttConnect_Info *connct_info = (MqttConnect_Info *)para;
    *len = sprintf((char*)buf, "AT+CMQTTCONNECT=%u,\"%s\",%u,%u,\"%s\",\"%s\"\r\n",
                         connct_info->client_index, connct_info->server_addr, connct_info->keepalive_time, connct_info->clean_session, connct_info->user_name, connct_info->pass_word);
    return true;
}
// +CMQTTCONNECT: 0,0
// +CMQTTCONNECT: <client_index>,<err>
static bool cmd_AnalyzeMqttConnect(uint8_t* buf, size_t len, void *para)
{
    MqttRecv_Info recv_info;
    const char *prefix = "+CMQTTCONNECT:";
    size_t prefixLen = strlen(prefix);
    uint8_t *ptr = (uint8_t *)buf;
    ptr = hexhex((uint8_t *)buf, (uint8_t *)prefix, len, prefixLen);
    if (ptr == NULL)
    {
        return false;
    }
    char *char_ptr = (char *)ptr;
    char_ptr += prefixLen;

    sscanf(char_ptr, "%02u", &(recv_info.client_index));
    uint16 i = 0;
    char *strTopic = char_ptr;
    while (*strTopic != ',')
    {
        strTopic++;
        i++;
        if (i > 100)
        {
            recv_info.RevCmdValidFlag = 0;
            return false;
        }
    }
    char_ptr = strTopic + 1;
    sscanf(char_ptr, "%02u", &(recv_info.errorcode));
    if (recv_info.client_index != MQTT_CLIENT_INDEX || recv_info.errorcode != 0)
    {
        printf("Mqtt Connect Failed, index = %u err = %u\r\n", recv_info.client_index, recv_info.errorcode);
        return false;
    }
    else
    {
        printf("Mqtt Connect Successfully!\r\n");
        return true;
    }
}

/*
    cmd_cb_test.cmdLen = sprintf(cmd_cb_test.cmd, "AT+CMQTTSUB=0,37,1\r\n");
    cmd_BufRecv(&cmd_cb_test);

    CMD_Delay(200);
    cmd_BufClear(&cmd_cb_test);
    cmd_cb_test.cmdLen = sprintf(cmd_cb_test.cmd, "$PERSISTENCE/command/8212345678200235\r\n");

*/
// AT+CMQTTSUB=0,37,1
static bool cmd_PackMqttSub(uint8_t* buf, size_t *len, void *para)
{
    MqttSub_Info *Topic = (MqttSub_Info *)para;
    *len = sprintf((char*)buf, "AT+CMQTTSUB=%u,%u,%u\r\n", Topic->client_index, Topic->reqLength, Topic->qos);
    return true;
}

static bool cmd_AnalyzeMqttSub(uint8_t* buf, size_t len, void *para)
{
    char *str = (char*)buf;
    if (!strncmp(str, "\r\n>", sizeof("\r\n>")))
    {
        printf("input data here:\r\n");
        return true;
    }
    else
    {
        printf("Mqtt subscribe Failed\r\n");
        return false;
    }
}
// sub send topic
static bool cmd_PackMqttSubSend(uint8_t* buf, size_t *len, void *para)
{
    char *topic_info = (char *)para;
    *len = sprintf((char*)buf, "%s\r\n", topic_info);
    return true;
}

static bool cmd_AnalyzeMqttSubSend(uint8_t* buf, size_t len, void *para)
{
    MqttRecv_Info recv_info;
    const char *prefix = "+CMQTTSUB:";
    size_t prefixLen = strlen(prefix);
    uint8_t *ptr = (uint8_t *)buf;
    ptr = hexhex((uint8_t *)buf, (uint8_t *)prefix, len, prefixLen);
    if (ptr == NULL)
    {
        return false;
    }
    char *char_ptr = (char *)ptr;
    char_ptr += prefixLen;

    sscanf(char_ptr, "%02u", &(recv_info.client_index));
    uint16 i = 0;
    char *strTopic = char_ptr;
    while (*strTopic != ',')
    {
        strTopic++;
        i++;
        if (i > 100)
        {
            recv_info.RevCmdValidFlag = 0;
            return false;
        }
    }
    char_ptr = strTopic + 1;
    sscanf(char_ptr, "%02u", &(recv_info.errorcode));
    if (recv_info.client_index != MQTT_CLIENT_INDEX || recv_info.errorcode != 0)
    {
        printf("Mqtt subscribe Failed, index = %u err = %u\r\n", recv_info.client_index, recv_info.errorcode);
        return false;
    }
    else
    {
        printf("Mqtt subscribe Successfully!\r\n");
        return true;
    }
}

// AT+CMQTTTOPIC
// AT+CMQTTTOPIC=<client_index>,<req_length>
static bool cmd_PackMqttTopic(uint8_t* buf, size_t *len, void *para)
{
    Topic_Info *Topic = (Topic_Info *)para;
    *len = sprintf((char*)buf, "AT+CMQTTTOPIC=%u,%u\r\n", Topic->client_index, Topic->req_length);
    return true;
}
static bool cmd_AnalyzeMqttTopic(uint8_t* buf, size_t len, void *para)
{
    char *str = (char*)buf;
    if (!strncmp(str, "\r\n>", sizeof("\r\n>")))
    {
        printf("input data here:\r\n");
        return true;
    }
    else
    {
        printf("Mqtt topic Error\r\n");
        return false;
    }
}

static bool cmd_PackMqttTopicSend(uint8_t* buf, size_t *len, void *para)
{
    char *topic_info = (char *)para;
    *len = sprintf((char*)buf, "%s\r\n", topic_info);
    return true;
}

static bool cmd_AnalyzeMqttTopicSend(uint8_t* buf, size_t len, void *para)
{
    return true;
}

// AT+CMQTTPAYLOAD=0,489
static bool cmd_PackMqttPayload(uint8_t* buf, size_t *len, void *para)
{
    Paylaod_Info *Topic = (Paylaod_Info *)para;
    *len = sprintf((char*)buf, "AT+CMQTTPAYLOAD=%u,%u\r\n", Topic->client_index, Topic->req_length);
    return true;
}
static bool cmd_AnalyzeMqttPayload(uint8_t* buf, size_t len, void *para)
{
    char *str = (char*)buf;
    if (!strncmp(str, "\r\n>", sizeof("\r\n>")))
    {
        printf("input data here:\r\n");
        return true;
    }
    else
    {
        printf("Mqtt payload Error\r\n");
        return false;
    }
}
static bool cmd_PackMqttPayloadSend(uint8_t* buf, size_t *len, void *para)
{
    char *topic_info = (char *)para;
    *len = sprintf((char*)buf, "%s\r\n", topic_info);
    return true;
}
static bool cmd_AnalyzeMqttPayloadSend(uint8_t* buf, size_t len, void *para)
{
    return true;
}

//AT+CMQTTDISC=0,120
static bool cmd_PackMqttDisc(uint8_t* buf, size_t *len, void *para)
{

    *len = sprintf((char*)buf, "AT+CMQTTDISC=%d,120\r\n", MQTT_CLIENT_INDEX);
    return true;
}
static bool cmd_AnalyzeMqttDisc(uint8_t* buf, size_t len, void *para)
{
    return true;
}

//AT+CMQTTREL=0
static bool cmd_PackMqttRel(uint8_t* buf, size_t *len, void *para)
{

    *len = sprintf((char*)buf, "AT+CMQTTREL=%d\r\n", MQTT_CLIENT_INDEX);
    return true;
}
static bool cmd_AnalyzeMqttRel(uint8_t* buf, size_t len, void *para)
{
    return true;
}

//AT+CMQTTSTOP
static bool cmd_PackMqttStop(uint8_t* buf, size_t *len, void *para)
{

    *len = sprintf((char*)buf, "AT+CMQTTSTOP\r\n");
    return true;
}
static bool cmd_AnalyzeMqttStop(uint8_t* buf, size_t len, void *para)
{
    return true;
}


// AT+CMQTTPUB=0,0,60
// AT+CMQTTPUB=<client_index>,<qos>,<pub_timeout>[,<ratained>[,<dup>]]
static bool cmd_PackMqttPublish(uint8_t* buf, size_t *len, void *para)
{
    Publish_Info *pub_info = (Publish_Info *)para;
    *len = sprintf((char*)buf, "AT+CMQTTPUB=%u,%u,%u\r\n", pub_info->client_index, pub_info->qos, pub_info->pub_timeout);
    return true;
}

static bool cmd_AnalyzeMqttPublish(uint8_t* buf, size_t len, void *para)
{
    MqttRecv_Info recv_info;
    const char *prefix = "+CMQTTPUB:";
    size_t prefixLen = strlen(prefix);
    uint8_t *ptr = (uint8_t *)buf;
    ptr = hexhex((uint8_t *)buf, (uint8_t *)prefix, len, prefixLen);
    if (ptr == NULL)
    {
        printf("Mqtt ptr null!\r\n");
        return false;
    }
    char *char_ptr = (char *)ptr;
    char_ptr += prefixLen;

    sscanf(char_ptr, "%02u", &(recv_info.client_index));
    uint16_t i = 0;
    char *strTopic = char_ptr;
    while (*strTopic != ',')
    {
        strTopic++;
        i++;
        if (i > 100)
        {
            recv_info.RevCmdValidFlag = 0;
            printf("Mqtt Publish max try failed!\r\n");
            return false;
        }
    }
    char_ptr = strTopic + 1;
    sscanf(char_ptr, "%02u", &(recv_info.errorcode));
    if (recv_info.client_index != MQTT_CLIENT_INDEX || recv_info.errorcode != 0)
    {
        printf("Mqtt Publish Failed, index = %u err = %u\r\n", recv_info.client_index, recv_info.errorcode);
        return false;
    }
    else
    {
        printf("Mqtt Publish Successfully!\r\n");
        return true;
    }
}

static bool cmd_PackCockRead(uint8_t* buf, size_t *len, void *para)
{
    *len = sprintf((char*)buf, "AT+CCLK?\r\n");
    return true;
}

//"yy/MM/dd,hh:mm:ss±zz"
//+CCLK: "14/01/01,02:14:36+08"
static bool cmd_AnalyzeCockRead(uint8_t* buf, size_t len, void *para)
{
    tWanClock *clock = (tWanClock *)para;
    const char *prefix = "+CCLK: \"";
    size_t prefixLen = strlen(prefix);
    uint8_t *ptr = (uint8_t *)buf;
    ptr = hexhex((uint8_t *)buf, (uint8_t *)prefix, len, prefixLen);
    if (ptr == NULL)
    {
        return false;
    }
    char *char_ptr = (char *)ptr;
    char_ptr += prefixLen;
    int year, month, day, hour, minute, second;

    sscanf(char_ptr, "%d", &year);
    char_ptr += 3;
    sscanf(char_ptr, "%d", &month);
    char_ptr += 3;
    sscanf(char_ptr, "%d", &day);
    char_ptr += 3;
    sscanf(char_ptr, "%d", &hour);
    char_ptr += 3;
    sscanf(char_ptr, "%d", &minute);
    char_ptr += 3;
    sscanf(char_ptr, "%d", &second);
    char_ptr += 3;
    sscanf(char_ptr, "%d", &clock->zone);

    // 检查各个字段是否在合理范围内
    if (year < 0 || year > 99)
        return false;
    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > 31)
        return false;
    if (hour < 0 || hour > 23)
        return false;
    if (minute < 0 || minute > 59)
        return false;
    if (second < 0 || second > 59)
        return false;

    // 设置时间数据
    clock->clock.year_h = 20;           // 年份高位
    clock->clock.year_l = (year % 100); // 年份低位
    clock->clock.month = (uint8_t)month;
    clock->clock.day = (uint8_t)day;
    clock->clock.week = 0xFF;          // 星期暂不处理
    clock->clock.hour = (uint8_t)hour; //+8时区
    clock->clock.min = (uint8_t)minute;
    clock->clock.sec = (uint8_t)second;
    clock->clock.hund = 0xFF; // 百分之一秒暂不处理
    clock->clock.dev_h = 0x80;
    clock->clock.dev_l = 0x00;
    clock->clock.status = 0x00;

    return true;
}

static bool cmd_PackColdStart(uint8_t* buf, size_t *len, void *para)
{
    *len = 0;
    return true;
}

//"yy/MM/dd,hh:mm:ss±zz"
//+CCLK: "14/01/01,02:14:36+08"
static bool cmd_AnalyzeColdStart(uint8_t* buf, size_t len, void *para)
{
    return true;
}


//AT+CPOF=? 关机
static bool cmd_PackPowerOff(uint8_t* buf, size_t *len, void *para)
{
    *len = sprintf((char*)buf, "AT+CPOF\r\n");
    return true;
}

static bool cmd_AnalyzePowerOff(uint8_t* buf, size_t len, void *para)
{
    return true;
}

/*
receive: len:143

+CMQTTRXSTART: 0,38,15
+CMQTTRXTOPIC: 0,38
$PERSISTENCE/configDn/8212345678100001
+CMQTTRXPAYLOAD: 0,15
{"boardType":0}
+CMQTTRXEND: 0
 

*/

static bool cmd_PackmqttRev(uint8_t* buf, size_t *len, void *para)
{
    *len = 0;
    return true;
}
static bool cmd_AnalyzemqttRev(uint8_t* buf, size_t len, void *para)
{
    printf("cmd_AnalyzemqttRev:\r\n");
    printf("%.*s \r\n", (len), buf);
    return true;
}

static const tCmd cmdList[] =
{
    CMD_ADD(CMD_A7680C_PDP_READ              ,   3,  "+CGDCONT:",       "\r\n",   "ERROR",    SendRev,    PdpRead    ),
    CMD_ADD(CMD_A7680C_PDP_CONFIG            ,   3,  "OK",              "\r\n",   "ERROR",    SendRev,    PdpConfig  ), 
    CMD_ADD(CMD_A7680C_PDP_IPSET             ,   3,  "OK",              "\r\n",   "ERROR",    SendRev,    PdpIpSet   ), 
    CMD_ADD(CMD_A7680C_ECHO_SWITCH           ,   3,  "OK",              "\r\n",   "ERROR",    SendRev,    EchoSwitch ),
    CMD_ADD(CMD_A7680C_CSQ_AUTO_REPORT       ,   3,  "OK",              "\r\n",   "ERROR",    SendRev,    CsqReport  ),
    CMD_ADD(CMD_A7680C_CPIN_READ             ,   3,  "+CPIN:",          "\r\n",   "ERROR",    SendRev,    CpinRead   ), 
    CMD_ADD(CMD_A7680C_NITZ_TIMEUPDATE_SWITCH,   3,  "OK",              "\r\n",   "ERROR",    SendRev,    NitzSwitch ), 
    CMD_ADD(CMD_A7680C_IMSI_READ             ,   3,  NULL,              NULL,     "ERROR",    SendRev,    ImsiRead   ),
    CMD_ADD(CMD_A7680C_SN_READ               ,   3,  NULL,              NULL,     "ERROR",    SendRev,    SnRead     ),
    CMD_ADD(CMD_A7680C_ICCID_READ            ,   3,  "+ICCID",          "\r\n",   "ERROR",    SendRev,    IccidRead  ), 
    CMD_ADD(CMD_A7680C_MODEL_ID_READ         ,   3,  NULL,              NULL,     "ERROR",    SendRev,    ModIdRead  ), 
    CMD_ADD(CMD_A7680C_FWVERSION_READ        ,   3,  "+CGMR:",          "\r\n",   "ERROR",    SendRev,    FwVerRead  ),
    CMD_ADD(CMD_A7680C_ME_INFO_READ          ,   3,  "+CSQ: ",          "\r\n",   "ERROR",    SendRev,    MeInfoRead ),
    CMD_ADD(CMD_A7680C_REGISTER_STATUS_READ  ,   3,  "+CGREG",          "\r\n",   "ERROR",    SendRev,    StatusRead ), 
    CMD_ADD(CMD_A7680C_CMMQTT_START          ,   5,  "+CMQTTSTART:",    "\r\n",   "ERROR",    SendRev,    MqttStart  ), 
    CMD_ADD(CMD_A7680C_CMQTT_ACCQ            ,   5,  "OK",              "\r\n",   "ERROR",    SendRev,    MqttAccq   ), 
    CMD_ADD(CMD_A7680C_CMQTT_CONNECT         ,   5,  "+CMQTTCONNECT:",  "\r\n",   "ERROR",    SendRev,    MqttConnect),
    CMD_ADD(CMD_A7680C_CMQTT_SUBSCRIBE       ,   5,  "\r\n>",           NULL,    "ERROR",    SendRev,    MqttSub    ),
    CMD_ADD(CMD_A7680C_CMQTT_SUB_SEND        ,   5,  "+CMQTTSUB:",      "\r\n",  "ERROR",    SendRev,    MqttSubSend),
    CMD_ADD(CMD_A7680C_CMQTT_TOPIC           ,   5,  "\r\n>",            NULL,   "ERROR",    SendRev,    MqttTopic    ),
    CMD_ADD(CMD_A7680C_CMQTT_TOPIC_SEND      ,   5,  "OK",              "\r\n",  "ERROR",    SendRev,    MqttTopicSend),
    CMD_ADD(CMD_A7680C_CMQTT_PAYLOAD         ,   5,  "\r\n>",           NULL,    "ERROR",    SendRev,    MqttPayload    ),
    CMD_ADD(CMD_A7680C_CMQTT_PAYLOAD_SEND    ,   5,  "OK",              NULL,    "ERROR",    SendRev,    MqttPayloadSend),
    CMD_ADD(CMD_A7680C_CMQTT_PUBISH          ,   5,  "+CMQTTPUB:",     "\r\n",   "ERROR",    SendRev,    MqttPublish),
    CMD_ADD(CMD_A7680C_CMQTT_DISC            ,   5,  "OK",            "\r\n",    "ERROR",    SendRev,    MqttDisc),
    CMD_ADD(CMD_A7680C_CMQTT_REL             ,   5,  "OK",            "\r\n",    "ERROR",    SendRev,    MqttRel),
    CMD_ADD(CMD_A7680C_CMQTT_STOP            ,   5,  "OK",            "\r\n",    "ERROR",    SendRev,    MqttStop),
    CMD_ADD(CMD_A7680C_CLOCK_READ            ,   5,  "+CCLK:",         "\r\n",    "ERROR",    SendRev,    CockRead),
    CMD_ADD(CMD_A7680C_POWEROFF              ,   5,  "OK",              "\r\n",  "ERROR",    SendRev,   PowerOff),
    CMD_ADD(CMD_A7680C_COLD_START_CHECK      ,   5,  "^SIMST:",         NULL,    NULL,       RecvSend,   ColdStart),
    CMD_ADD(CMD_A7680C_MQTTREV               ,   5,  "+CMQTTRXSTART:",  NULL,    NULL,       RecvSend,   mqttRev),

};


tCmd const *CMD_A7680CCmdGet(void)
{
    return cmdList;
}

uint16_t CMD_A7680CCmdNumGet(void)
{
    return (sizeof(cmdList) / sizeof(tCmd));
}
