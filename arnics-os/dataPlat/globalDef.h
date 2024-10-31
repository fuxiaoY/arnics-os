#ifndef GLOBAL_DEF_H
#define GLOBAL_DEF_H
#include "../inc/typedef.h"
#include "../inc/projDefine.h"

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------系统数据平台------------------------------------*/
//RTC时间戳对象
typedef struct
{       
  uint8_t Year;		  /*!<RTC 年*/
  uint8_t Month;		/*!<RTC 月*/
  uint8_t Date;		  /*!<RTC 日*/	
  uint8_t Hour;		  /*!<RTC 时*/
  uint8_t Minute;		/*!<RTC 分*/	
  uint8_t Second;		/*!<RTC 秒*/			
  uint8_t Week;		  /*!<RTC 周*/	
} rtcTimeDateTypeDef_t;


/*---------------------------------------------------------------------------------------*/

/*----------------------------------------行政中心----------------------------------------*/

typedef struct {
    time_t ID_Ts;    //消息ID，可用于检查区分消息传送
    uint32_t sleepTime_ms;
} adMessage_t;

/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------消息中心----------------------------------------*/

typedef struct {
    time_t ID_Ts;    //消息ID，可用于检查区分消息传送 
    char buf[20];  // 可以根据需要添加更多的字段
    int length;     // 数据长度
} Message_t;

/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------媒体中心----------------------------------------*/
typedef struct {
    time_t ID_Ts;    //消息ID，可用于检查区分消息传送
    char buf[20];  // 可以根据需要添加更多的字段
    int length;     // 数据长度
} MediaMessage_t;

/*---------------------------------------------------------------------------------------*/

/*----------------------------------------决策层------------------------------------------*/

// 设备运行状态
enum ENUM_WORKSTATE
{
  WORKSTAT_INIT = 0,   // 上电初始化
};


/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------配置数据---------------------------------------*/
//设备运行参数集，存放于片上Flash，同步一份到内存中常驻
struct SytemCfg
{
  uint32_t SaveTs;					//保存时间戳，同时也用于判定Flash上保存的参数集是否有效

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
#define SytemCfg_SIZE	sizeof(SytemCfg)
/*---------------------------------------------------------------------------------------*/

/*-----------------------------------------运行数据----------------------------------------*/
//设备运行状态记录集，常驻内存中，同步存放一份到片上Flash，复位重启后读回来作为缺省状态
struct SytemState
{
  uint32_t SaveTs;				//保存时间戳，同时也用于判定Flash上保存的参数集是否有效

  ENUM_WORKSTATE WorkStat;			//当前运行状态 参考ENUM_WORKSTATE定义
  ENUM_WORKSTATE PreWorkStat;		//上次的运行状态 参考ENUM_WORKSTATE定义


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
#define SytemState_SIZE		sizeof(SytemState)

#ifdef __cplusplus
}
#endif

#endif
