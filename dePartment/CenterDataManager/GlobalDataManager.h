#ifndef _GLOBAL_DATA_MANAGER_H_
#define _GLOBAL_DATA_MANAGER_H_
#include "ulog_def.h"
#include "typedef.h"
#include "GlobalData.h"
#include "s2j.h"
#include "FreeRTOS.h"

// 定义系统参数的起始位置
#define Unity_System_START  0

// 定义系统参数的宏
#define Unity_System_DevNum               (1 + Unity_System_START)    // 设备编号
#define Unity_System_DevDate              (1 + Unity_System_DevNum)   // 设备日期
#define Unity_System_ALLOWSLEEP           (1 + Unity_System_DevDate)  // 是否允许休眠标志位
#define Unity_System_MediaWantSleep       (1 + Unity_System_ALLOWSLEEP)  // 媒体中心休眠申请
#define Unity_System_eventosWantSleep     (1 + Unity_System_MediaWantSleep)  // 事件OS休眠申请
#define Unity_System_ConsoleWantSleep     (1 + Unity_System_eventosWantSleep)  // 控制台休眠申请
#define Unity_System_StartGuardWantSleep  (1 + Unity_System_ConsoleWantSleep)  // 督查中心休眠申请
#define Unity_System_isFirstPowerOn       (1 + Unity_System_StartGuardWantSleep)  // 系统是否第一次开机
#define Unity_System_systemResetFlag      (1 + Unity_System_isFirstPowerOn)  // 系统重置标志
#define Unity_System_SHELL_ACTIVE         (1 + Unity_System_systemResetFlag)  // 系统是否处于shell状态
#define Unity_System_FREERTOS_INIT_ISDONE (1 + Unity_System_SHELL_ACTIVE)  // FreeRTOS是否初始化完毕
#define Unity_System_EVENTOS_INIT_ISDONE  (1 + Unity_System_FREERTOS_INIT_ISDONE)  // EVENTOS是否初始化完毕
#define Unity_System_g_InitTs             (1 + Unity_System_EVENTOS_INIT_ISDONE)  // 系统启动时的时间戳
#define Unity_System_usr_systick          (1 + Unity_System_g_InitTs)  // 用户系统滴答计数
#define Unity_System_Exti_Counter         (1 + Unity_System_usr_systick)  // EXTI计数器
#define Unity_System_debug_uart_Counter   (1 + Unity_System_Exti_Counter)  // 调试UART计数器
#define Unity_System_systemResetFlagNun   (1 + Unity_System_debug_uart_Counter)  // 系统重置标志判断
#define Unity_System_VirtualEnvironment   (1 + Unity_System_systemResetFlagNun)  // 虚拟环境标志
#define Unity_System_systemAllowSleep     (1 + Unity_System_VirtualEnvironment)  // 是否允许休眠     
#define Unity_System_TSDBForceCleanCount  (1 + Unity_System_systemAllowSleep)  // TSDB强制清理计数

