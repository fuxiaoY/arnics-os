#ifndef __MCTSTRUCT_H__
#define __MCTSTRUCT_H__

#ifdef __cplusplus
extern "C" {
#endif
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
}uWanClock;

typedef struct
{
    uWanClock clock;
    int       zone;
}tWanClock;

typedef struct 
{
    uint8_t socket_id;
    uint8_t return_code;
}mqttconnnack_t;

typedef struct 
{
char* Topic;
int QOS;
uint32_t PublishLen;
char* PublishData;
}MqttPulish_t;



#ifdef __cplusplus
}
#endif
#endif

