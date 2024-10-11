#ifndef GLOBAL_DEF_H
#define GLOBAL_DEF_H
#include "typedef.h"
#include "ProjDefine.h"
#include "crc16.h"
#include "interface_def.h"
/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------系统数据平台------------------------------------*/

/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------数据库平台-------------------------------------*/
#define DB_RECORD_MASK(x)    ((uint32)0x00000001 << (x))
#define DB_NA            ((uint32)0x00000000)

typedef enum
{
  DB_REBOOT = 0,
  DB_SYSTEM, //系统信息
  DB_BUSINESS,   //主业务信息
  DB_CONSOLE,      //消息日志
  DB_UPLOAD,      //上传信息
  DB_WANMODME,    //外网模块信息
  DB_LCMODME,     //北斗模块信息

}DB_RECORD_TYPE;

typedef enum
{
  BOOTUP_TYPE_INFO = 0,
  LOG_TYPE_INFO,

}tsdb_type;
typedef struct 
{
    RTC_TimeDateTypeDef   time;
    uint8_t  rst_info;

} tsdb_rcc_info;

typedef struct 
{
    RTC_TimeDateTypeDef   time;
    uint16_t len;
    uint8_t  rst_info[100];

} tsdb_print_info;

typedef struct
{
    tsdb_type type;
    uint32_t level;
    union
    {
        tsdb_rcc_info rcc;
        tsdb_print_info print;
    } data; // 使用 union 来存储不同类型的结构体

}tsdb_info;
/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------模块供电----------------------------------------*/
// 通用模块供电状态定义 高电平关闭，低电平打开
#define POWER_ON  0 //低电平ON
#define POWER_OFF 1 //高电平OFF


/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------消息中心----------------------------------------*/
typedef enum//Open：常开 close:关闭
{
    Close = 0,
    Open = 1,

}GNSS_check_ACTION;

typedef enum
{
    MUSICBUZZER = 1,
    ONWAYBUZZER,
    CHARGEBUZZER,
    DEFENDBUZZER,
}buzzer_action;

typedef struct
{
    BOOL need_battery_check;  // 电量检测事件
    BOOL need_posture_check;  // 姿态检测事件
    BOOL need_sensorIR_check; // 红外检测事件
    BOOL need_RFID_check; // RFID检测事件
    BOOL need_GNSS_check; // 北斗检测事件
    BOOL need_buzzer_action; // 蜂鸣器事件
    buzzer_action buzzer_action; // 蜂鸣器动作
    GNSS_check_ACTION GNSS_check_action;//北斗检测开关

    time_t battery_check_Delay_MS;//电量检测事件延时 0为不延时
    time_t posture_check_Delay_MS;//姿态检测事件延时
    time_t sensorIR_check_Delay_MS;//红外检测事件延时
    time_t RFID_check_Delay_MS;//红外检测事件延时

} messeage_need_sample_t; // 需要采样消息


/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------媒体中心----------------------------------------*/

// 组网状态定义
typedef enum
{
    NETWORK_ALWAYS_ON,     // 常开组网状态
    NETWORK_EVENT_DRIVEN,   // 消息驱动型组网状态 推送完消息则关机
    NETWORK_SELFDEF_DRIVEN  // 灵活组网状态 30s内没震动则关机
} NetworkStatus_t;

/*---------------------------------------------------------------------------------------*/

/*----------------------------------------休眠中心----------------------------------------*/


/*---------------------------------------------------------------------------------------*/

/*----------------------------------------决策层----------------------------------------*/

// 设备运行状态
enum Enum_WORKSTATE
{
  WORKSTAT_INIT = 0,   // 上电初始化
};


/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------配置数据----------------------------------------*/
//设备运行参数集，存放于片上Flash，同步一份到内存中常驻
struct ST_RTConfig
{
  uint32_t SaveTs;					//保存时间戳，同时也用于判定Flash上保存的参数集是否有效

  //数据库配置
  uint32_t FlashDBrecordMask;
  uint32_t FlashDBrecordMaxNum;
  uint16_t FlashDBForceCleanEveryDay;  // 每多少天强制清空数据库，0表示不强制清空


/*
* 设备动态属性
*/
  float VolCalibrate;	//电压校准值，单位为mV



  //校验位
  uint16_t crc;    
  //保存到片上Flash
  //返回值：TRUE=写入成功，FALSE=写入失败
  BOOL Save(void);

  //从片上Flash读出
  //返回值：TRUE=读出成功，FALSE=读取失败
  BOOL Load(void);

  //全部恢复缺省设置
  //参数1：是否需要将Flash上保存的记录也同步恢复成缺省值
  //返回值：TRUE=恢复成功，FLASE=恢复失败（内存常驻配置已恢复，但写入Flash失败导致记录不同步），若SyncSave为FLASE，则永远不会失败
  BOOL Reset(BOOL SyncSave = FALSE);