/*---------------------------------------------------------------------------------------------*/
/*-Upload-------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------*/
#define Unity_Upload_DevNum                     0                      // 设备编号
#define Unity_Upload_BoradType                  (1 + Unity_Upload_DevNum)    // 主板类型
#define Unity_Upload_Parent                     (1 + Unity_Upload_BoradType) // 父级设备编号
#define Unity_Upload_HomingPos                  (1 + Unity_Upload_Parent)    // 归位序号
#define Unity_Upload_CollectTs                  (1 + Unity_Upload_HomingPos) // 采集时间戳
#define Unity_Upload_UploadTs                   (1 + Unity_Upload_CollectTs) // 上报时间戳
#define Unity_Upload_RunTime                    (1 + Unity_Upload_UploadTs)  // 运行时长
#define Unity_Upload_SignalStrength             (1 + Unity_Upload_RunTime)   // 公网信号强度
#define Unity_Upload_BatteryPer                 (1 + Unity_Upload_SignalStrength) // 电池电量百分比
#define Unity_Upload_Charging                   (1 + Unity_Upload_BatteryPer)  // 充电状态
#define Unity_Upload_DefindBit                  (1 + Unity_Upload_Charging)   // 防溜传感标志位
#define Unity_Upload_vBat                       (1 + Unity_Upload_DefindBit)  // 电池电压
#define Unity_Upload_TrackDist                  (1 + Unity_Upload_vBat)       // 轨距
#define Unity_Upload_WheelDist                  (1 + Unity_Upload_TrackDist)  // 轮距
#define Unity_Upload_onTrack                    (1 + Unity_Upload_WheelDist)  // 贴轮
#define Unity_Upload_onWheel                    (1 + Unity_Upload_onTrack)    // 贴轨
#define Unity_Upload_onPress                    (1 + Unity_Upload_onWheel)    // 压鞋
#define Unity_Upload_charging                   (1 + Unity_Upload_onPress)    // 是否在充电
#define Unity_Upload_lastest                    (1 + Unity_Upload_charging)   // 是否是新消息
#define Unity_Upload_WorkStat                   (1 + Unity_Upload_lastest)    // 当前运行状态
#define Unity_Upload_Event                      (1 + Unity_Upload_WorkStat)   // 事件类型
#define Unity_Upload_GPSLon                     (1 + Unity_Upload_Event)      // 卫星定位经度
#define Unity_Upload_GPSLat                     (1 + Unity_Upload_GPSLon)     // 卫星定位纬度
#define Unity_Upload_LBSLon                     (1 + Unity_Upload_GPSLat)     // 基站定位经度
#define Unity_Upload_LBSLat                     (1 + Unity_Upload_LBSLon)     // 基站定位纬度
#define Unity_Upload_VerCode                    (1 + Unity_Upload_LBSLat)     // 版本编号
#define Unity_Upload_VerDate                    (1 + Unity_Upload_VerCode)    // 版本日期
#define Unity_Upload_AngleR                     (1 + Unity_Upload_VerDate)    // 姿态角采样值
#define Unity_Upload_Accel                      (1 + Unity_Upload_AngleR)     // 加速计采样值
#define Unity_Upload_MAX_ID                     (1 + Unity_Upload_Accel)      // 最大ID
/*---------------------------------------------------------------------------------------------*/
/*-RTStat-------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------*/
#define Unity_RTStat_START                     0                      // 起始ID
#define Unity_RTStat_SaveTs                    (1 + Unity_RTStat_START)    // 保存时间戳，同时也用于判定Flash上保存的参数集是否有效
#define Unity_RTStat_WorkStat                  (1 + Unity_RTStat_SaveTs)   // 当前运行状态，取值范围0~4，详情参考Enum_WORKSTATE定义
#define Unity_RTStat_PreWorkStat               (1 + Unity_RTStat_WorkStat) // 上次的运行状态，取值范围0~4，详情参考Enum_WORKSTATE定义
// 最新电压采样
#define Unity_RTStat_Battery                   (1 + Unity_RTStat_PreWorkStat) // 电池电压，单位为mV
#define Unity_RTStat_BatteryPercentage         (1 + Unity_RTStat_Battery)     // 电池电压百分比，单位为%
#define Unity_RTStat_Charging                  (1 + Unity_RTStat_BatteryPercentage) // 充电电压，单位为mV
// 最新传感器采样
#define Unity_RTStat_TrackDist                 (1 + Unity_RTStat_Charging)    // 轨距，单位为mm，最大数值受测距探头自身特性限制，例如VL53L0只能测量2~2300mm
#define Unity_RTStat_WheelDist                 (1 + Unity_RTStat_TrackDist)   // 轮距，单位为mm，最大数值受测距探头自身特性限制，例如VL53L0只能测量2~2300mm
#define Unity_RTStat_Temprate                  (1 + Unity_RTStat_WheelDist)   // 环境温度，单位为℃
// 最新六轴姿态
#define Unity_RTStat_GryoR_0                   (1 + Unity_RTStat_Temprate)    // 陀螺仪采样值，实时更新
#define Unity_RTStat_GryoR_1                   (1 + Unity_RTStat_GryoR_0)     // 陀螺仪采样值，实时更新
#define Unity_RTStat_GryoR_2                   (1 + Unity_RTStat_GryoR_1)     // 陀螺仪采样值，实时更新
#define Unity_RTStat_AccelR_0                  (1 + Unity_RTStat_GryoR_2)     // 加速器采样值，实时更新
#define Unity_RTStat_AccelR_1                  (1 + Unity_RTStat_AccelR_0)    // 加速器采样值，实时更新
#define Unity_RTStat_AccelR_2                  (1 + Unity_RTStat_AccelR_1)    // 加速器采样值，实时更新
// 全向角
#define Unity_RTStat_AngleR_0                  (1 + Unity_RTStat_AccelR_2)    // 全向角采样值，实时更新
#define Unity_RTStat_AngleR_1                  (1 + Unity_RTStat_AngleR_0)    // 全向角采样值，实时更新
#define Unity_RTStat_AngleR_2                  (1 + Unity_RTStat_AngleR_1)    // 全向角采样值，实时更新
// 布防基准记录
#define Unity_RTStat_GryoB_0                   (1 + Unity_RTStat_AngleR_2)    // 陀螺仪采样值，布防基准记录
#define Unity_RTStat_GryoB_1                   (1 + Unity_RTStat_GryoB_0)     // 陀螺仪采样值，布防基准记录
#define Unity_RTStat_GryoB_2                   (1 + Unity_RTStat_GryoB_1)     // 陀螺仪采样值，布防基准记录
#define Unity_RTStat_AccelB_0                  (1 + Unity_RTStat_GryoB_2)     // 加速器采样值，布防基准记录
#define Unity_RTStat_AccelB_1                  (1 + Unity_RTStat_AccelB_0)    // 加速器采样值，布防基准记录
#define Unity_RTStat_AccelB_2                  (1 + Unity_RTStat_AccelB_1)    // 加速器采样值，布防基准记录
#define Unity_RTStat_AngleB_0                  (1 + Unity_RTStat_AccelB_2)    // 全向角采样值，布防基准记录
#define Unity_RTStat_AngleB_1                  (1 + Unity_RTStat_AngleB_0)    // 全向角采样值，布防基准记录
#define Unity_RTStat_AngleB_2                  (1 + Unity_RTStat_AngleB_1)    // 全向角采样值，布防基准记录
// 最新定位
#define Unity_RTStat_LBSLon                    (1 + Unity_RTStat_AngleB_2)    // 基站定位经度
#define Unity_RTStat_LBSLat                    (1 + Unity_RTStat_LBSLon)      // 基站定位纬度
#define Unity_RTStat_GPSLon                    (1 + Unity_RTStat_LBSLat)      // 卫星定位经度
#define Unity_RTStat_GPSLat                    (1 + Unity_RTStat_GPSLon)      // 卫星定位纬度
// 北斗模块开启状态
#define Unity_RTStat_GnssState                 (1 + Unity_RTStat_GPSLat)      // 北斗模块开启状态  开启TRUE/关闭FALSE
#define Unity_RTStat_GnssRefreshID             (1 + Unity_RTStat_GnssState)   // 北斗定位成功次数，每次定位则刷新，可溢出归零
// RFID状态
#define Unity_RTStat_RFID_CHECK                (1 + Unity_RTStat_GnssRefreshID) // 是否找到RFID
// 整合传感状态值
#define Unity_RTStat_HallVals                  (1 + Unity_RTStat_RFID_CHECK)  // 整合传感状态值，由传感器自行管理，不可改动，bit0、1、2、3、4依次为贴轨、贴轮、压鞋、轮距松动、轨距松动、姿态水平，0未达成，1已达成
// 异常事件位
#define Unity_RTStat_Event                     (1 + Unity_RTStat_HallVals)    // 异常事件位
#define Unity_RTStat_GnssLat                   (1 + Unity_RTStat_Event)       // 卫星定位纬度
#define Unity_RTStat_GnssLon                   (1 + Unity_RTStat_GnssLat)     // 卫星定位经度
#define Unity_RTStat_GryoR                     (1 + Unity_RTStat_GnssLon)     // 陀螺仪采样值，实时更新
#define Unity_RTStat_AngleR                    (1 + Unity_RTStat_GryoR)       // 全向角采样值，实时更新
#define Unity_RTStat_MAX_ID                    (1 + Unity_RTStat_AngleR)      // 最大ID
/*---------------------------------------------------------------------------------------------*/
/*-RTCfg------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------*/
#define Unity_RTCfg_START                          0

