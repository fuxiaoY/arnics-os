#ifndef _UPGRADE_MANAGER_H_
#define _UPGRADE_MANAGER_H_
#include "../../inc/ProjDefine.h"
#include "../../inc/typedef.h"
#include "../../port/portInclude.h"


/* Exported types -------------------------------------------------------*/
#define IMAGE_IDENTIFIER_CMP_LENGTH      32            // 程序标识符比较最大长度
#define IMAGE_BLOCK_SIZE                 256            //256为最小block单位
#define IMAGE_TOTAL_SIZE                (200 * 1024ul) // 程序文件最大长度, 单位: B
#define IMAGE_BLOCK_NUM                 (IMAGE_TOTAL_SIZE / IMAGE_BLOCK_SIZE) //256为最小block单位

#define PART_ENABLE 0X5A  //待升级标志 表示该区域是待升级的程序，下载成功的程序自带该标志
#define PART_LOAD 0X1A    //已装载标志 表示该区域是当前运行的程序，由bootloader改写


/* Class image transfer: image_transfer_status枚举定义 */
typedef enum
{
    transfer_initial = 0,   //初始化状态
    transferring,           //传输中
    transfer_failed,        //传输失败
    transfer_successful     //传输成功
} IMAGE_TRANSFER_STATUS;

/* Image transfer 对象存储数据结构定义 */
/*
已知成员大小总和：32 + 4 + 4 + 4 + 4 + 220 + 220 = 488
填充：在enabled和transfer_status后面各需要3字节填充，共6字节。
*/
typedef struct
{
    uint8  identification[IMAGE_IDENTIFIER_CMP_LENGTH]; // 标识符   32字节
    uint32  enabled;                                    // 程序激活开关 4字节
    uint32 transfer_status;                             // 升级过程状态  4字节
    uint32 image_size;                                  // 程序大小 4字节
    uint32 image_version;                               // 程序版本号 4字节
    uint8  block_status[256];                           // 升级程序块接收状态  256
    uint16 block_crc16[256];                            // 升级程序块校验  512
    uint32 crc16;
} tImageIdentifer;

typedef enum
{
    SECTIONNONE,
    SECTIONA,
    SECTIONB
} UpgradeSecton;

typedef enum{
    GO_DEBUG,
}UpgradeRemap;


extern uint8_t ymodem_upgrade(void);
#endif
