#ifndef __MCTDEFINITION_H__
#define __MCTDEFINITION_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_FRAMES 5
#define NULL_CMD_SEEK (int32_t)-1
/* typedef -----------------------------------------------------------*/
typedef enum
{
    match_error = (uint8_t)0,
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
    uint16_t startOffset;
    uint16_t endOffset;
    size_t length;
    uint16_t tcmd_id;
    dealprocess status;
} Frame_t;


typedef struct {
    Frame_t frames_expected;
    bool have_expected;
    Frame_t frames[MAX_FRAMES];
    uint8_t size;
} StaticFrameList;

typedef struct MctInstance MctInstance;

/* typedef -----------------------------------------------------------*/
typedef bool (*cmdFun)(MctInstance* inst,void *para);
typedef struct
{
    uint16_t                          id;
    cmdFun                          fun;
}tCmdApi;

typedef tCmdApi const *(*ModemApi)(void);
typedef struct
{
    char                            *name;
    ModemApi                        api;
}tModemList;

struct MctInstance{
    uint8_t* cmd_cache;
    uint8_t* payload_cache;
    size_t cmd_size;
    size_t payload_size;

    ModemApi api;
    StaticFrameList payload_list;
    size_t CMD_MAX_SIZE;
    size_t PAYLOAD_MAX_SIZE;
    int (*mct_write)(void *file, uint16_t len);
    uint32_t (*mct_read)(uint8_t *buf, uint16_t maxlen);

};

/* typedef -----------------------------------------------------------*/
typedef bool (*cmd_Pack)(uint8_t* buf, size_t* len, void *para);
typedef bool (*cmd_Analyze)(uint8_t* buf, size_t len, void *para);

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


typedef struct
{
    int32_t                         id;
    uint16_t                        timeout;
    void                            *rightPhase;
    uint16_t                        rightPhaseLen;
    void                            *SubRightPhase;
    uint16_t                        SubRightPhaseLen;
    void                            *errorPhase;
    uint16_t                        errorPhaseLen;
    tCmd_type                       Type;
    tFormat_type                    format;
    cmd_Pack                        pack;
    cmd_Analyze                     analyze;
}tCmd;




#ifdef __cplusplus
}
#endif
#endif