// 保存时间戳
#define Unity_RTCfg_SaveTs                         (1 + Unity_RTCfg_START)    // 保存时间戳

// 设备静态属性
#define Unity_RTCfg_ProductKey                     (1 + Unity_RTCfg_SaveTs)   // 产品密钥
#define Unity_RTCfg_DevName                        (1 + Unity_RTCfg_ProductKey) // 设备名称
#define Unity_RTCfg_DevSecret                      (1 + Unity_RTCfg_DevName)  // 设备密钥
#define Unity_RTCfg_NamePlate                      (1 + Unity_RTCfg_DevSecret) // 名牌信息
#define Unity_RTCfg_EquipSn                        (1 + Unity_RTCfg_NamePlate) // 设备序列号
#define Unity_RTCfg_Parent                         (1 + Unity_RTCfg_EquipSn)   // 父级设备编号
#define Unity_RTCfg_BoardType                      (1 + Unity_RTCfg_Parent)    // 主板类型

// 运行时参数
#define Unity_RTCfg_FlashDBrecordMask              (1 + Unity_RTCfg_BoardType) // 日志记录掩码
#define Unity_RTCfg_FlashDBrecordMaxNum            (1 + Unity_RTCfg_FlashDBrecordMask) // 日志记录Z最大条数
#define Unity_RTCfg_FlashDBForceCleanEveryDay      (1 + Unity_RTCfg_FlashDBrecordMaxNum)  // 每多少天强制清空数据库，0表示不强制清空

