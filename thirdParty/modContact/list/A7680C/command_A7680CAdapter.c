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
 *
 *
 * @note
 *
 *           %2    %4     %3     brief
 *******************************************************************************
 */
#include "command_A7680CAdapter.h"
#include "../../../ulog/ulogDef.h"

static bool cmd_EchoClose(MctInstance *inst, void *para)
{
    uint8_t MAX_RETRIES = 3;
    // ATE0  ECHO off
    echoSwitch_e echo_state = ATE0;
    for (int i = 0; i < MAX_RETRIES; i++)
    {
        if (mct_a7680c_execute(inst, CMD_A7680C_ECHO_SWITCH, &echo_state) == true)
        {
            //ULOG_INFO("ECHO disable successfully!\r\n");
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            ULOG_INFO("ECHO disable failed!\r\n");
            return false;
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }
    return true;
}


static bool cmd_apnConfig(MctInstance *inst, void *para)
{
    uint8_t MAX_RETRIES = 3;
    // AT+CGDCON     APN Config
    pdpContext_t pdp_read = {0};
    bool apn_state = false;
    for (int i = 0; i < MAX_RETRIES; i++)
    {
        apn_state = mct_a7680c_execute(inst,  CMD_A7680C_PDP_READ, &pdp_read);
        if (apn_state == true)
            break;
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }

    {
        char string_ip[] = {"IP"};
        char string_cmnet[] = {"cmnet"};
        /*
        char string_ctnet[] = {"ctnet"};

        if ((NULL != hexhex((uint8_t *)pdp_read.PDP_type, (uint8_t *)string_ip,sizeof(pdp_read.PDP_type),strlen(string_ip))) &&  \
              (NULL!= hexhex((uint8_t *)pdp_read.APN, (uint8_t *)string_cmnet,sizeof(pdp_read.APN),strlen(string_cmnet))|| \
              NULL!= hexhex((uint8_t *)pdp_read.APN, (uint8_t *)string_ctnet,sizeof(pdp_read.APN),strlen(string_ctnet))))
              */
        if ((NULL != hexhex((uint8_t *)pdp_read.PDP_type, (uint8_t *)string_ip,sizeof(pdp_read.PDP_type),strlen(string_ip))))

        {
            ULOG_INFO("Match successfully, PDP_type = %s, APN = %s\r\n", pdp_read.PDP_type, pdp_read.APN);
        }
        else
        {
            // AT+CGDCONT=1,"IP","cmnet"
            ULOG_INFO("Match failed, reconfigure PDP...\r\n");
            pdpContextSet_t pdp_set;
            pdp_set.cid = 1;
            pdp_set.PDP_type = string_ip;
            pdp_set.APN = string_cmnet;
            for (int i = 0; i < MAX_RETRIES; i++)
            {
                if (mct_a7680c_execute(inst,  CMD_A7680C_PDP_CONFIG, &pdp_set) == true)
                {
                    ULOG_INFO("Reconfigure PDP done\r\n");
                    break;
                }
                if (i == MAX_RETRIES - 1)
                {
                    ULOG_INFO("PDP config failed!\n");
                    return false;
                }
                MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
            }
            return false;
        }
    }


    return true;
}

static bool cmd_PDP_IPConfig(MctInstance *inst, void *para)
{
    // AT+CGAUTH=1,0  GPRS PDP-IP connection config
    uint8_t MAX_RETRIES = 3;
    pdpIpAuthType_t pdpip_auth;
    pdpip_auth.cid = 1;
    pdpip_auth.auth_type = NONE;
    for (int i = 0; i < MAX_RETRIES; i++)
    {
        if (mct_a7680c_execute(inst, CMD_A7680C_PDP_IPSET, &pdpip_auth) == true)
        {
            //ULOG_INFO("PDP-IP set successfully!\r\n");
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            ULOG_INFO("PDP-IP set failed!\r\n");
            return false;
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }
    return true;
}
static bool cmd_CSQ_AutoReportOff(MctInstance *inst, void *para)
{
    // AT+AUTOCSQ=0,0    CSQ Auto report off
    csqReport_t csq_rep;
    csq_rep.mode = 0;
    csq_rep.auto_sw = 0;

    if (mct_a7680c_execute(inst,  CMD_A7680C_CSQ_AUTO_REPORT, &csq_rep) == true)
    {
        //ULOG_INFO("CSQ auto report autodisable successfully!\r\n");

    }
    else
    {
        ULOG_INFO("CSQ auto report disable failed!\r\n");
        //return false; 允许失败
    }

    
    return true;  
}
static bool cmd_checkPinReady(MctInstance *inst, void *para)
{
    // AT+CPIN?  SIM is ready？
    uint8_t MAX_RETRIES = 3;
    char sim_ready[100] = {0};
    for (int i = 0; i < MAX_RETRIES; i++) 
    {
        if (mct_a7680c_execute(inst,  CMD_A7680C_CPIN_READ, &sim_ready) == true)
        {
            //ULOG_INFO("SIM is ready!\r\n");
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            ULOG_INFO("SIM is not ready!\r\n");
            return false;
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }
    return true;  
}


static bool cmd_UpdateZonNITZ(MctInstance *inst, void *para)
{
    // AT+CTZU=1	 updete zone NITZ
    uint8_t MAX_RETRIES = 3;
    nitzSwitch_t nitz_state = NITZ_ON;
    for (int i = 0; i < MAX_RETRIES; i++)
    {
        if (mct_a7680c_execute(inst,  CMD_A7680C_NITZ_TIMEUPDATE_SWITCH, &nitz_state) == true)
        {
            //ULOG_INFO("NITZ enable successfully!\r\n");
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            ULOG_INFO("NITZ enable failed!\r\n");
            return false;
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }
    return true;
}

static bool cmd_ReadIMSI(MctInstance *inst, void *para)
{
    // AT+CIMI  IMSI read
    uint8_t MAX_RETRIES = 3;
    char IMSI_code[100] = {0};
    for (int i = 0; i < MAX_RETRIES; i++)
    {
        if (mct_a7680c_execute(inst,  CMD_A7680C_IMSI_READ, &IMSI_code) == true)
        {
            ULOG_INFO("IMSI number : %s\r\n", IMSI_code);
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            ULOG_INFO("IMSI read failed!\r\n");
            return false;
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }
    return true;
}
static bool cmd_SnNumberRead(MctInstance *inst, void *para)
{
    // AT+CGSN  SN bumber read
    uint8_t MAX_RETRIES = 3;

    char SN_code[100] = {0};
    for (int i = 0; i < MAX_RETRIES; i++)
    {
        if (mct_a7680c_execute(inst,  CMD_A7680C_SN_READ, &SN_code) == true)
        {
            ULOG_INFO("Serial number : %s\r\n", SN_code);
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            ULOG_INFO("Serial number read failed!\r\n");
            return false;
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }
    return true;
}

static bool cmd_ICCIDRead(MctInstance *inst, void *para)
{
    // AT+CGSN  SN bumber read
    uint8_t MAX_RETRIES = 3;
    char ICCD_code[100] = {0};
    for (int i = 0; i < MAX_RETRIES; i++)
    {
        if (mct_a7680c_execute(inst,  CMD_A7680C_ICCID_READ, &ICCD_code) == true)
        {
            ULOG_INFO("ICCD number : %s\r\n", ICCD_code);
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            ULOG_INFO("ICCD number read failed!\r\n");
            return false;
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }
    return true;
}

static bool cmd_CGMMRead(MctInstance *inst, void *para)
{
    uint8_t MAX_RETRIES = 3;
    // AT+CGMM   model id read
    char Model_id[100] = {0};
    for (int i = 0; i < MAX_RETRIES; i++)
    {
        if (mct_a7680c_execute(inst,  CMD_A7680C_MODEL_ID_READ, &Model_id) == true)
        {
            ULOG_INFO("Model id : %s\r\n", Model_id);
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            ULOG_INFO("Model id read failed!\r\n");
            return false;
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }
    return true;
}

static bool cmd_CGMRRead(MctInstance *inst, void *para)
{
    uint8_t MAX_RETRIES = 3;
    // AT+CGMR   FW version id read
    char FWversion_id[100] = {0};
    for (int i = 0; i < MAX_RETRIES; i++)
    {
        if (mct_a7680c_execute(inst,  CMD_A7680C_FWVERSION_READ, &FWversion_id) == true)
        {
            ULOG_INFO("FW version id : %s\r\n", FWversion_id);
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            ULOG_INFO("FW version id read failed!\r\n");
            return false;
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }
    return true;
}
static bool cmd_InfoRead(MctInstance *inst, void *para)
{
        // AT+CIMI  IMSI read
    if(!cmd_ReadIMSI(inst, para))
    {
        return false;
    }
    // AT+CGSN  SN bumber read
    if(!cmd_SnNumberRead(inst, para))
    {
        return false;
    }

    // AT+CICCID  SIM ICCID read
    if(!cmd_ICCIDRead(inst, para))
    {
        return false;
    }
    // AT+CGMM   model id read
    if(!cmd_CGMMRead(inst, para))
    {
        return false;
    }
    // AT+CGMR   FW version id read
    if(!cmd_CGMRRead(inst, para))
    {
        return false;
    }
		return true;
}

static bool cmd_autoConfig(MctInstance *inst, void *para)
{
    MCT_DELAY(1000);
    // ATE0  ECHO off
    if(!cmd_EchoClose(inst, para))
    {
        return false;
    }
    // AT+CGDCON     APN Config
    if(!cmd_apnConfig(inst, para))
    {
        return false;
    }
    // AT+CGAUTH=1,0  GPRS PDP-IP connection config
    if(!cmd_PDP_IPConfig(inst, para))
    {
        return false;
    }
    // AT+AUTOCSQ=0,0    CSQ Auto report off
    if(!cmd_CSQ_AutoReportOff(inst, para))
    {
        return false;
    }
    // AT+CPIN?  SIM is ready？
    if(!cmd_checkPinReady(inst, para))
    {
        return false;
    }
    // AT+CTZU=1	 updete zone NITZ
    if(!cmd_UpdateZonNITZ(inst, para))
    {
        return false;
    }
    if(!cmd_InfoRead(inst, para))
    {
        return false;
    }
    return true;
}

// signal strength record
static bool cmd_ModemUpdateSignal(MctInstance *inst, void *para)
{
    meInfo_t signal;
    memset((void *)&signal, 0x00, sizeof(meInfo_t));

    if (false == mct_a7680c_execute(inst,  CMD_A7680C_ME_INFO_READ, (void *)&signal))
    {
        ULOG_INFO("A7680C: CMD_A7680C_ME_INFO_READ failed");
        return false;
    }
    else
    {
        ULOG_INFO("rssi : %u  ber : %u\r\n", signal.rssi, signal.ber);
        networkPara_t* networkPara = (networkPara_t*)para;
        networkPara->SignalStrength = signal.rssi;
    }
    
    return true;
}

// register stat record
static bool cmd_CheckCEREG(MctInstance *inst, void *para)
{
    networkInfo_t net_info;
    if (false == mct_a7680c_execute(inst, CMD_A7680C_REGISTER_STATUS_READ, (void *)&net_info))
    {
        ULOG_INFO("A7680C: CMD_A7680C_REGISTER_STATUS_READ failed");
        return false;
    }
    else
    {
        ULOG_INFO("net_info.stat : %d\r\n", net_info.stat);
        networkPara_t* networkPara = (networkPara_t*)para;
        networkPara->REGstatus = (regStatus_e)net_info.stat;
    }
    return true;
}

static bool cmd_NetworkCheck(MctInstance *inst, void *para)
{
    cmd_ModemUpdateSignal(inst,para);
    cmd_CheckCEREG(inst,para);

    return true;
}

// mqtt connect and subscribe


static bool cmd_mqttConnect(MctInstance *inst, void *para)
{
    // start
    if (mct_a7680c_execute(inst,  CMD_A7680C_CMMQTT_START, (void *)NULL) == false)
    {
        ULOG_INFO("MQTT Service Start Failed!\r\n");
        return false;
    }
    // accq
    mqttAccqInfo_t accq_test;
    char client_id[128] = {0};
    sprintf(client_id, "%s.%s|securemode=3,signmethod=hmacsha1|",(char*)MCT_PTR(mqtt_key),(char*)MCT_PTR(device_name));

    accq_test.client_index = MQTT_CLIENT_INDEX;
    accq_test.clientID = client_id;
    accq_test.server_type = SERVER_TCP;
    if (mct_a7680c_execute(inst,  CMD_A7680C_CMQTT_ACCQ, (void *)&accq_test) == false)
    {
        return false;
    }
    /*
    connect
    from : RTConfig.cpp
    0,"tcp://arong.xicp.net:21903",120,1,"1&DefaultProductKey","EF78D2329E1E387DCB5CA110E71E3839A13C3CF"
    */
   
    //网络连接参数
    char server_addr[100] = {0};
    char user_name[100] = {0};
    uint16_t server_port = 0;
    MCT_GET(server_port,&server_port, sizeof(server_port));
    snprintf(server_addr, sizeof(server_addr), "tcp://%s:%u", (char*)MCT_PTR(server_url),server_port);
    snprintf(user_name, sizeof(user_name), "%s&%s",(char*)MCT_PTR(device_name), (char*)MCT_PTR(mqtt_key));
    mqttConnectInfo_t connect_info;
    connect_info.client_index = MQTT_CLIENT_INDEX;
    connect_info.server_addr = server_addr;
    MCT_GET(mqtt_keep_alive, &connect_info.keepalive_time, sizeof(connect_info.keepalive_time));
    connect_info.clean_session = 1;
    connect_info.user_name = user_name;
    connect_info.pass_word = (char*)MCT_PTR(mqtt_secret);
    if (mct_a7680c_execute(inst,  CMD_A7680C_CMQTT_CONNECT, (void *)&connect_info) == false)
    {
        return false;
    }

    return true;
}

static bool cmd_mqttSubscribe(MctInstance *inst, void *para)
{
    // subscribe
    mqttSubInfo_t subinfo;
    subinfo.client_index = MQTT_CLIENT_INDEX;
    subinfo.reqLength = strlen(MCT_PTR(mqtt_response_topic));
    subinfo.qos = 1;
    if (mct_a7680c_execute(inst,  CMD_A7680C_CMQTT_SUBSCRIBE, (void *)&subinfo) == false)
    {
        return false;
    }
    if (mct_a7680c_execute(inst,  CMD_A7680C_CMQTT_SUB_SEND, MCT_PTR(mqtt_response_topic)) == false)
    {
        return false;
    }
    subinfo.reqLength = strlen(MCT_PTR(mqtt_config_down_topic));
    if (mct_a7680c_execute(inst,  CMD_A7680C_CMQTT_SUBSCRIBE, (void *)&subinfo) == false)
    {
        return false;
    }
    if (mct_a7680c_execute(inst,  CMD_A7680C_CMQTT_SUB_SEND, MCT_PTR(mqtt_config_down_topic)) == false)
    {
        return false;
    }
    return true;

}
static bool cmd_mqttflow(MctInstance *inst, void *para)
{
    //ULOG_INFO("cmd_mqttflow start----------------------\r\n");
    // connect
    if(!cmd_mqttConnect(inst, para))
    {
        return false;
    }
    // subscribe
    if(!cmd_mqttSubscribe(inst, para))
    {
        return false;
    }
    return true;
}


static bool cmd_mqttpublish(MctInstance *inst, void *para)
{
    mqttPulish_t *mattPub = (mqttPulish_t *)para;
    // topic
    topicInfo_t topicinfo;
    topicinfo.client_index = MQTT_CLIENT_INDEX;
    topicinfo.req_length = strlen(mattPub->Topic);
    if (mct_a7680c_execute(inst, CMD_A7680C_CMQTT_TOPIC, &topicinfo) == false)
    {
        return false;
    }
    if (mct_a7680c_execute(inst,  CMD_A7680C_CMQTT_TOPIC_SEND, mattPub->Topic) == false)
    {
        return false;
    }
    // payload
    payloadInfo_t paylaod_info;
    paylaod_info.client_index = MQTT_CLIENT_INDEX;
    paylaod_info.req_length = mattPub->PublishLen;
    if (mct_a7680c_execute(inst,  CMD_A7680C_CMQTT_PAYLOAD, (void *)&paylaod_info) == false)
    {
        return false;
    }
    if (mct_a7680c_execute(inst, CMD_A7680C_CMQTT_PAYLOAD_SEND, mattPub->PublishData) == false)
    {
        return false;
    }
    // pubilsh
    publishInfo_t pub_info;
    pub_info.client_index = MQTT_CLIENT_INDEX;
    pub_info.qos = mattPub->QOS;
    pub_info.pub_timeout = 60;
    if (mct_a7680c_execute(inst,  CMD_A7680C_CMQTT_PUBISH, &pub_info) == false)
    {
        return false;
    }
    return true;
}
static bool cmd_mqttStop(MctInstance *inst, void *para)
{
     //执行一次 无需关心失败与否
    if (!mct_a7680c_execute(inst,  CMD_A7680C_CMQTT_DISC, NULL))
    {
        //return false;
    }
     if (!mct_a7680c_execute(inst,  CMD_A7680C_CMQTT_REL, NULL))
    {
        //return false;
    }   
    if (!mct_a7680c_execute(inst,  CMD_A7680C_CMQTT_STOP, NULL))
    {
        //return false;
    }
    return true;
}

// time read
static bool cmd_GetZTZEU(MctInstance *inst, void *para)
{
    wanClock_t *clock = (wanClock_t *)para;
    if (true == mct_a7680c_execute(inst,  CMD_A7680C_CLOCK_READ, clock))
    {
        return true;
    }
    else
    {
        memset(clock, 0, sizeof(wanClock_t));
        ULOG_INFO("A7680C: GetZTZEU failed:");
        return false;
    }
}
static bool cmd_poweroff(MctInstance *inst, void *para)
{
    //执行一次 无需关心失败与否
    mct_a7680c_execute(inst,  CMD_A7680C_POWEROFF, NULL);
    return true;
}

static bool cmd_NULL(MctInstance *inst, void *para)
{
    return true;
}


//cold start check
static bool cmd_coldstartcheck(MctInstance *inst, void *para)
{
    return mct_a7680c_execute(inst,CMD_A7680C_COLD_START_CHECK,NULL);
}

static bool cmd_revHandle(MctInstance *inst,void *para)
{
    mct_a7680c_execute(inst,NULL_CMD_SEEK,NULL);
    return true;
}

static bool cmd_httpConnect(MctInstance *inst, void *para)
{
    uint8_t MAX_RETRIES = 3;
    httpAction_t *url_info = (httpAction_t *)para;
    
    if (url_info == NULL || url_info->ip == NULL)
    {
        ULOG_INFO("HTTP Connect: Invalid URL parameters\r\n");
        return false;
    }
    
    // HTTP服务初始化

    for (int i = 0; i < MAX_RETRIES; i++)
    {
        if (mct_a7680c_execute(inst, CMD_A7680C_HTTPINIT, NULL) == true)
        {
            ULOG_INFO("HTTP Init Successfully!\r\n");
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            ULOG_INFO("HTTP Init Failed!\r\n");
            return false;
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }
    
    // 构建完整URL
    char full_url[256] = {0};
    snprintf(full_url, sizeof(full_url), "http://%s:%u%s", url_info->ip, url_info->port, url_info->file_path);
    
    // 设置HTTP参数 - URL
    httpParaInfo_t http_para;
    http_para.param_tag = "URL";
    http_para.param_value = full_url;
    
    for (int i = 0; i < MAX_RETRIES; i++)
    {
        if (mct_a7680c_execute(inst, CMD_A7680C_HTTPPARA, &http_para) == true)
        {
            ULOG_INFO("HTTP URL Set Successfully: %s\r\n", full_url);
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            ULOG_INFO("HTTP URL Set Failed!\r\n");
            return false;
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }
    
    // 设置HTTP参数 - Content-Type
    http_para.param_tag = "CONTENT";
    http_para.param_value = "application/octet-stream";
    
    if (mct_a7680c_execute(inst, CMD_A7680C_HTTPPARA, &http_para) == true)
    {
        ULOG_INFO("HTTP Content-Type Set Successfully!\r\n");
    }
    else
    {
        ULOG_INFO("HTTP Content-Type Set Failed!\r\n");
        // 允许失败，继续执行
    }
    // 执行HTTP GET请求
    httpActionInfo_t http_action;
    memset(&http_action, 0, sizeof(httpActionInfo_t));
    http_action.http_action_pack.method = 0; // GET方法

    
    for (int i = 0; i < MAX_RETRIES; i++)
    {
        if (mct_a7680c_execute(inst, CMD_A7680C_HTTPACTION, &http_action) == true)
        {
            ULOG_INFO("HTTP GET Request Successfully!\r\n");
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            ULOG_INFO("HTTP GET Request Failed!\r\n");
            return false;
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }

    return true;
}

static bool cmd_httpGet(MctInstance *inst, void *para)
{
    httpAction_t *http_info = (httpAction_t *)para;
    
    if (http_info == NULL)
    {
        ULOG_INFO("HTTP GET: Invalid parameters\r\n");
        return false;
    }
    
    // 查询并读取数据
    httpReadInfo_t read_info = {0};
    memset(&read_info, 0, sizeof(httpReadInfo_t));
    // 传输起始地址
    read_info.start_pos = http_info->block_num *http_info->block_len;
    // 传输长度
    read_info.length = http_info->block_len;
    // 最大缓冲大小 = 传输长度
    read_info.max_buffer_size = http_info->block_len;
    // 数据指针
    read_info.data_buffer =http_info->p_file_data;

    
    if (mct_a7680c_execute(inst, CMD_A7680C_HTTPREAD, &read_info) == true)
    {
        if (read_info.RevCmdValidFlag && read_info.data_length > 0)
        {
            ULOG_INFO("HTTP Data Downloaded: %u bytes\r\n", read_info.data_length);
            // 更新实际下载的数据长度
            http_info->file_len = read_info.data_length;
            return true;
        }
        else
        {
            ULOG_INFO("No HTTP Response Data Available\r\n");
            return false;
        }
    }
    else
    {
        ULOG_INFO("HTTP Data Download Failed!\r\n");
        return false;
    }
}

static bool cmd_httpClose(MctInstance *inst, void *para)
{
    uint8_t MAX_RETRIES = 3;    
    
    // 终止HTTP服务
    for (int i = 0; i < MAX_RETRIES; i++)
    {
        if (mct_a7680c_execute(inst, CMD_A7680C_HTTPTERM, NULL) == true)
        {
            ULOG_INFO("HTTP Service Terminated Successfully!\r\n");
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            ULOG_INFO("HTTP Service Terminate Failed!\r\n");
            // 允许失败，因为可能服务已经关闭
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }
    
    return true;
}


static const tCmdApi funList[] =
    {
        {.id = CMD_ECHO_CLOSE, .fun = cmd_NULL},
        {.id = CMD_WORKLOCKSET, .fun = cmd_NULL},
        {.id = CMD_MODEM_INFO, .fun = cmd_NULL},
        {.id = CMD_PINCODE_SET, .fun = cmd_NULL},
        {.id = CMD_POWER_OFF, .fun = cmd_NULL},
        {.id = CMD_AUTO_CONFIG, .fun = cmd_autoConfig},
        {.id = CMD_NETWORK_CHECK, .fun = cmd_NetworkCheck},
        {.id = CMD_MQTTFLOW, .fun = cmd_mqttflow},
        {.id = CMD_MQTTPUBLISH, .fun = cmd_mqttpublish},
        {.id = CMD_MQTTREV, .fun = cmd_NULL},
        {.id = CMD_MODEM_CLOCK_GET, .fun = cmd_GetZTZEU},
        {.id = CMD_MQTTREVPUSH, .fun = cmd_NULL},
        {.id = CMD_MQTTSTOP,   .fun = cmd_mqttStop},
        {.id = CMD_STICKFRAME, .fun = cmd_NULL},
        {.id = CMD_COLDSTARTUP_CHECK, .fun = cmd_coldstartcheck},
        {.id = CMD_POWEROFF, .fun = cmd_poweroff},
        {.id = CMD_REV_FLOW,   .fun = cmd_revHandle},
        {.id = CMD_HTTPCONNECT,   .fun = cmd_httpConnect},
        {.id = CMD_HTTPGET,   .fun = cmd_httpGet},
        {.id = CMD_HTTPCLOSE,   .fun = cmd_httpClose},
};

tCmdApi const *CMD_A7680CApiGet(void)
{
    return funList;
}
