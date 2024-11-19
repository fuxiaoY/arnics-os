/**
 *******************************************************************************
 * @file     $command_A7680C.h$
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
#ifndef __COMMAND_A7680C_H__
#define __COMMAND_A7680C_H__
#include "../../common/mctProcesser.h"
#include "../../common/mctLib.h"
#include "../../dataplat/mctStruct.h"
#include "../mctList.h"
#include "../../../ulog/ulogDef.h"
#include "../../port/mctDriver.h"
#define CMD_A7680C_ID_BASE                      (int32_t)0

#define CMD_A7680C_PDP_READ                      (int32_t)(CMD_A7680C_ID_BASE +  1)
#define CMD_A7680C_PDP_CONFIG                    (int32_t)(CMD_A7680C_ID_BASE +  2)
#define CMD_A7680C_PDP_IPSET                     (int32_t)(CMD_A7680C_ID_BASE +  3)
#define CMD_A7680C_ECHO_SWITCH                   (int32_t)(CMD_A7680C_ID_BASE +  4)
#define CMD_A7680C_CSQ_AUTO_REPORT               (int32_t)(CMD_A7680C_ID_BASE +  5)
#define CMD_A7680C_CPIN_READ                     (int32_t)(CMD_A7680C_ID_BASE +  6)
#define CMD_A7680C_NITZ_TIMEUPDATE_SWITCH        (int32_t)(CMD_A7680C_ID_BASE +  7)
#define CMD_A7680C_IMSI_READ                     (int32_t)(CMD_A7680C_ID_BASE +  8)
#define CMD_A7680C_SN_READ                       (int32_t)(CMD_A7680C_ID_BASE +  9)
#define CMD_A7680C_ICCID_READ                    (int32_t)(CMD_A7680C_ID_BASE +  10)
#define CMD_A7680C_MODEL_ID_READ                 (int32_t)(CMD_A7680C_ID_BASE +  11)
#define CMD_A7680C_FWVERSION_READ                (int32_t)(CMD_A7680C_ID_BASE +  12)
#define CMD_A7680C_ME_INFO_READ                  (int32_t)(CMD_A7680C_ID_BASE +  13)
#define CMD_A7680C_REGISTER_STATUS_READ          (int32_t)(CMD_A7680C_ID_BASE +  14)
//MQTT
#define CMD_A7680C_CMMQTT_START                  (int32_t)(CMD_A7680C_ID_BASE +  15)
#define CMD_A7680C_CMQTT_ACCQ                    (int32_t)(CMD_A7680C_ID_BASE +  16)
#define CMD_A7680C_CMQTT_CONNECT                 (int32_t)(CMD_A7680C_ID_BASE +  17)
#define CMD_A7680C_CMQTT_SUBSCRIBE               (int32_t)(CMD_A7680C_ID_BASE +  18)
#define CMD_A7680C_CMQTT_SUB_SEND                (int32_t)(CMD_A7680C_ID_BASE +  19)

#define CMD_A7680C_CMQTT_TOPIC                   (int32_t)(CMD_A7680C_ID_BASE +  20)
#define CMD_A7680C_CMQTT_TOPIC_SEND              (int32_t)(CMD_A7680C_ID_BASE +  21)
#define CMD_A7680C_CMQTT_PAYLOAD                 (int32_t)(CMD_A7680C_ID_BASE +  22)
#define CMD_A7680C_CMQTT_PAYLOAD_SEND            (int32_t)(CMD_A7680C_ID_BASE +  23)
#define CMD_A7680C_CMQTT_PUBISH                  (int32_t)(CMD_A7680C_ID_BASE +  24)
#define CMD_A7680C_CLOCK_READ                    (int32_t)(CMD_A7680C_ID_BASE +  25)
#define CMD_A7680C_COLD_START_CHECK              (int32_t)(CMD_A7680C_ID_BASE +  26)
#define CMD_A7680C_POWEROFF                      (int32_t)(CMD_A7680C_ID_BASE +  27)
/* 
1,"IP","cmnet","10.7.70.160,"",0,0,,,,C11
<cid>,<PDP_type>,<APN>[[,<PDP_addr>],<d_comp>,<h_comp>,<ipv4_ctrl>,<request_type>,<PCSCF_discovery>,<IM_CN_Signalling_Flag_Ind>]<CR><LF>
*/
typedef struct 
{
    uint8_t RevCmdValidFlag;
    uint32_t cid;
    char PDP_type[10];
    char APN[10];
    char PDP_addr[20];
    char d_comp[10];
    char h_comp[10];
    char ipv4_ctrl[10];
    char request_type[10];
    char PCSCF_discovery[10];
    char IM_CN_Signalling_Flag_Ind[10];
    char CR[10];
    char LF[10];
}PDP_Context;

