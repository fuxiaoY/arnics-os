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
#include <string.h>
#include "../../Common/mctAdapter.h"

#define CMD_Y7025_ID_BASE                           (uint16)0

#define CMD_Y7025_IMEI_GET                          (uint16)(CMD_Y7025_ID_BASE +  1)
#define CMD_Y7025_IMSI_GET                          (uint16)(CMD_Y7025_ID_BASE +  2)
#define CMD_Y7025_VERSION_GET                       (uint16)(CMD_Y7025_ID_BASE +  3)
#define CMD_Y7025_ICCID_GET                         (uint16)(CMD_Y7025_ID_BASE +  4)
#define CMD_Y7025_SYNC_CLOCK                        (uint16)(CMD_Y7025_ID_BASE +  5)
#define CMD_Y7025_CLOCK_GET                         (uint16)(CMD_Y7025_ID_BASE +  6)
#define CMD_Y7025_APP_CFG_APN                       (uint16)(CMD_Y7025_ID_BASE +  7)
#define CMD_Y7025_APP_CFG_USER                      (uint16)(CMD_Y7025_ID_BASE +  8)
#define CMD_Y7025_APP_CFG_PASS                      (uint16)(CMD_Y7025_ID_BASE +  9)
#define CMD_Y7025_ENABLE_APP_FUN                    (uint16)(CMD_Y7025_ID_BASE + 10)
#define CMD_Y7025_DISBALE_APP_FUN                   (uint16)(CMD_Y7025_ID_BASE + 11)
#define CMD_Y7025_INQUIRE_APP_FUN                   (uint16)(CMD_Y7025_ID_BASE + 12)
#define CMD_Y7025_NTP_SERVER                        (uint16)(CMD_Y7025_ID_BASE + 13)
#define CMD_Y7025_NTP_ACTION                        (uint16)(CMD_Y7025_ID_BASE + 14)
#define CMD_Y7025_PINCODE_INQUIRE                   (uint16)(CMD_Y7025_ID_BASE + 15)
#define CMD_Y7025_PINCODE_SET                       (uint16)(CMD_Y7025_ID_BASE + 16)
#define CMD_Y7025_FILE_INIT                         (uint16)(CMD_Y7025_ID_BASE + 17)
#define CMD_Y7025_FILE_READ_READY                   (uint16)(CMD_Y7025_ID_BASE + 18)
#define CMD_Y7025_FILE_READ                         (uint16)(CMD_Y7025_ID_BASE + 19)
#define CMD_Y7025_FILE_SIZE                         (uint16)(CMD_Y7025_ID_BASE + 20)
#define CMD_Y7025_FILE_DELETE                       (uint16)(CMD_Y7025_ID_BASE + 21)
#define CMD_Y7025_FILE_WRITE_READY                  (uint16)(CMD_Y7025_ID_BASE + 22)
#define CMD_Y7025_FILE_WRITE                        (uint16)(CMD_Y7025_ID_BASE + 23)
#define CMD_Y7025_FILE_FREE                         (uint16)(CMD_Y7025_ID_BASE + 24)
#define CMD_Y7025_SYS_CHOICE                        (uint16)(CMD_Y7025_ID_BASE + 25)
#define CMD_Y7025_NB_ENABLE                         (uint16)(CMD_Y7025_ID_BASE + 26)
#define CMD_Y7025_NB_CATM_CHOICE                    (uint16)(CMD_Y7025_ID_BASE + 27)
#define CMD_Y7025_ENABLE_MUTI_CHANNEL               (uint16)(CMD_Y7025_ID_BASE + 28)
#define CMD_Y7025_MUTI_CHANNEL_INQUIRE              (uint16)(CMD_Y7025_ID_BASE + 29)
#define CMD_Y7025_DATA_FORMAT_SET                   (uint16)(CMD_Y7025_ID_BASE + 30)
#define CMD_Y7025_OPERATOR_INQUIRE                  (uint16)(CMD_Y7025_ID_BASE + 31)
#define CMD_Y7025_SYSTEM_GET                        (uint16)(CMD_Y7025_ID_BASE + 32)