#define Unity_RTCfg_SleepItvl                      (1 + Unity_RTCfg_FlashDBForceCleanEveryDay) // 睡眠间隔
#define Unity_RTCfg_SleepMethod                    (1 + Unity_RTCfg_SleepItvl) // 睡眠方法
#define Unity_RTCfg_RunMode                        (1 + Unity_RTCfg_SleepMethod) // 运行模式
#define Unity_RTCfg_WakeKeepRTC                    (1 + Unity_RTCfg_RunMode)    // 休眠时RTC是否工作

// 用户配置
#define Unity_RTCfg_Defend_RFIDCheckItvl           (1 + Unity_RTCfg_WakeKeepRTC) // 设防状态下RFID检查间隔
#define Unity_RTCfg_Defend_sensorIRheckItvl        (1 + Unity_RTCfg_Defend_RFIDCheckItvl) // 设防状态下红外传感器检查间隔
#define Unity_RTCfg_Defend_PostureCheckItvl        (1 + Unity_RTCfg_Defend_sensorIRheckItvl) // 设防状态下姿态检查间隔
#define Unity_RTCfg_Onway_RFIDCheckItvl            (1 + Unity_RTCfg_Defend_PostureCheckItvl) // 在途状态下RFID检查间隔
#define Unity_RTCfg_Onway_sensorIRheckItvl         (1 + Unity_RTCfg_Onway_RFIDCheckItvl) // 在途状态下红外传感器检查间隔
#define Unity_RTCfg_Onway_PostureCheckItvl         (1 + Unity_RTCfg_Onway_sensorIRheckItvl) // 在途状态下姿态检查间隔
#define Unity_RTCfg_charg_RFIDCheckItvl            (1 + Unity_RTCfg_Onway_PostureCheckItvl) // 充电状态下RFID检查间隔
#define Unity_RTCfg_charg_sensorIRheckItvl         (1 + Unity_RTCfg_charg_RFIDCheckItvl) // 充电状态下红外传感器检查间隔
#define Unity_RTCfg_charg_PostureCheckItvl         (1 + Unity_RTCfg_charg_sensorIRheckItvl) // 充电状态下姿态检查间隔

// 电量扫描时间
#define Unity_RTCfg_batteryCheckItvl               (1 + Unity_RTCfg_charg_PostureCheckItvl) // 电量扫描时间
#define Unity_RTCfg_onywayOutTimeItvl              (1 + Unity_RTCfg_batteryCheckItvl) // 在途超时时间
#define Unity_RTCfg_onywayOutTimeUploadItvl        (1 + Unity_RTCfg_onywayOutTimeItvl) // 在途超时上报时间