  uint16_t CalculateCRC(void);
};
#define ST_RTCONFIG_SIZE	sizeof(ST_RTConfig)
/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------运行数据----------------------------------------*/
//设备运行状态记录集，常驻内存中，同步存放一份到片上Flash，复位重启后读回来作为缺省状态
struct ST_RTState
{
  uint32_t SaveTs;				//保存时间戳，同时也用于判定Flash上保存的参数集是否有效

  Enum_WORKSTATE WorkStat;			//当前运行状态，  取值范围0~4，详情参考Enum_WORKSTATE定义
  Enum_WORKSTATE PreWorkStat;		//上次的运行状态，取值范围0~4，详情参考Enum_WORKSTATE定义


  //校验位
  uint16_t crc;    

  //保存到片上Flash（仅当重要字段发生变更时才保存）
  //返回值：TRUE=写入成功，FALSE=写入失败
  BOOL Save(void);

  //保存到片上Flash（强制保存，即使要字段未发生变更）
  //返回值：TRUE=写入成功，FALSE=写入失败
  BOOL ForceSave(void);

  //从片上Flash读出
  //返回值：TRUE=读出成功，FALSE=读取失败
  BOOL Load(void);

  //全部恢复缺省设置
  //参数1：是否需要将Flash上保存的记录也同步恢复成缺省值
  //返回值：TRUE=恢复成功，FLASE=恢复失败（内存常驻配置已恢复，但写入Flash失败导致记录不同步），若SyncSave为FLASE，则永远不会失败
  BOOL Reset(BOOL SyncSave = FALSE);



  uint16_t CalculateCRC(void);

};
#define ST_RTSTATE_SIZE		sizeof(ST_RTState)
/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------上报数据----------------------------------------*/
// 状态上报
struct ST_ShoeUpload
{
    char DevNum[20]; // 设备编号
    uint32_t boradType;
    char Parent[20];        // 父级设备编号
    uint16_t HomingPos;      // 归位序号
    char CollectDT[20];     // 采集时间戳
    char UploadDT[20];      // 上报时间戳
    uint32_t RunTime;       // 运行时长
    uint8_t SignalStrength; // 公网信号强度
    uint8_t BatteryPer;     // 电池电量百分比
    BOOL Charging;          // 充电状态，FALSE=未充电，TRUE=正在充电
                            // onTrack onWheel onPress
    uint8_t DefindBit;      // 防溜传感标志位，详见Enum_DefendBit定义
    uint16_t vBat; // vbat 电池电压
    uint8_t onTrack;
    uint8_t onWheel;
    uint8_t onPress;
    uint8_t charging;
    uint8_t latest;
    uint16_t TrackDist; // 轨距，单位为mm，最大数值受测距探头自身特性限制，例如VL53L0只能测量2~2300mm
    uint16_t WheelDist; // 轮距，单位为mm，最大数值受测距探头自身特性限制，例如VL53L0只能测量2~2300mm
    uint8_t workStat;   // 当前处于成什么状态，取值范围0~4，详情参考Enum_WORKSTATE定义
    uint8_t Event;      // 事件类型 服务器端只有0、1、2、3四种值，对应正常、松动、压鞋、失联，彼此互斥不会同时发生
    float GPSLon;       // 卫星定位经度
    float GPSLat;       // 卫星定位纬度
    float LBSLon;       // 基站定位经度
    float LBSLat;       // 基站定位纬度
    char VerCode[10]; // 版本编号，分别对应版本号的4个数字段
    char VerDate[16];   // 版本日期，精确到日
    float AngleR[3];    // 姿态角采样值
    float Accel[3];     // 加速计采样值

    // 信号强度，上报时管理
    void *CaptureRSSI(void);
    // 上报时间戳 需要上报时管理
    void *CaptureUploadTime(void);
    // 捕捉当前数据快照
    void *CaptureCurrentValue(void);
    void afterUploadAction();
};

/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------消息中心----------------------------------------*/

typedef struct {
    messeage_need_sample_t message_need_sample; //需要采样信息
    time_t ID_Ts;    //消息ID，可用于检查区分消息传送 
    char buf[20];  // 可以根据需要添加更多的字段
    int length;     // 数据长度
} Message_t;

/*---------------------------------------------------------------------------------------*/

/*----------------------------------------休眠中心----------------------------------------*/

typedef struct {
    time_t ID_Ts;    //消息ID，可用于检查区分消息传送
    uint32_t sleepTime_ms;
} SleepMessage_t;

/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------媒体中心----------------------------------------*/
typedef struct {
    messeage_need_sample_t message_need_sample; //需要采样信息
    time_t ID_Ts;    //消息ID，可用于检查区分消息传送
    ST_ShoeUpload UploadData; //上报状态  
    char buf[20];  // 可以根据需要添加更多的字段
    int length;     // 数据长度
} MediaMessage_t;


#endif

