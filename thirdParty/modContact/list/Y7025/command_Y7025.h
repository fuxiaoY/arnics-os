/**
 *******************************************************************************
 * @file     $command_Y7025.h$
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
#ifndef __COMMAND_Y7025_H__
#define __COMMAND_Y7025_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "../../core/mctCore.h"
#include "../../common/mctStruct.h"
#include "../../dataPlat/dataLink.h"
#include "../../port/mctDriver.h"

#define CMD_Y7025_ID_BASE                           (uint16_t)0

#define CMD_Y7025_IMEI_GET                          (uint16_t)(CMD_Y7025_ID_BASE +  1)
#define CMD_Y7025_IMSI_GET                          (uint16_t)(CMD_Y7025_ID_BASE +  2)
#define CMD_Y7025_VERSION_GET                       (uint16_t)(CMD_Y7025_ID_BASE +  3)
#define CMD_Y7025_ICCID_GET                         (uint16_t)(CMD_Y7025_ID_BASE +  4)
#define CMD_Y7025_SYNC_CLOCK                        (uint16_t)(CMD_Y7025_ID_BASE +  5)
#define CMD_Y7025_CLOCK_GET                         (uint16_t)(CMD_Y7025_ID_BASE +  6)
#define CMD_Y7025_APP_CFG_APN                       (uint16_t)(CMD_Y7025_ID_BASE +  7)
#define CMD_Y7025_APP_CFG_USER                      (uint16_t)(CMD_Y7025_ID_BASE +  8)
#define CMD_Y7025_APP_CFG_PASS                      (uint16_t)(CMD_Y7025_ID_BASE +  9)
#define CMD_Y7025_ENABLE_APP_FUN                    (uint16_t)(CMD_Y7025_ID_BASE + 10)
#define CMD_Y7025_DISBALE_APP_FUN                   (uint16_t)(CMD_Y7025_ID_BASE + 11)
#define CMD_Y7025_INQUIRE_APP_FUN                   (uint16_t)(CMD_Y7025_ID_BASE + 12)
#define CMD_Y7025_NTP_SERVER                        (uint16_t)(CMD_Y7025_ID_BASE + 13)
#define CMD_Y7025_NTP_ACTION                        (uint16_t)(CMD_Y7025_ID_BASE + 14)
#define CMD_Y7025_PINCODE_INQUIRE                   (uint16_t)(CMD_Y7025_ID_BASE + 15)
#define CMD_Y7025_PINCODE_SET                       (uint16_t)(CMD_Y7025_ID_BASE + 16)
#define CMD_Y7025_FILE_INIT                         (uint16_t)(CMD_Y7025_ID_BASE + 17)
#define CMD_Y7025_FILE_READ_READY                   (uint16_t)(CMD_Y7025_ID_BASE + 18)
#define CMD_Y7025_FILE_READ                         (uint16_t)(CMD_Y7025_ID_BASE + 19)
#define CMD_Y7025_FILE_SIZE                         (uint16_t)(CMD_Y7025_ID_BASE + 20)
#define CMD_Y7025_FILE_DELETE                       (uint16_t)(CMD_Y7025_ID_BASE + 21)
#define CMD_Y7025_FILE_WRITE_READY                  (uint16_t)(CMD_Y7025_ID_BASE + 22)
#define CMD_Y7025_FILE_WRITE                        (uint16_t)(CMD_Y7025_ID_BASE + 23)
#define CMD_Y7025_FILE_FREE                         (uint16_t)(CMD_Y7025_ID_BASE + 24)
#define CMD_Y7025_SYS_CHOICE                        (uint16_t)(CMD_Y7025_ID_BASE + 25)
#define CMD_Y7025_NB_ENABLE                         (uint16_t)(CMD_Y7025_ID_BASE + 26)
#define CMD_Y7025_NB_CATM_CHOICE                    (uint16_t)(CMD_Y7025_ID_BASE + 27)
#define CMD_Y7025_ENABLE_MUTI_CHANNEL               (uint16_t)(CMD_Y7025_ID_BASE + 28)
#define CMD_Y7025_MUTI_CHANNEL_INQUIRE              (uint16_t)(CMD_Y7025_ID_BASE + 29)
#define CMD_Y7025_DATA_FORMAT_SET                   (uint16_t)(CMD_Y7025_ID_BASE + 30)
#define CMD_Y7025_OPERATOR_INQUIRE                  (uint16_t)(CMD_Y7025_ID_BASE + 31)
#define CMD_Y7025_SYSTEM_GET                        (uint16_t)(CMD_Y7025_ID_BASE + 32)

#define CMD_Y7025_DATA_REGISTER                     (uint16_t)(CMD_Y7025_ID_BASE + 34)
#define CMD_Y7025_UE_INFO_GET                       (uint16_t)(CMD_Y7025_ID_BASE + 35)
#define CMD_Y7025_ADJACANT_INFO_GET                 (uint16_t)(CMD_Y7025_ID_BASE + 36)
#define CMD_Y7025_PDP_INQUIRE                       (uint16_t)(CMD_Y7025_ID_BASE + 37)
#define CMD_Y7025_AUTH_SET                          (uint16_t)(CMD_Y7025_ID_BASE + 38)
#define CMD_Y7025_APN_SET                           (uint16_t)(CMD_Y7025_ID_BASE + 39)
#define CMD_Y7025_PDP_OPEN                          (uint16_t)(CMD_Y7025_ID_BASE + 40)
#define CMD_Y7025_PDP_CLOSE                         (uint16_t)(CMD_Y7025_ID_BASE + 41)
#define CMD_Y7025_DNS                               (uint16_t)(CMD_Y7025_ID_BASE + 42)
#define CMD_Y7025_TCP_NEW                           (uint16_t)(CMD_Y7025_ID_BASE + 43)
#define CMD_Y7025_TCP_CONNECT                       (uint16_t)(CMD_Y7025_ID_BASE + 44)
#define CMD_Y7025_TCP_SEND                          (uint16_t)(CMD_Y7025_ID_BASE + 45)
#define CMD_Y7025_TCP_CLOSE                         (uint16_t)(CMD_Y7025_ID_BASE + 46)
#define CMD_Y7025_CONN_OPEN                         (uint16_t)(CMD_Y7025_ID_BASE + 47)
#define CMD_Y7025_CONN_CLOSE                        (uint16_t)(CMD_Y7025_ID_BASE + 48)
#define CMD_Y7025_DATALEN_INQUIRE                   (uint16_t)(CMD_Y7025_ID_BASE + 49)
#define CMD_Y7025_DATA_REC                          (uint16_t)(CMD_Y7025_ID_BASE + 50)
#define CMD_Y7025_DATA_SEND_READY                   (uint16_t)(CMD_Y7025_ID_BASE + 51)
#define CMD_Y7025_DATA_SEND                         (uint16_t)(CMD_Y7025_ID_BASE + 52)
#define CMD_Y7025_LAST_GASP                         (uint16_t)(CMD_Y7025_ID_BASE + 53)
#define CMD_Y7025_EDRX_CLOSE                        (uint16_t)(CMD_Y7025_ID_BASE + 54)
#define CMD_Y7025_BAND_SET                          (uint16_t)(CMD_Y7025_ID_BASE + 55)
#define CMD_Y7025_PSM_CLOSE                         (uint16_t)(CMD_Y7025_ID_BASE + 56)
#define CMD_Y7025_ENABLE_CFUN                       (uint16_t)(CMD_Y7025_ID_BASE + 57)
#define CMD_Y7025_DISABLE_CFUN                      (uint16_t)(CMD_Y7025_ID_BASE + 58)
#define CMD_Y7025_SUB_VERSION_GET                   (uint16_t)(CMD_Y7025_ID_BASE + 59)
#define CMD_Y7025_POWER_OFF                         (uint16_t)(CMD_Y7025_ID_BASE + 60)

#define CMD_Y7025_WORKLOCK                       (uint16_t)(CMD_Y7025_ID_BASE + 61)
#define CMD_Y7025_CPSMS                          (uint16_t)(CMD_Y7025_ID_BASE + 62)
#define CMD_Y7025_ECHO_OFF                       (uint16_t)(CMD_Y7025_ID_BASE + 63)
#define CMD_Y7025_CEREG                          (uint16_t)(CMD_Y7025_ID_BASE + 64)
#define CMD_Y7025_SIGNAL_GET                     (uint16_t)(CMD_Y7025_ID_BASE + 65)
#define CMD_Y7025_NCCID                          (uint16_t)(CMD_Y7025_ID_BASE + 66)
#define CMD_Y7025_CGMM                           (uint16_t)(CMD_Y7025_ID_BASE + 67)
#define CMD_Y7025_CGMR                           (uint16_t)(CMD_Y7025_ID_BASE + 68)

#define CMD_Y7025_MQTTNEW                       (uint16_t)(CMD_Y7025_ID_BASE +  69)
#define CMD_Y7025_MQTTCON                       (uint16_t)(CMD_Y7025_ID_BASE +  70)
#define CMD_Y7025_MQTTSUB                       (uint16_t)(CMD_Y7025_ID_BASE +  71)
#define CMD_Y7025_MQTTPUB                       (uint16_t)(CMD_Y7025_ID_BASE +  72)
#define CMD_Y7025_MQTTCLOSE                     (uint16_t)(CMD_Y7025_ID_BASE +  73)
#define CMD_Y7025_BOOTZTZEU                     (uint16_t)(CMD_Y7025_ID_BASE +  74)
#define CMD_Y7025_MQTTREV                       (uint16_t)(CMD_Y7025_ID_BASE +  75)

#define CMD_Y7025_HTTPCREATE                    (uint16_t)(CMD_Y7025_ID_BASE +  76)
#define CMD_Y7025_HTTPHEADSET                   (uint16_t)(CMD_Y7025_ID_BASE +  77)
#define CMD_Y7025_HTTPHEADGET                   (uint16_t)(CMD_Y7025_ID_BASE +  78)
#define CMD_Y7025_HTTPCONTENTSET                (uint16_t)(CMD_Y7025_ID_BASE +  79)
#define CMD_Y7025_HTTPCONTENTGET                (uint16_t)(CMD_Y7025_ID_BASE +  80)
#define CMD_Y7025_HTTPSEND                      (uint16_t)(CMD_Y7025_ID_BASE +  81)
#define CMD_Y7025_HTTPCLOSE                     (uint16_t)(CMD_Y7025_ID_BASE +  82)
#define CMD_Y7025_HTTPCFG                       (uint16_t)(CMD_Y7025_ID_BASE +  83)   
#define CMD_Y7025_CMNTP                         (uint16_t)(CMD_Y7025_ID_BASE +  84)   
#define CMD_Y7025_MQTTDSICON                    (uint16_t)(CMD_Y7025_ID_BASE +  85)   
#define mct_y7025_execute(inst,expected_a7680c_id, para)    CMD_Execute(inst,expected_a7680c_id,CMD_Y7025CmdGet(), CMD_Y7025CmdNumGet(), para)

typedef struct
{
    uint8_t                           *addr;
    uint16_t                          maxLen;
}tStringReq;
typedef enum
{
    PIN_OK                          = (uint8_t)0,
    PIN_ERROR,
    PIN_INPUT,
}ePincode;
typedef struct
{
    int8_t            rssi;
    int8_t            ber;
}tsignalQuality;
typedef struct
{
    char* Serverip;
    uint16_t server_port;
    char* mqtt_key;
    char* DeviceName;
    char* DeviceSecret;
    uint16_t mqtt_connect_timeout;
    uint16_t mqtt_keep_alive;
}mqttConnet_t;

typedef enum
{
    topic_Other,
    topic_Command,			//下行订阅主题
    topic_ConfigDn,		//下行配置主题
}MqTType;
typedef struct 
{
    uint8_t   RevCmdValidFlag; //接收命令是否有效
    MqTType mqttType; //MQTT类型
    uint16_t  RevCmdLen; //接收命令长度
    char   RevCmd[256]; //数据缓存
    mqttPulish_t mqttPub; //发布数据
}tWanData;

typedef struct
{
    uint32_t                      cs;
    regStatus_e                  value;
}tRegStus;
typedef struct
{
    uint32_t                      cs;
    uint16_t                      data;
}tPincode;



extern tCmd const *CMD_Y7025CmdGet(void);
extern uint16_t CMD_Y7025CmdNumGet(void);
#ifdef __cplusplus
}
#endif
#endif
