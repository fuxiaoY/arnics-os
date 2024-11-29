
#ifndef __MCTLIST_H__
#define __MCTLIST_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "../dataPlat/mctDefinition.h"
/***************************************************************
*Command API
***************************************************************/
#define CMD_ECHO_CLOSE              0
#define CMD_MODEM_INFO              (1 + CMD_ECHO_CLOSE)
#define CMD_SIM_INFO                (1 + CMD_MODEM_INFO)
#define CMD_FACTORY_INFO            (1 + CMD_SIM_INFO)
#define CMD_PINCODE_SET             (1 + CMD_FACTORY_INFO)
#define CMD_MODEM_CONFIG            (1 + CMD_PINCODE_SET)
#define CMD_MODEM_CHECK             (1 + CMD_MODEM_CONFIG)
#define CMD_MODEM_SIGNAL            (1 + CMD_MODEM_CHECK)
#define CMD_REGISTER                (1 + CMD_MODEM_SIGNAL)
#define CMD_CLOSE_ERGISTER          (1 + CMD_REGISTER)
#define CMD_OPEN_ERGISTER           (1 + CMD_CLOSE_ERGISTER)
#define CMD_CELL_MAIN_INFO          (1 + CMD_OPEN_ERGISTER)
#define CMD_CELL_ADJACENT_INFO      (1 + CMD_CELL_MAIN_INFO)
#define CMD_PDP_INQUIRE             (1 + CMD_CELL_ADJACENT_INFO)
#define CMD_PDP_OPEN                (1 + CMD_PDP_INQUIRE)
#define CMD_PDP_CLOSE               (1 + CMD_PDP_OPEN)
#define CMD_IPADDR_GET              (1 + CMD_PDP_CLOSE)
#define CMD_LISTEN_INQUIRE          (1 + CMD_IPADDR_GET)
#define CMD_LISTEN_CLOSE            (1 + CMD_LISTEN_INQUIRE)
#define CMD_LISTEN_OPEN             (1 + CMD_LISTEN_CLOSE)
#define CMD_CONN_INQUIRE            (1 + CMD_LISTEN_OPEN)
#define CMD_FREE_CONN_INQUIRE       (1 + CMD_CONN_INQUIRE)
#define CMD_CONN_OPEN               (1 + CMD_FREE_CONN_INQUIRE)
#define CMD_CONN_CLOSE              (1 + CMD_CONN_OPEN)
#define CMD_DATA_REC                (1 + CMD_CONN_CLOSE)
#define CMD_DATA_SEND               (1 + CMD_DATA_REC)
#define CMD_LAST_GASP               (1 + CMD_DATA_SEND)
#define CMD_LAST_GASP_CLIENT        (1 + CMD_LAST_GASP)
#define CMD_FOTA_CFG                (1 + CMD_LAST_GASP_CLIENT)
#define CMD_FOTA_DOWNLOAD_START     (1 + CMD_FOTA_CFG)
#define CMD_FOTA_DOWNLOAD           (1 + CMD_FOTA_DOWNLOAD_START)
#define CMD_FOTA_ACT                (1 + CMD_FOTA_DOWNLOAD)
#define CMD_POWER_OFF               (1 + CMD_FOTA_ACT)
#define CMD_NTP_SYNC                (1 + CMD_POWER_OFF)
#define CMD_MODEM_CLOCK_GET         (1 + CMD_NTP_SYNC)
#define CMD_MODEM_PULL_OUT          (1 + CMD_MODEM_CLOCK_GET)
#define CMD_MODEM_PULL_FILTER       (1 + CMD_MODEM_PULL_OUT)
#define CMD_DATA_SEND_ACK           (1 + CMD_MODEM_PULL_FILTER)
#define CMD_MODEM_REBOOT            (1 + CMD_DATA_SEND_ACK)
#define CMD_AUTO_REBOOT_CHECK_SET   (1 + CMD_MODEM_REBOOT)

#define CMD_AUTO_CONFIG             (1 + CMD_AUTO_REBOOT_CHECK_SET)
#define CMD_NETWORK_CHECK           (1 + CMD_AUTO_CONFIG )
#define CMD_COLDSTARTUP_CHECK       (1 + CMD_NETWORK_CHECK )
#define CMD_WORKLOCKSET             (1 + CMD_COLDSTARTUP_CHECK )

#define CMD_MQTTFLOW                (1 + CMD_WORKLOCKSET )
#define CMD_MQTTPUBLISH             (1 + CMD_MQTTFLOW )
#define CMD_MQTTREV                 (1 + CMD_MQTTPUBLISH )
#define CMD_MQTTSTOP                 (1 + CMD_MQTTREV )
#define CMD_BOOTUPCLOCK             (1 + CMD_MQTTSTOP )
#define CMD_USERCALL                (1 + CMD_BOOTUPCLOCK )
#define CMD_MQTTREVPUSH             (1 + CMD_USERCALL )
#define CMD_STICKFRAME              (1 + CMD_MQTTREVPUSH )
#define CMD_REV_FLOW                (1 + CMD_STICKFRAME )
#define CMD_POWEROFF                (1 + CMD_REV_FLOW )

#define CMD_MAX                     (1 + CMD_POWEROFF)

extern uint16_t mctModemLisNumGet(void);
extern const tModemList *mctModemListGet(void);

#ifdef __cplusplus
}
#endif

#endif
