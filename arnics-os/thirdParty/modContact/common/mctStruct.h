#ifndef __MCTSTRUCT_H__
#define __MCTSTRUCT_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#define DATA_TIME_LEN  12
typedef union
{
    struct
    {
        uint8_t year_h;
        uint8_t year_l;
        uint8_t month;
        uint8_t day;
        uint8_t week;
        uint8_t hour;
        uint8_t min;
        uint8_t sec;
        uint8_t hund;
        uint8_t dev_h;
        uint8_t dev_l;
        uint8_t status;
    };
    uint8_t str[DATA_TIME_LEN];
}wanClock_u;

typedef struct
{
    wanClock_u clock;
    int       zone;
}wanClock_t;

typedef struct 
{
    char*    Topic;
    int      QOS;
    uint32_t PublishLen;
    char*    PublishData;
}mqttPulish_t;

typedef enum
{
    registerNone        = (uint8_t)0,
    registerHomeNet,
    resistering,
    registerDenied,
    registerUnknow,
    registerRoaming, 
    registerMax,
}regStatus_e;

typedef struct
{
  uint8_t SignalStrength; // 公网信号强度
  regStatus_e REGstatus;  // 组网进度
}networkPara_t;

typedef struct 
{
    char* ip;
    uint16_t port;
}httpURL;

typedef struct
{
    uint16_t BlockNum;
    uint32_t fileLen;
    char UpdatePath[128];		  //远程刷机绝对地址
    uint8_t *fileData;
} http_t;

typedef struct
{
    char      *ip;                // http地址
    uint16_t  port;               // http端口号
    char      *file_path;         // http文件地址
    uint32_t  file_path_len;      // http文件地址长度 
    uint32_t  block_len;          // 单包获取长度


    uint16_t  block_num;          // 传输序号
    uint8_t   *p_file_data;       // 单包数据指针
    uint32_t  file_len;           // 实际获取长度


}httpAction_t;
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
#define CMD_MQTTSTOP                (1 + CMD_MQTTREV )
#define CMD_BOOTUPCLOCK             (1 + CMD_MQTTSTOP )
#define CMD_USERCALL                (1 + CMD_BOOTUPCLOCK )
#define CMD_MQTTREVPUSH             (1 + CMD_USERCALL )
#define CMD_STICKFRAME              (1 + CMD_MQTTREVPUSH )
#define CMD_REV_FLOW                (1 + CMD_STICKFRAME )
#define CMD_POWEROFF                (1 + CMD_REV_FLOW )

#define CMD_HTTPCONNECT             (1 + CMD_POWEROFF )
#define CMD_HTTPGET                 (1 + CMD_HTTPCONNECT )
#define CMD_HTTPCLOSE               (1 + CMD_HTTPGET )



#define CMD_MODBUS_01              (1 + CMD_HTTPCLOSE)
#define CMD_MODBUS_03              (1 + CMD_MODBUS_01)
#define CMD_MODBUS_05              (1 + CMD_MODBUS_03)
#define CMD_MODBUS_06              (1 + CMD_MODBUS_05)

#define CMD_MODBUS_SLAVE           (1 + CMD_MODBUS_06)
#define CMD_MAX                    (1 + CMD_MODBUS_SLAVE)

#ifdef __cplusplus
}
#endif
#endif

