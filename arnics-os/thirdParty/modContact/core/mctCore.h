/**
  ******************************************************************************
  * @file           : mctCore.h
  * @brief          : modContact core file
  * 
  * 
  * 
  * @version        : 1.0.5
  * @date           : 2026-09-26
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 arong.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#ifndef __MCTCORE_H__
#define __MCTCORE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "../common/mctLib.h"
#include "../port/mctDriver.h"


#define MAX_FRAMES                                                5
#define NULL_CMD_SEEK                                   (int32_t)-1

#define MCT_VERSION                                          "1.0.5"
/* typedef -----------------------------------------------------------*/
typedef enum
{
    match_error                                        =  (uint8_t)0,
    match_sucess,
    match_null,
}frameMacheType;	

typedef enum
{
    FRAME_NONE = 0,
    FRAME_NEW,
    FRAME_SUCCEED,
    FRAME_FAILED
}dealprocess;
typedef struct {                                    
    uint16_t                                              startOffset;
    uint16_t                                                endOffset;
    size_t                                                     length;
    uint16_t                                                  tcmd_id;
    dealprocess                                                status;
} Frame_t;

typedef struct {                            
    Frame_t                                           frames_expected;
    bool                                                have_expected;
    Frame_t                                        frames[MAX_FRAMES];
    uint8_t                                                      size;
} StaticFrameList;

typedef struct MctInstance                                MctInstance;
typedef bool                  (*cmdFun)(MctInstance* inst,void *para);
typedef struct
{
    uint16_t                                                       id;
    cmdFun                                                        fun;
}tCmdApi;

typedef tCmdApi const                              *(*ModemApi)(void);
typedef struct
{
    char                                                        *name;
    ModemApi                                                      api;
}tModemList;

struct MctInstance
{
    uint8_t*                                                cmd_cache;
    uint8_t*                                            payload_cache;
    size_t                                                   cmd_size;
    size_t                                               payload_size;
    bool                                          sticky_frame_enable;
                              
    ModemApi                                                      api;
    StaticFrameList                                      payload_list;
    size_t                                               CMD_MAX_SIZE;
    size_t                                           PAYLOAD_MAX_SIZE;
    int                        (*mct_write)(void *file, uint16_t len);
    uint32_t               (*mct_read)(uint8_t *buf, uint16_t maxlen);

};

/* typedef -----------------------------------------------------------*/
typedef bool       (*cmd_Pack)(uint8_t* buf, size_t* len, void *para);
typedef bool     (*cmd_Analyze)(uint8_t* buf, size_t len, void *para);

typedef enum
{
    SendRev = 0,
    RecvSend,
}tCmd_type;

typedef enum
{
    AscII = 0,
    HeX,
}tFormat_type;

typedef enum
{
    USE_VAR = 0,
    USE_CB,
} sticky_type;

/*-------------------------------------------------------------------------------------*/
/* 切帧策略层 · 类型定义                                                                 */
/*-------------------------------------------------------------------------------------*/

/* 切帧类型：老命令缺省为 DELIMITER(0),故旧表自动走 legacy,行为不变 */
typedef enum
{
    FRAMING_DELIMITER = 0, /* 头 + 尾(legacy,逐位保持） */
    FRAMING_CALLBACK,      /* 框架定位帧头,用户回调决定帧尾 */
    FRAMING_TYPE_MAX,
} framing_kind_t;

/* 扫描结果 */
typedef enum
{
    FRAME_SCAN_ERROR = 0, /* 命中错误帧(DELIMITER 且定义了错误帧） */
    FRAME_SCAN_MATCH,     /* 完整帧 [*start,*end) */
    FRAME_SCAN_NEED_MORE, /* 在帧边界/见头未到齐,继续等,不丢字节 */
    FRAME_SCAN_NO_HEAD,   /* 本窗口内没有该命令的帧 */
} frame_scan_result_t;

/* 中立格式枚举(与引擎的 tFormat_type 解耦） */
typedef enum
{
    FRAMING_FMT_ASCII = 0,
    FRAMING_FMT_HEX,
} framing_format_t;

/*-------------------------------------------------------------------------------------*/
/**
 * @brief 用户切帧回调。框架已定位帧头，由用户决定帧边界。
 *
 *        所有偏移相对 @p buffer(帧头第一字节,buffer[0] 即帧头）。
 *
 * @param[in]  buffer   帧头指针
 * @param[in]  len      帧头起可用字节数
 * @param[out] start    帧起始（相对 buffer，一般为 0）
 * @param[out] end      帧结束（相对 buffer，半开区间）
 * @param[in]  user_arg 用户上下文，可为 NULL
 *
 * @return MATCH 帧完整；NEED_MORE 未到齐继续等；NO_HEAD 无有效帧。
 */
typedef frame_scan_result_t (*framing_user_cb_t)(const uint8_t    *buffer,
                                                 uint16_t             len,
                                                 uint16_t          *start,
                                                 uint16_t            *end,
                                                 void          *user_arg);

/* CALLBACK 附加参数：DELIMITER 复用命令现有相位字段,不使用本结构 */
typedef struct
{
    framing_user_cb_t                                             user_cb;
    void                                                        *user_arg;
} framing_spec_t;

/* 防腐输入：由引擎从 tCmd 装配,策略层只认它,不认识 tCmd */
typedef struct
{
    framing_format_t                                               format;
    const uint8_t                                                   *head;
    uint16_t                                                     head_len;
    const uint8_t                                                   *tail;
    uint16_t                                                     tail_len;
    const uint8_t                                                    *err;
    uint16_t                                                      err_len;
    const framing_spec_t                                            *spec;
} frame_input_t;

/* 策略函数签名；偏移相对 window,len 既是窗口长度也是末端上界 */
typedef frame_scan_result_t (*framing_scan_fn)(
                                             const frame_input_t      *in,
                                             const uint8_t        *window,
                                             uint16_t                 len,
                                             uint16_t              *start,
                                             uint16_t               *end);

/*-------------------------------------------------------------------------------------*/

typedef struct tCmd
{
    int32_t                                                            id;
    uint16_t                                                      timeout;
    void                                                      *rightPhase;
    uint16_t                                                rightPhaseLen;
    void                                                   *SubRightPhase;
    uint16_t                                             SubRightPhaseLen;
    void                                                      *errorPhase;
    uint16_t                                                errorPhaseLen;
    tCmd_type                                                        Type;
    tFormat_type                                                   format;
    cmd_Pack                                                         pack;
    cmd_Analyze                                                   analyze;
    sticky_type                                                stickytype;
    void                                                            *para;
    void*                                               (*get_para)(void);

  
    framing_kind_t                                           framing_kind;
    framing_spec_t                                           framing_spec;
}tCmd;



extern void mct_data_reset(MctInstance* pInstance);

extern void initStaticFrameList(StaticFrameList *list);

extern bool CMD_Execute(MctInstance *inst, \
                                   int32_t expected_id, \
            tCmd const *List,uint16_t cmdNum,void *para);
#ifdef __cplusplus
}
#endif
#endif