// 轨距与轮距参数
#define Unity_RTCfg_TrackMin                       (1 + Unity_RTCfg_onywayOutTimeUploadItvl) // 轨距最小值
#define Unity_RTCfg_WheelMax                       (1 + Unity_RTCfg_TrackMin) // 轮距最大值
#define Unity_RTCfg_WheelMin                       (1 + Unity_RTCfg_WheelMax) // 轮距最小值
#define Unity_RTCfg_TrackZOF                       (1 + Unity_RTCfg_WheelMin) // 轨距零偏移
#define Unity_RTCfg_WheelZOF                       (1 + Unity_RTCfg_TrackZOF) // 轮距零偏移
#define Unity_RTCfg_TrackCLO                       (1 + Unity_RTCfg_WheelZOF) // 轨距闭合值
#define Unity_RTCfg_WheelCLO                       (1 + Unity_RTCfg_TrackCLO) // 轮距闭合值

// 北斗策略
#define Unity_RTCfg_onwayGnssMethod                (1 + Unity_RTCfg_WheelCLO) // 在途状态下的北斗策略
#define Unity_RTCfg_defendGnssMethod               (1 + Unity_RTCfg_onwayGnssMethod) // 设防状态下的北斗策略
#define Unity_RTCfg_chargGnssMethod                (1 + Unity_RTCfg_defendGnssMethod) // 充电状态下的北斗策略

// 网络策略
#define Unity_RTCfg_MediaMethod                    (1 + Unity_RTCfg_chargGnssMethod) // 媒体传输方式

// 姿态参数
#define Unity_RTCfg_MinAngleChg                    (1 + Unity_RTCfg_MediaMethod) // 最小角度变化

// 电量检测参数
#define Unity_RTCfg_BatteryLow                     (1 + Unity_RTCfg_MinAngleChg) // 低电量阈值
#define Unity_RTCfg_BatteryHigh                    (1 + Unity_RTCfg_BatteryLow) // 高电量阈值
#define Unity_RTCfg_PKPulsItvl                     (1 + Unity_RTCfg_BatteryHigh) // PK脉冲间隔
#define Unity_RTCfg_PKTimeOut                      (1 + Unity_RTCfg_PKPulsItvl) // PK超时时间
#define Unity_RTCfg_DelayShut                      (1 + Unity_RTCfg_PKTimeOut) // 延迟关机时间

// 电池电量曲线
#define Unity_RTCfg_BatteryCurveC                  (1 + Unity_RTCfg_DelayShut) // 电池电量曲线C
#define Unity_RTCfg_BatteryCurveD                  (1 + Unity_RTCfg_BatteryCurveC) // 电池电量曲线D

// MPU6050专用参数
#define Unity_RTCfg_MPU_DLPFLevel                  (1 + Unity_RTCfg_BatteryCurveD) // MPU6050数字低通滤波等级
#define Unity_RTCfg_MPU_THR_FF                     (1 + Unity_RTCfg_MPU_DLPFLevel) // MPU6050自由落体阈值
#define Unity_RTCfg_MPU_DUR_FF                     (1 + Unity_RTCfg_MPU_THR_FF) // MPU6050自由落体持续时间
#define Unity_RTCfg_MPU_THR_MD                     (1 + Unity_RTCfg_MPU_DUR_FF) // MPU6050运动检测阈值
#define Unity_RTCfg_MPU_DUR_MD                     (1 + Unity_RTCfg_MPU_THR_MD) // MPU6050运动检测持续时间
#define Unity_RTCfg_MPU_THR_SD                     (1 + Unity_RTCfg_MPU_DUR_MD) // MPU6050睡眠检测阈值
#define Unity_RTCfg_MPU_DUR_SD                     (1 + Unity_RTCfg_MPU_THR_SD) // MPU6050睡眠检测持续时间

