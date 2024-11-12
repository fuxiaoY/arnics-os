#ifndef __MCTDEFINITION_H__
#define __MCTDEFINITION_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_FRAMES 5
/* typedef -----------------------------------------------------------*/
	
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

};

/* typedef -----------------------------------------------------------*/
typedef bool (*cmd_Pack)(uint8_t* buf, size_t maxlen, void *para);
typedef bool (*cmd_Analyze)(uint8_t* buf, size_t len, void *para);

typedef enum
{
    SendRev = 0,
    RecvSend,
}tCmd_type;

typedef struct
{
    uint16_t                        id;
    uint16_t                        timeout;
    char                            *rightPhase;
    char                            *SubRightPhase;
    char                            *errorPhase;
    tCmd_type                       Type;
    cmd_Pack                        pack;
    cmd_Analyze                     analyze;
}tCmd;




#ifdef __cplusplus
}
#endif
#endif