//ptr
typedef struct 
{
    uint8_t RevCmdValidFlag;
    uint8_t cid;
    char *PDP_type;
    char *APN;
    char *PDP_addr;
    char *d_comp;
    char *h_comp;
    char *ipv4_ctrl;
    char *request_type;
    char *PCSCF_discovery;
    char *IM_CN_Signalling_Flag_Ind;
    char *CR;
    char *LF;
}PDP_Context_set;

/*
'AT+CGAUTH=<cid>[,<auth_type>[,<passwd>[,<user>]]]
*/
typedef enum
{
    NONE,
    PAP,
    CHAP
}Auth_Type;
typedef struct 
{
    uint8_t cid;
    Auth_Type auth_type;
    char *passwd;
    char *user;
}PDPIP_Auth_Type ;


/* 'ATE[<value>] */
typedef enum
{
    ATE0,
    ATE1
}EchoSwitch;


/* AT+AUTOCSQ=<auto>[,<mode>] */
typedef struct 
{
    uint8_t auto_sw;
    uint8_t mode;
}CsqReport;

/* 'AT+CTZU=<on/off> */
typedef enum
{
    NITZ_OFF,
    NITZ_ON
}NitzSwitch;


/*
+CSQ: <rssi>,<ber>
<rssi> : signal strength indication 
<ber> : channel bit error rate 
*/
typedef struct 
{
    uint8_t RevCmdValidFlag;
    uint8_t rssi;
    uint8_t ber;
}ME_Info;


/* +CGREG: <n>,<stat>[,<lac>,<ci>] */
typedef struct 
{
    uint8_t RevCmdValidFlag;
    uint32_t n;
    uint32_t stat;
    uint32_t lac;
    char ci[10];
}Network_Info;


/* 
AT+CMQTTACCQ=<client_index>,<clientID>[<server_type> 
clientID    :identifies a client,range 0-1
clientID    :unique identifier for the client
server_type :0 MQTT server with TCP
             1 MQTT server with SSL/TLS
*/
typedef enum
{
    MQTT_TCP,
    MQTT_SSL_TLS
}srv_type;

typedef struct 
{
    uint8_t client_index;
    char* clientID;
    srv_type server_type;
}ACCQ_Info;

/* 
AT+CMQTTACCQ=<client_index>,<clientID>[<server_type> 
client_index    :identifies a client,range 0-1
clientID        :a unique identifier for the client
server_type     :0 MQTT server with TCP
                 1 MQTT server with SSL/TLS
*/
typedef enum
{
    SERVER_TCP,
    SERVER_SSL_TLS
}serve_type;

typedef struct 
{
    uint8_t client_index;
    char* clientID;
    serve_type server_type;
}MqttAccq_Info;


/*
AT+CMQTTCONNECT=<client_index>,<server_addr>,<keepalive_time>,<clean_session>
[,<user_name>[,<pass_word>]]

client_index:  identifies a client
server_addr:    server address and port
keepalive_time: The time interval between two messages received from a client
clean_session:  The clean session flag 0/1
user_name:      name of the user,1 to 256 bytes
pass_word:      password of the user,1 to 256 bytes
*/             
typedef struct 
{
    uint8_t client_index;
    char* server_addr;
    uint16_t keepalive_time;
    uint8_t clean_session;
    char* user_name;
    char* pass_word;   
}MqttConnect_Info;

typedef struct 
{
    uint8_t RevCmdValidFlag;
    uint32_t client_index;
    uint32_t errorcode;
}MqttRecv_Info;

//AT+CMQTTSUB=<client_index>,<reqLength>,<qos>[,<dup>]
typedef struct 
{
    uint8_t client_index;
    uint16_t reqLength;
    uint8_t qos;
    uint8_t dup;
}MqttSub_Info;


//AT+CMQTTTOPIC=<client_index>,<req_length>
typedef struct 
{
    uint8_t client_index;
    uint16_t req_length;
}Topic_Info;

//AT+CMQTTPAYLOAD=<client_index>,<req_length>
typedef struct 
{
    uint8_t client_index;
    uint32_t req_length;
}Paylaod_Info;

//AT+CMQTTPUB=<client_index>,<qos>,<pub_timeout>[,<ratained>[,<dup>]]
typedef struct 
{
    uint8_t client_index;
    uint8_t qos;
    uint16_t pub_timeout;
}Publish_Info;

//默认MQTT客户端ID
#define  MQTT_CLIENT_INDEX 0         
#define mct_a7680c_execute(inst,is_expected,expected_a7680c_id, para)                  CMD_Execute(inst,is_expected,expected_a7680c_id,CMD_A7680CCmdGet(), CMD_A7680CCmdNumGet(), para)


extern tCmd const *CMD_A7680CCmdGet(void);
extern uint16_t CMD_A7680CCmdNumGet(void);

#endif