// 网络参数
#define Unity_RTCfg_UpdateURL                      (1 + Unity_RTCfg_MPU_DUR_SD) // 更新服务器地址
#define Unity_RTCfg_UpdatePort                     (1 + Unity_RTCfg_UpdateURL) // 更新服务器端口
#define Unity_RTCfg_ServerURL                      (1 + Unity_RTCfg_UpdatePort) // 服务器地址
#define Unity_RTCfg_ServerPort                     (1 + Unity_RTCfg_ServerURL) // 服务器端口
#define Unity_RTCfg_ConnectTimeOut                 (1 + Unity_RTCfg_ServerPort) // 连接超时时间
#define Unity_RTCfg_KeepAlive                      (1 + Unity_RTCfg_ConnectTimeOut) // 保活时间
#define Unity_RTCfg_APNName                        (1 + Unity_RTCfg_KeepAlive) // APN名称
#define Unity_RTCfg_APNAuth                        (1 + Unity_RTCfg_APNName) // APN认证类型
#define Unity_RTCfg_APNUser                        (1 + Unity_RTCfg_APNAuth) // APN用户名
#define Unity_RTCfg_APNPswd                        (1 + Unity_RTCfg_APNUser) // APN密码
#define Unity_RTCfg_MaxUploadItvl                  (1 + Unity_RTCfg_APNPswd) // 最大上传间隔

// 设防下是否震动立即触发
#define Unity_RTCfg_Exit_defend_RFIDTrigger        (1 + Unity_RTCfg_MaxUploadItvl) // 设防状态下RFID触发
#define Unity_RTCfg_Exit_defend_sensorIRTrigger    (1 + Unity_RTCfg_Exit_defend_RFIDTrigger) // 设防状态下红外传感器触发
#define Unity_RTCfg_Exit_defend_PostureTrigger     (1 + Unity_RTCfg_Exit_defend_sensorIRTrigger) // 设防状态下姿态触发

// 在途下是否震动立即触发
#define Unity_RTCfg_Exit_Onway_RFIDTrigger         (1 + Unity_RTCfg_Exit_defend_PostureTrigger) // 在途状态下RFID触发
#define Unity_RTCfg_Exit_Onway_sensorIRTrigger     (1 + Unity_RTCfg_Exit_Onway_RFIDTrigger) // 在途状态下红外传感器触发
#define Unity_RTCfg_Exit_Onway_PostureTrigger      (1 + Unity_RTCfg_Exit_Onway_sensorIRTrigger) // 在途状态下姿态触发

// 归位下是否震动立即触发
#define Unity_RTCfg_Exit_charg_RFIDTrigger         (1 + Unity_RTCfg_Exit_Onway_PostureTrigger) // 归位状态下RFID触发
#define Unity_RTCfg_Exit_charg_sensorIRTrigger     (1 + Unity_RTCfg_Exit_charg_RFIDTrigger) // 归位状态下红外传感器触发
#define Unity_RTCfg_Exit_charg_PostureTrigger      (1 + Unity_RTCfg_Exit_charg_sensorIRTrigger) // 归位状态下姿态触发

#define Unity_RTCfg_MeidaTrace                     (1 + Unity_RTCfg_Exit_charg_PostureTrigger) // 媒体追踪
#define Unity_RTCfg_VolCalibrate                   (1 + Unity_RTCfg_MeidaTrace) // 电压校准

#define Unity_RTCfg_MAX_ID                         (1 + Unity_RTCfg_VolCalibrate) // 最大ID
typedef enum
{
    FORMAT = 0,
    UNFORMAT,
}JsonFomat;


#define RW_MASK(x)    ((uint32)0x00000001 << (x))
#define NA            ((uint32)0x00000000)
#define R             RW_MASK(0)
#define W             RW_MASK(1)
#define S             RW_MASK(2)

typedef enum {
    NULL_TYPE = 0,
    UINT8_TYPE,
    INT_TYPE,
    INT16_TYPE,
    UINT16_TYPE,
    UINT32_TYPE,
    FLOAT_TYPE,
    STRING_TYPE,
    ARRAY_TYPE
} FieldType;

typedef struct {
    uint32 access;
    uint16 id;
    FieldType type;
    FieldType subType;
    void *addr;
    uint16 len;
    const char *key;
} unityParaList_t;
extern void UintySystemInterface(const char *ArgReq);
extern char* UintyUploadInterface(const char *ArgReq);
extern void UintyRTCfgInterface(const char *ArgReq);
extern void UintyRTStatInterface(const char *ArgReq);
extern char* UintyWanInterface(const char *ArgReq);

extern void SystemParaLoad();
extern void SystemParaStore();

#endif

