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
#include "../../../../dataPlat/dataPlatInclude.h"
static bool cmd_EchoClose(MctInstance *inst, void *para)
{
    uint8_t MAX_RETRIES = 3;
    // ATE0  ECHO off
    EchoSwitch echo_state = ATE0;
    for (int i = 0; i < MAX_RETRIES; i++)
    {
        if (mct_a7680c_execute(inst, true, CMD_A7680C_ECHO_SWITCH, &echo_state) == true)
        {
            //printf("ECHO disable successfully!\r\n");
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            printf("ECHO disable failed!\r\n");
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
    PDP_Context pdp_read = {0};
    bool apn_state = false;
    for (int i = 0; i < MAX_RETRIES; i++)
    {
        apn_state = mct_a7680c_execute(inst, true, CMD_A7680C_PDP_READ, &pdp_read);
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
            printf("Match successfully, PDP_type = %s, APN = %s\r\n", pdp_read.PDP_type, pdp_read.APN);
        }
        else
        {
            // AT+CGDCONT=1,"IP","cmnet"
            printf("Match failed, reconfigure PDP...\r\n");
            PDP_Context_set pdp_set;
            pdp_set.cid = 1;
            pdp_set.PDP_type = string_ip;
            pdp_set.APN = string_cmnet;
            for (int i = 0; i < MAX_RETRIES; i++)
            {
                if (mct_a7680c_execute(inst, true, CMD_A7680C_PDP_CONFIG, &pdp_set) == true)
                {
                    printf("Reconfigure PDP done\r\n");
                    break;
                }
                if (i == MAX_RETRIES - 1)
                {
                    printf("PDP config failed!\n");
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
    PDPIP_Auth_Type pdpip_auth;
    pdpip_auth.cid = 1;
    pdpip_auth.auth_type = NONE;
    for (int i = 0; i < MAX_RETRIES; i++)
    {
        if (mct_a7680c_execute(inst, true, CMD_A7680C_PDP_IPSET, &pdpip_auth) == true)
        {
            //printf("PDP-IP set successfully!\r\n");
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            printf("PDP-IP set failed!\r\n");
            return false;
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }
    return true;
}
static bool cmd_CSQ_AutoReportOff(MctInstance *inst, void *para)
{
    // AT+AUTOCSQ=0,0    CSQ Auto report off
    CsqReport csq_rep;
    csq_rep.mode = 0;
    csq_rep.auto_sw = 0;

    if (mct_a7680c_execute(inst, true, CMD_A7680C_CSQ_AUTO_REPORT, &csq_rep) == true)
    {
        //printf("CSQ auto report autodisable successfully!\r\n");

    }
    else
    {
        printf("CSQ auto report disable failed!\r\n");
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
        if (mct_a7680c_execute(inst, true, CMD_A7680C_CPIN_READ, &sim_ready) == true)
        {
            //printf("SIM is ready!\r\n");
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            printf("SIM is not ready!\r\n");
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
    NitzSwitch nitz_state = NITZ_ON;
    for (int i = 0; i < MAX_RETRIES; i++)
    {
        if (mct_a7680c_execute(inst, true, CMD_A7680C_NITZ_TIMEUPDATE_SWITCH, &nitz_state) == true)
        {
            //printf("NITZ enable successfully!\r\n");
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            printf("NITZ enable failed!\r\n");
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
        if (mct_a7680c_execute(inst, true, CMD_A7680C_IMSI_READ, &IMSI_code) == true)
        {
            printf("IMSI number : %s\r\n", IMSI_code);
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            printf("IMSI read failed!\r\n");
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
        if (mct_a7680c_execute(inst, true, CMD_A7680C_SN_READ, &SN_code) == true)
        {
            printf("Serial number : %s\r\n", SN_code);
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            printf("Serial number read failed!\r\n");
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
        if (mct_a7680c_execute(inst, true, CMD_A7680C_ICCID_READ, &ICCD_code) == true)
        {
            printf("ICCD number : %s\r\n", ICCD_code);
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            printf("ICCD number read failed!\r\n");
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
        if (mct_a7680c_execute(inst, true, CMD_A7680C_MODEL_ID_READ, &Model_id) == true)
        {
            printf("Model id : %s\r\n", Model_id);
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            printf("Model id read failed!\r\n");
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
        if (mct_a7680c_execute(inst, true, CMD_A7680C_FWVERSION_READ, &FWversion_id) == true)
        {
            printf("FW version id : %s\r\n", FWversion_id);
            break;
        }
        if (i == MAX_RETRIES - 1)
        {
            printf("FW version id read failed!\r\n");
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
    ME_Info signal;
    memset((void *)&signal, 0x00, sizeof(ME_Info));

    if (false == mct_a7680c_execute(inst, true, CMD_A7680C_ME_INFO_READ, (void *)&signal))
    {
        ULOG_INFO("A7680C: CMD_A7680C_ME_INFO_READ failed");
        return false;
    }
    else
    {
        printf("rssi : %u  ber : %u\r\n", signal.rssi, signal.ber);
        NetworkPara_t* networkPara = (NetworkPara_t*)para;
        networkPara->SignalStrength = signal.rssi;
    }
    
    return true;
}

// register stat record
static bool cmd_CheckCEREG(MctInstance *inst, void *para)
{
    Network_Info net_info;
    if (false == mct_a7680c_execute(inst, true, CMD_A7680C_REGISTER_STATUS_READ, (void *)&net_info))
    {
        ULOG_INFO("A7680C: CMD_A7680C_REGISTER_STATUS_READ failed");
        return false;
    }
    else
    {
        printf("net_info.stat : %d\r\n", net_info.stat);
        NetworkPara_t* networkPara = (NetworkPara_t*)para;
        networkPara->REGstatus = (eRegStatus)net_info.stat;
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
    if (mct_a7680c_execute(inst, true, CMD_A7680C_CMMQTT_START, (void *)NULL) == false)
    {
        printf("MQTT Service Start Failed!\r\n");
        return false;
    }
    // accq
    MqttAccq_Info accq_test;
    char client_id[128] = {0};
    sprintf(client_id, "%s.%s|securemode=3,signmethod=hmacsha1|", global_cfg.ProductKey,global_cfg.DevName);

    accq_test.client_index = MQTT_CLIENT_INDEX;
    accq_test.clientID = client_id;
    accq_test.server_type = SERVER_TCP;
    if (mct_a7680c_execute(inst, true, CMD_A7680C_CMQTT_ACCQ, (void *)&accq_test) == false)
    {
        return false;
    }
    /*
    connect
    from : RTConfig.cpp
    0,"tcp://teeechina.xicp.net:21903",120,1,"8212345678200235&DefaultProductKey","EF78D2329E1E387DCB5CA110E71E3839A134C3CF"
    */
    //网络连接参数

    char server_addr[100] = {0};
    char user_name[100] = {0};
    snprintf(server_addr, sizeof(server_addr), "%s:%u", global_cfg.ServerURL, global_cfg.ServerPort);
    snprintf(user_name, sizeof(user_name), "%s&%s", global_cfg.DevName, global_cfg.ProductKey);
    MqttConnect_Info connect_info;
    connect_info.client_index = MQTT_CLIENT_INDEX;
    connect_info.server_addr = server_addr;
    connect_info.keepalive_time = global_cfg.KeepAlive;
    connect_info.clean_session = 1;
    connect_info.user_name = user_name;
    connect_info.pass_word = global_cfg.DevSecret;
    if (mct_a7680c_execute(inst, true, CMD_A7680C_CMQTT_CONNECT, (void *)&connect_info) == false)
    {
        return false;
    }

    return true;
}

static bool cmd_mqttSubscribe(MctInstance *inst, void *para)
{
    // subscribe
    MqttSub_Info subinfo;
    subinfo.client_index = MQTT_CLIENT_INDEX;
    subinfo.reqLength = strlen(g_MQTopic.Resp);
    subinfo.qos = 1;
    if (mct_a7680c_execute(inst, true, CMD_A7680C_CMQTT_SUBSCRIBE, (void *)&subinfo) == false)
    {
        return false;
    }
    if (mct_a7680c_execute(inst, true, CMD_A7680C_CMQTT_SUB_SEND, &g_MQTopic.Resp) == false)
    {
        return false;
    }
    subinfo.reqLength = strlen(g_MQTopic.ConfigDn);
    if (mct_a7680c_execute(inst, true, CMD_A7680C_CMQTT_SUBSCRIBE, (void *)&subinfo) == false)
    {
        return false;
    }
    if (mct_a7680c_execute(inst, true, CMD_A7680C_CMQTT_SUB_SEND, &g_MQTopic.ConfigDn) == false)
    {
        return false;
    }
    return true;

}
static bool cmd_mqttflow(MctInstance *inst, void *para)
{
    //printf("cmd_mqttflow start----------------------\r\n");
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
    MqttPulish_t *mattPub = (MqttPulish_t *)para;
    // topic
    Topic_Info topicinfo;
    topicinfo.client_index = MQTT_CLIENT_INDEX;
    topicinfo.req_length = strlen(mattPub->Topic);
    if (mct_a7680c_execute(inst, true, CMD_A7680C_CMQTT_TOPIC, &topicinfo) == false)
    {
        return false;
    }
    if (mct_a7680c_execute(inst, true, CMD_A7680C_CMQTT_TOPIC_SEND, mattPub->Topic) == false)
    {
        return false;
    }
    // payload
    Paylaod_Info paylaod_info;
    paylaod_info.client_index = MQTT_CLIENT_INDEX;
    paylaod_info.req_length = mattPub->PublishLen;
    if (mct_a7680c_execute(inst, true, CMD_A7680C_CMQTT_PAYLOAD, (void *)&paylaod_info) == false)
    {
        return false;
    }
    if (mct_a7680c_execute(inst, true, CMD_A7680C_CMQTT_PAYLOAD_SEND, mattPub->PublishData) == false)
    {
        return false;
    }
    // pubilsh
    Publish_Info pub_info;
    pub_info.client_index = MQTT_CLIENT_INDEX;
    pub_info.qos = mattPub->QOS;
    pub_info.pub_timeout = 60;
    if (mct_a7680c_execute(inst, true, CMD_A7680C_CMQTT_PUBISH, &pub_info) == false)
    {
        return false;
    }
    return true;
}
static bool cmd_mqttStop(MctInstance *inst, void *para)
{
     //执行一次 无需关心失败与否
    if (!mct_a7680c_execute(inst, true, CMD_A7680C_CMQTT_DISC, NULL))
    {
        //return false;
    }
     if (!mct_a7680c_execute(inst, true, CMD_A7680C_CMQTT_REL, NULL))
    {
        //return false;
    }   
    if (!mct_a7680c_execute(inst, true, CMD_A7680C_CMQTT_STOP, NULL))
    {
        //return false;
    }
    return true;
}

// time read
static bool cmd_GetZTZEU(MctInstance *inst, void *para)
{
    tWanClock *clock = (tWanClock *)para;
    if (true == mct_a7680c_execute(inst, true, CMD_A7680C_CLOCK_READ, clock))
    {
        return true;
    }
    else
    {
        memset(clock, 0, sizeof(tWanClock));
        ULOG_INFO("A7680C: GetZTZEU failed:");
        return false;
    }
}
static bool cmd_poweroff(MctInstance *inst, void *para)
{
    //执行一次 无需关心失败与否
    mct_a7680c_execute(inst, true, CMD_A7680C_POWEROFF, NULL);
    return true;
}

static bool cmd_NULL(MctInstance *inst, void *para)
{
    return true;
}


//cold start check
static bool cmd_coldstartcheck(MctInstance *inst, void *para)
{
    return mct_a7680c_execute(inst,false,CMD_A7680C_COLD_START_CHECK,NULL);
}

static bool cmd_revHandle(MctInstance *inst,void *para)
{
    mct_a7680c_execute(inst,false,NULL_CMD_SEEK,NULL);
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

};

tCmdApi const *CMD_A7680CApiGet(void)
{
    return funList;
}