#define CMD_Y7025_DATA_REGISTER                     (uint16)(CMD_Y7025_ID_BASE + 34)
#define CMD_Y7025_UE_INFO_GET                       (uint16)(CMD_Y7025_ID_BASE + 35)
#define CMD_Y7025_ADJACANT_INFO_GET                 (uint16)(CMD_Y7025_ID_BASE + 36)
#define CMD_Y7025_PDP_INQUIRE                       (uint16)(CMD_Y7025_ID_BASE + 37)
#define CMD_Y7025_AUTH_SET                          (uint16)(CMD_Y7025_ID_BASE + 38)
#define CMD_Y7025_APN_SET                           (uint16)(CMD_Y7025_ID_BASE + 39)
#define CMD_Y7025_PDP_OPEN                          (uint16)(CMD_Y7025_ID_BASE + 40)
#define CMD_Y7025_PDP_CLOSE                         (uint16)(CMD_Y7025_ID_BASE + 41)
#define CMD_Y7025_IP_GET                            (uint16)(CMD_Y7025_ID_BASE + 42)
#define CMD_Y7025_TCP_LISTEN_INQUIRE                (uint16)(CMD_Y7025_ID_BASE + 43)
#define CMD_Y7025_TCP_LISTEN_CLOSE                  (uint16)(CMD_Y7025_ID_BASE + 44)
#define CMD_Y7025_TCP_LISTEN_OPEN                   (uint16)(CMD_Y7025_ID_BASE + 45)
#define CMD_Y7025_CONN_INQUIRE                      (uint16)(CMD_Y7025_ID_BASE + 46)
#define CMD_Y7025_CONN_OPEN                         (uint16)(CMD_Y7025_ID_BASE + 47)
#define CMD_Y7025_CONN_CLOSE                        (uint16)(CMD_Y7025_ID_BASE + 48)
#define CMD_Y7025_DATALEN_INQUIRE                   (uint16)(CMD_Y7025_ID_BASE + 49)
#define CMD_Y7025_DATA_REC                          (uint16)(CMD_Y7025_ID_BASE + 50)
#define CMD_Y7025_DATA_SEND_READY                   (uint16)(CMD_Y7025_ID_BASE + 51)
#define CMD_Y7025_DATA_SEND                         (uint16)(CMD_Y7025_ID_BASE + 52)
#define CMD_Y7025_LAST_GASP                         (uint16)(CMD_Y7025_ID_BASE + 53)
#define CMD_Y7025_EDRX_CLOSE                        (uint16)(CMD_Y7025_ID_BASE + 54)
#define CMD_Y7025_BAND_SET                          (uint16)(CMD_Y7025_ID_BASE + 55)
#define CMD_Y7025_PSM_CLOSE                         (uint16)(CMD_Y7025_ID_BASE + 56)
#define CMD_Y7025_ENABLE_CFUN                       (uint16)(CMD_Y7025_ID_BASE + 57)
#define CMD_Y7025_DISABLE_CFUN                      (uint16)(CMD_Y7025_ID_BASE + 58)
#define CMD_Y7025_SUB_VERSION_GET                   (uint16)(CMD_Y7025_ID_BASE + 59)
#define CMD_Y7025_POWER_OFF                         (uint16)(CMD_Y7025_ID_BASE + 60)

#define CMD_Y7025_WORKLOCK                          (uint16)(CMD_Y7025_ID_BASE + 61)
#define CMD_Y7025_CPSMS                          (uint16)(CMD_Y7025_ID_BASE + 62)
#define CMD_Y7025_ECHO_OFF                          (uint16)(CMD_Y7025_ID_BASE +  0)
#define CMD_Y7025_CEREG                          (uint16)(CMD_Y7025_ID_BASE +  63)
#define CMD_Y7025_SIGNAL_GET                        (uint16)(CMD_Y7025_ID_BASE + 33)
#define CMD_Y7025_NCCID                          (uint16)(CMD_Y7025_ID_BASE +  64)
#define CMD_Y7025_CGMM                           (uint16)(CMD_Y7025_ID_BASE +  65)
#define CMD_Y7025_CGMR                           (uint16)(CMD_Y7025_ID_BASE +  66)

#define CMD_Y7025_MQTTNEW                    (uint16)(CMD_Y7025_ID_BASE +  67)
#define CMD_Y7025_MQTTCON                    (uint16)(CMD_Y7025_ID_BASE +  68)
#define CMD_Y7025_MQTTSUB                    (uint16)(CMD_Y7025_ID_BASE +  69)
#define CMD_Y7025_MQTTPUB                    (uint16)(CMD_Y7025_ID_BASE +  70)
#define CMD_Y7025_BOOTZTZEU                   (uint16)(CMD_Y7025_ID_BASE +  71)
#define CMD_Y7025_MQTTREV                     (uint16)(CMD_Y7025_BOOTZTZEU +  72)

#define CMD_Y7025Execute(inst,payloadlist,is_expected,id, para)                  CMD_Execute(inst,payloadlist,is_expected,id,CMD_Y7025CmdGet(), CMD_Y7025CmdNumGet(), para)
#define SOCKET_NUM                                  8

typedef struct
{
    int8            rssi;
    int8            ber;
}tsignalQuality;

extern tCmd const *CMD_Y7025CmdGet(void);
extern uint16 CMD_Y7025CmdNumGet(void);

#endif
