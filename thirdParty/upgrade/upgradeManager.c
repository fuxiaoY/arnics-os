
#include "upgradeManager.h"
#include "../../common/commonInclude.h"
#include "../../drivers/driversInclude.h"
#include "../AlgorithmLib/ymodem.h"
#include "../AlgorithmLib/crc16.h"
#include "../modContact/mctInclude.h"
#include "../ulog/ulogDef.h"

/* interface -----------------------------------------------------------*/
void upgrade_delay_ms(unsigned long ms)
{
    Delay_ms(ms);
}
void Iwdg_reloader()
{
    dev_ctl(&iwdg_ds,IWDG_RELOAD);
}
int debug_uart_read_byte(unsigned char *ch)
{
    return dev_read(&debug_ds,ch,1);

}
void debug_uart_write_byte(unsigned char *ch)
{
    dev_write(&debug_ds,ch,1);
}   

// 擦除分区
void exflash_erase_partition(const char *partition_name)
{
    const struct fal_partition *part = fal_partition_find(partition_name);
    if (part)
    {
        fal_partition_erase(part, 0, part->len);
    }
    else
    {
        ULOG_ERROR("Partition %s not found!", partition_name);
    }
}

// 写入数据到分区
void exflash_write(const char *partition_name, uint32_t address, uint8_t* data, size_t size)
{
    const struct fal_partition *part = fal_partition_find(partition_name);
    if (part)
    {
        fal_partition_write(part, address, data, size);
    }
    else
    {
        ULOG_ERROR("Partition %s not found!", partition_name);
    }
}

// 从分区读取数据
void exflash_read(const char *partition_name, uint32_t address, uint8_t *data, size_t size)
{
    const struct fal_partition *part = fal_partition_find(partition_name);
    if (part)
    {
        fal_partition_read(part, address, data, size);
    }
    else
    {
        ULOG_ERROR("Partition %s not found!", partition_name);
    }
}

// 定义 Ymodem 结构体和全局变量
static modem_struct ymodem;
static tImageIdentifer upgradeStorage;
static tSwTimer ymodem_rev_timeoutTimer = {0};
static UpgradeSecton Section_Flag = SECTIONNONE;
static int fwType = 0;                      // 0: 未知, 1: BOOTLOADER, 2: APP

//退出需要重设的变量
static bool first_packet_received = false;   // 标记是否已经处理过第一个数据包
static unsigned long organizedlen = 0;       //规整化之后的文件长

// 定义 UART 接口重映射变量
static UpgradeRemap remapUart = GO_DEBUG;

/**
 * @brief 初始化 UpgradeManager。
 * 
 * @param remap 重映射 UART 接口选择（GO_MODEM 或 GO_DEBUG）。
 */
void Upgrade_Init(UpgradeRemap remap)
{
    remapUart = remap;

}

/**
 * @brief 通过模块 UART 或调试 UART 发送字节。
 * 
 * @param ch 要发送的字节。
 */
void UsrSendByte(unsigned char ch)
{
    if (remapUart == GO_DEBUG)
    {
        // 发送字节通过调试 UART 接口
        debug_uart_write_byte(&ch);
    }
}

/**
 * @brief 通过模块 UART 或调试 UART 接收字节。
 * 
 * @param ch 用于存储接收到的字节的指针。
 * @param timeout 接收超时时间（毫秒）。
 * @return char 接收到字节返回 1，超时返回 0。
 */
char UsrmodemRecvByte(unsigned char *ch, unsigned int timeout)
{
    if (remapUart == GO_DEBUG)
    {
        // 通过调试 UART 接口接收字节，带超时处理
        halTimerInterval(&ymodem_rev_timeoutTimer, timeout);
        while (!halTimerExpired(&ymodem_rev_timeoutTimer))
        {
            if (debug_uart_read_byte(ch))
            {
                return 1; // 成功接收
            }
        }
        return 0; // 接收超时
    }

    return 0; // 未知的 remapUart 值
}


/*---------------------------------------------------------------------------------------------*/
static void reset_flash_state()
{
    first_packet_received = false;
    organizedlen = 0;
    Section_Flag = SECTIONNONE;
}

static void checkAndUpdateAddress(tImageIdentifer *identifier, const char *partitionIdentA, const char *partitionIdentB,
                                  const char *partitionDataA, const char *partitionDataB, const char *moduleName)
{
    exflash_read(partitionIdentA, 0, (uint8_t*)identifier, sizeof(tImageIdentifer));
    if (identifier->enabled != PART_LOAD)
    {
        ULOG_DEBUG("%s A is not active. Using A partition for upgrade.", moduleName);
        Section_Flag = SECTIONA;
        exflash_erase_partition(partitionIdentA);
        exflash_erase_partition(partitionDataA);
    }
    else
    {
        exflash_read(partitionIdentB, 0, (uint8_t*)identifier, sizeof(tImageIdentifer));
        if (identifier->enabled != PART_LOAD)
        {
            ULOG_DEBUG("%s B is not active. Using B partition for upgrade.", moduleName);
            Section_Flag = SECTIONB;
            exflash_erase_partition(partitionIdentB);
            exflash_erase_partition(partitionDataB);
        }
        else
        {
            ULOG_DEBUG("Both %s A and B are active. Cannot perform upgrade.", moduleName);
        }
    }
}


/**
 * @brief 处理从 Ymodem 接收到的文件数据。
 * 
 * @param mblock 指向包含接收数据的 modem_struct 的指针。
 * @param remaining_len 剩余要处理的文件数据长度。
 */
static bool process_received_file(modem_struct *mblock, unsigned long remaining_len)
{
    ULOG_DEBUG("Received Ymodem Data%d Byte:",mblock->len);
    // 如果是第一个数据包且尚未接收到，处理文件头部信息
    if (remaining_len >= mblock->filelen && !first_packet_received)
    {
        // 将第一个数据包拷贝到临时变量tempIdentifer
        memcpy(&upgradeStorage, mblock->buf, sizeof(tImageIdentifer));
        
        // 输出文件头部信息
        ULOG_DEBUG("Received file header:");
        ULOG_DEBUG("Identification: %s", upgradeStorage.identification);
        ULOG_DEBUG("Image size: %lu", upgradeStorage.image_size);
        ULOG_DEBUG("Image version: %lu", upgradeStorage.image_version);
        ULOG_DEBUG("Enabled: 0x%x", upgradeStorage.enabled);
        ULOG_DEBUG("Transfer status: %d", upgradeStorage.transfer_status);

        if (strstr((char*)upgradeStorage.identification, "app"))
        {
            fwType = 2;
            ULOG_DEBUG("Detected: APP");
        }
        else if (strstr((char*)upgradeStorage.identification, "boot"))
        {
            fwType = 1;
            ULOG_DEBUG("Detected: BOOTLoader"); 
        }
        else
        {
            ULOG_DEBUG("Unknown image type.");
            return false;
        }

        // 临时保存从闪存读取的数据
        tImageIdentifer tempIdentifer;
        // 根据不同类型判断使用A地址还是B地址，并擦除选定的区域（包含identity和数据区）
        if(fwType == 2)
        {
            checkAndUpdateAddress(&tempIdentifer, PARTITION_NAME_FW_IDENTIFER_APP_A, PARTITION_NAME_FW_IDENTIFER_APP_B,
                                  PARTITION_NAME_FW_APP_A, PARTITION_NAME_FW_APP_B, "Application");
        }
        if(fwType == 1)
        {
            checkAndUpdateAddress(&tempIdentifer, PARTITION_NAME_FW_IDENTIFIER_BOOT_A, PARTITION_NAME_FW_IDENTIFER_BOOT_B,
                                  PARTITION_NAME_FW_BOOT_A, PARTITION_NAME_FW_BOOT_B, "Boot");
        }
        if(Section_Flag == SECTIONNONE)
        {
            ULOG_DEBUG("Section_Flag invalid");
            return false;
        }

        // 标记第一个包已接收
        first_packet_received = true;
    }
    else // 处理文件的其他数据
    {
        //APP程序
        if (first_packet_received && (2 == fwType))
        {
            unsigned long writeAddr =  (organizedlen - remaining_len) - 0x400;//去掉第一包的头部信息 1K字节
            if(SECTIONA == Section_Flag)
            {
                exflash_write(PARTITION_NAME_FW_APP_A, writeAddr, mblock->buf, mblock->len);
            }
            else if(SECTIONB == Section_Flag)
            {
                exflash_write(PARTITION_NAME_FW_APP_B, writeAddr, mblock->buf, mblock->len);
            }
        }
        //BOOT程序
        else if (first_packet_received && (1 == fwType))
        {
            unsigned long writeAddr =  (organizedlen - remaining_len) - 0x400;//去掉第一包的头部信息 1K字节
            if(SECTIONA == Section_Flag)
            {
                exflash_write(PARTITION_NAME_FW_BOOT_A, writeAddr, mblock->buf, mblock->len);
            }
            else if(SECTIONB == Section_Flag)
            {
                exflash_write(PARTITION_NAME_FW_BOOT_B, writeAddr, mblock->buf, mblock->len);
            } 
        }
        else
        {
            ULOG_DEBUG("No valid address found for writing data. Skipping data write operation.");
            return false;
        }
    }
		return true;
}

/**
 * @brief 将当前的upgradeStorage写入IDENTIFY区域。
 * 
 * 根据当前的isBootloader和address进行写入操作。
 */

void SaveIdentify()
{
// 写入IDENTIFY区域
    if(fwType == 2)
    {
        if(SECTIONA == Section_Flag)
        {
            ULOG_DEBUG("Writing App A IDENTIFY...");
            exflash_erase_partition(PARTITION_NAME_FW_IDENTIFER_APP_A);
            exflash_write(PARTITION_NAME_FW_IDENTIFER_APP_A, 0, (uint8_t*)&upgradeStorage, sizeof(tImageIdentifer));
        }
        else if(SECTIONB == Section_Flag)
        {
            ULOG_DEBUG("Writing App B IDENTIFY...");
            exflash_erase_partition(PARTITION_NAME_FW_IDENTIFER_APP_B);
            exflash_write(PARTITION_NAME_FW_IDENTIFER_APP_B, 0, (uint8_t*)&upgradeStorage, sizeof(tImageIdentifer));
        }
    }
    if(fwType == 1)
    {
        if(SECTIONA == Section_Flag)
        {
            ULOG_DEBUG("Writing Bootloader A IDENTIFY...");
            exflash_erase_partition(PARTITION_NAME_FW_IDENTIFIER_BOOT_A);
            exflash_write(PARTITION_NAME_FW_IDENTIFIER_BOOT_A, 0, (uint8_t*)&upgradeStorage, sizeof(tImageIdentifer));
        }
        else if(SECTIONB == Section_Flag)
        {
            ULOG_DEBUG("Writing Bootloader B IDENTIFY...");
            exflash_erase_partition(PARTITION_NAME_FW_IDENTIFER_BOOT_B);
            exflash_write(PARTITION_NAME_FW_IDENTIFER_BOOT_B, 0, (uint8_t*)&upgradeStorage, sizeof(tImageIdentifer));
        }
    }
}




static void UpgradeInfoPrint(void)
{
    Iwdg_reloader(); // 喂狗，防止看门狗复位
    SaveIdentify();
    // 文件接收完成
    ULOG_INFO("File transfer completed!");
    ULOG_INFO("--------------------------------------");
    ULOG_INFO("Identification: %s", upgradeStorage.identification);
    ULOG_INFO("Image size: %lu", upgradeStorage.image_size);
    ULOG_INFO("Image version: %lu", upgradeStorage.image_version);
    ULOG_INFO("Enabled: 0x%x", upgradeStorage.enabled);
    ULOG_INFO("Transfer status: %d", upgradeStorage.transfer_status);
    ULOG_INFO("flash programm area: %d", fwType);
    if(fwType == 2)
    {
        switch (Section_Flag)
        {
            case SECTIONA:
            {
            ULOG_INFO("flash programm address: %s", PARTITION_NAME_FW_APP_A);
            ULOG_INFO("flash programm identify address: %s", PARTITION_NAME_FW_IDENTIFER_APP_A);
            }
            break;
            case SECTIONB:
            {
            ULOG_INFO("flash programm address: %s", PARTITION_NAME_FW_APP_B);
            ULOG_INFO("flash programm identify address: %s", PARTITION_NAME_FW_IDENTIFER_APP_B);
            }
            break;
            default:
            break;
        }
    }
    if(fwType == 1)
    {
        switch (Section_Flag)
        {
            case SECTIONA:
            {
            ULOG_INFO("flash programm address: %s", PARTITION_NAME_FW_BOOT_A);
            ULOG_INFO("flash programm identify address: %s", PARTITION_NAME_FW_IDENTIFIER_BOOT_A);
            }
            break;
            case SECTIONB:
            {
            ULOG_INFO("flash programm address: %s", PARTITION_NAME_FW_BOOT_B);
            ULOG_INFO("flash programm identify address: %s", PARTITION_NAME_FW_IDENTIFER_BOOT_B);
            }
            break;
            default:
            break;
        }
    }
    ULOG_INFO("--------------------------------------");
}


/* BootLoader写入FLASH测试 */
uint32_t ImageSize = 0;
UpgradeSecton Section_Flag_Uboot = SECTIONNONE;
static uint8_t checkAddress(tImageIdentifer *identifier, const char *partitionIdentA, const char *partitionIdentB,
                                  const char *partitionDataA, const char *partitionDataB, const char *moduleName)
{

    ULOG_INFO("Checking %s A...", moduleName);
    exflash_read(partitionIdentA, 0, (uint8_t*)identifier, sizeof(tImageIdentifer));
    ULOG_INFO("--------------------------------------");
    ULOG_INFO("Identification: %s", identifier->identification);
    ULOG_INFO("Image size: %lu", identifier->image_size);
    ULOG_INFO("Image version: %lu", identifier->image_version);
    ULOG_INFO("Enabled: 0x%x", identifier->enabled);
    ULOG_INFO("Transfer status: %d", identifier->transfer_status);
    ULOG_INFO("--------------------------------------");
    if (identifier->enabled == PART_ENABLE)
    {
        ULOG_INFO("%s A is ready to upgrade. Using A address for upgrade.", moduleName);
        identifier->enabled = PART_LOAD;
        ImageSize = identifier->image_size;
        //回写A区，清除B区
        exflash_erase_partition(partitionIdentA);
        exflash_write(partitionIdentA, 0, (uint8_t*)identifier, sizeof(tImageIdentifer));
        exflash_erase_partition(partitionIdentB);
        Section_Flag_Uboot = SECTIONA;
        
        return 1;
    }
    else
    {
        ULOG_INFO("Checking %s B...", moduleName);
        exflash_read(partitionIdentB, 0, (uint8_t*)identifier, sizeof(tImageIdentifer));
        ULOG_INFO("--------------------------------------");
        ULOG_INFO("Identification: %s", identifier->identification);
        ULOG_INFO("Image size: %lu", identifier->image_size);
        ULOG_INFO("Image version: %lu", identifier->image_version);
        ULOG_INFO("Enabled: 0x%x", identifier->enabled);
        ULOG_INFO("Transfer status: %d", identifier->transfer_status);
        ULOG_INFO("--------------------------------------");
        if (identifier->enabled == PART_ENABLE)
        {
            ULOG_INFO("%s B is ready to upgrade. Using B address for upgrade.", moduleName);
            identifier->enabled = PART_LOAD;
            ImageSize = identifier->image_size;
            //回写B区，清除a区
            exflash_erase_partition(partitionIdentB);
            exflash_write(partitionIdentB, 0, (uint8_t*)identifier, sizeof(tImageIdentifer));
            exflash_erase_partition(partitionIdentA);
            Section_Flag_Uboot = SECTIONB;
            
            return 1;
        }  
        else
        {
            ULOG_INFO("Both boot A and boot B are no new image. No need Upgrade.");
            return 0;
        }
    }
}

static void transferDataUntilComplete(void)
{
    ULOG_INFO("data transfer start ...");
    Delay_ms(1000);
    uint8_t flashData[FLASH_PAGE_SIZE] __attribute__((aligned(4))) = {0x00};
    uint32_t totalBytesTransferred = 0;
    uint32_t bytesToTransfer = sizeof(flashData);

    ULOG_INFO("erase partition:  %s",PARTITION_NAME_ONCHIP_BOOTLOADER);
    exflash_erase_partition(PARTITION_NAME_ONCHIP_BOOTLOADER);
    
    Delay_ms(1000);
    ULOG_INFO("Wrtting img ...");
    uint32_t count = 0;
    ULOG_INFO("Image size: %lu", ImageSize);
    while (totalBytesTransferred < CODEFLASH_SIZE_BOOTLOAD)
    {
        Iwdg_reloader(); // 喂狗，防止看门狗复位
        Delay_ms(10);
        if(SECTIONA == Section_Flag_Uboot)
        {
            exflash_read(PARTITION_NAME_FW_BOOT_A,totalBytesTransferred, flashData, bytesToTransfer);
        }
        else if(SECTIONB == Section_Flag_Uboot)
        {
            exflash_read(PARTITION_NAME_FW_BOOT_B,totalBytesTransferred, flashData, bytesToTransfer);
        }

        //判断flashData[FLASH_PAGE_SIZE]是否全为0xFF,是则return
        if (count >= (ImageSize * 1024/ FLASH_PAGE_SIZE)) 
        {
            bool allFF = true;
            for (size_t i = 0; i < bytesToTransfer; ++i)
            {
                if (flashData[i] != 0xFF)
                {
                    allFF = false;
                    break;
                }
            }

            if (allFF)
            {
                ULOG_INFO("All bytes are 0xFF, returning...");
                return;
            }
        }

        exflash_write(PARTITION_NAME_ONCHIP_BOOTLOADER, totalBytesTransferred, flashData, bytesToTransfer);
        totalBytesTransferred += bytesToTransfer;
        //ULOG_INFO("program addr: 0x%x",USER_APP_ADDR_A + totalBytesTransferred);
        count++;
    }
}


void BootLoaderDataTransfer(void)
{
    tImageIdentifer bootIdentifer;
    memset(&bootIdentifer, 0, sizeof(bootIdentifer));
    uint8_t checkresult = 0;
    checkresult = checkAddress(&bootIdentifer, PARTITION_NAME_FW_IDENTIFIER_BOOT_A, PARTITION_NAME_FW_IDENTIFER_BOOT_B,
                                  PARTITION_NAME_FW_BOOT_A, PARTITION_NAME_FW_BOOT_B, "Boot");

    if (0 == checkresult) // 无更新程序
    {
        ULOG_INFO("Normal state!");
        return;
    }
    ULOG_INFO("check identifer OK ! start upgrade...");
    // 搬运数据：
    transferDataUntilComplete();
    ULOG_INFO(" upgrade Done!");

}
/**
 * @brief Ymodem 文件接收测试函数。
 * @attention:  ymodem只下发1024格式，ymodem.len都是1024
 */
uint8_t ymodem_upgrade(void)
{
    uint8_t result = 0; // 初始化成功标志为0
    ULOG_INFO("Ymodem Start..."); // 打印日志信息，表示Ymodem升级开始
    // 初始化 Ymodem
    if (ymodem_init(&ymodem, UsrSendByte, UsrmodemRecvByte,upgrade_delay_ms) == 0)
    {
        int res = 0;
        uint16_t packNum = 0;
        unsigned long remaining_len = ymodem.filelen;
        // 将 remaining_len 补成 1024 的页整数倍
        if (remaining_len % 1024 != 0)
        {
            remaining_len = ((remaining_len / 1024) + 1) * 1024;

        }
        organizedlen = remaining_len;

        // 文件选择成功，开始接收文件数据
        while (1)
        {
            res = ymodem_recv(&ymodem);
            if (res == 0)
            {
                Iwdg_reloader(); // 喂狗，防止看门狗复位
                // 接收文件数据
                unsigned int data_len = ymodem.len;

				if((128 == data_len)&&first_packet_received)//不允许非1024传输
                {
                    // 接收错误或取消
                    ULOG_INFO("File transfer not with 1K!");
                    break;
                }

                if(!process_received_file(&ymodem, remaining_len))
                {
                    // 终止传输
                    ULOG_INFO("frame parse error, transfer aborted!");
                    break;
                }
                if (remaining_len >= data_len)
                {
                    remaining_len -= data_len;
                }
                else
                {
                    remaining_len = 0;
                }
                packNum++;
            }
            else if (res == 1)
            {
                UpgradeInfoPrint();

                //bootloader transmission
                if(fwType == 1)
                {
                    ULOG_INFO("BootLoader upgrade start...");
                    BootLoaderDataTransfer();
                }
                result = 1; // 升级成功
                break;
            }
            else
            {
                // 接收错误或取消
                ULOG_INFO("File transfer error or aborted!");
                break;
            }
        }

        // 重置静态变量
        reset_flash_state();
    }
    else
    {
        // 文件选择失败
        ULOG_INFO("No file selected!");
    }
    return result;
}

/* 远程升级-------------------------------------------------------------
#define GETWAIT_TIMEOUT_MS          100         //GET失败重试的时间
#define FRAME_GET_RETRY_TIMES       10          //CRC比对错误重新GET的次数
#define EXFLASH_PAGE_SIZE           1024    
static uint8_t *framedata;



static bool HttpCmdExecute(uint16_t id, void *para)
{
    uint32_t cnt = 0;
    const uint32_t maxCnt = GETWAIT_TIMEOUT_MS / WAIT_SCHEDULE_TIME_MS;
    while (cnt < maxCnt)
    {
        if(mctApiExecute(&mediaInstance,id,para))
        {
            cnt = 0;
            return true;
        }
        else
        {
            cnt++; 
        }
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }
    return false;
}

static void HttpEraseFlash(void)
{
    if(fwType == 2)
    {
        if(SECTIONA == Section_Flag)
        {
            exflash_erase_partition(PARTITION_NAME_FW_IDENTIFER_APP_A);
            exflash_erase_partition(PARTITION_NAME_FW_APP_A);
        }
        else if(SECTIONB == Section_Flag)
        {
            exflash_erase_partition(PARTITION_NAME_FW_IDENTIFER_APP_B);
            exflash_erase_partition(PARTITION_NAME_FW_APP_B);
        }
    }
    if(fwType == 1)
    {
        if(SECTIONA == Section_Flag)
        {
            exflash_erase_partition(PARTITION_NAME_FW_IDENTIFIER_BOOT_A);
            exflash_erase_partition(PARTITION_NAME_FW_BOOT_A);
        }
        else if(SECTIONB == Section_Flag)
        {
            exflash_erase_partition(PARTITION_NAME_FW_IDENTIFER_BOOT_B);
            exflash_erase_partition(PARTITION_NAME_FW_BOOT_B);
        }
    }
}

static void HttpCheckUpdateAddress(tImageIdentifer *identifier, const char *partitionIdentA, const char *partitionIdentB,
                                  const char *partitionDataA, const char *partitionDataB, const char *moduleName)
{
    exflash_read(partitionIdentA, 0, (uint8_t*)identifier, sizeof(tImageIdentifer));
    if (identifier->enabled != PART_LOAD)
    {
        ULOG_INFO("%s A is not active. Using A partition for upgrade.", moduleName);
        Section_Flag = SECTIONA;
    }
    else
    {
        exflash_read(partitionIdentB, 0, (uint8_t*)identifier, sizeof(tImageIdentifer));
        if (identifier->enabled != PART_LOAD)
        {
            ULOG_INFO("%s B is not active. Using B partition for upgrade.", moduleName);
            Section_Flag = SECTIONB;
        }
        else
        {
            ULOG_INFO("Both %s A and B are active. Cannot perform upgrade.", moduleName);
        }
    }
}

static bool HttpFirstFrameParse(tImageIdentifer *fIdentifier)
{
    if (!first_packet_received)
    {
        //远程identifier
        ULOG_INFO("------------------------");
        ULOG_INFO("Http file header:");
        ULOG_INFO("Identification: %s", fIdentifier->identification);
        ULOG_INFO("Image size: %lu", fIdentifier->image_size);
        ULOG_INFO("Image version: %lu", fIdentifier->image_version);
        ULOG_INFO("Enabled: 0x%x", fIdentifier->enabled);
        ULOG_INFO("Transfer status: %d", fIdentifier->transfer_status);
        if (strstr((char*)fIdentifier->identification, "app"))
        {
            fwType = 2;
            ULOG_INFO("Detected: APP");
        }
        else if (strstr((char*)fIdentifier->identification, "boot"))
        {
            fwType = 1;
            ULOG_INFO("Detected: BOOTLoader"); 
        }
        else
        {
            ULOG_INFO("[Error] Unknown image type.");
            return false;
        }
        tImageIdentifer tempIdentifer;
        if(fwType == 2)
        {
            HttpCheckUpdateAddress(&tempIdentifer, PARTITION_NAME_FW_IDENTIFER_APP_A, PARTITION_NAME_FW_IDENTIFER_APP_B,
                                  PARTITION_NAME_FW_APP_A, PARTITION_NAME_FW_APP_B, "Application");
        }
        if(fwType == 1)
        {
            HttpCheckUpdateAddress(&tempIdentifer, PARTITION_NAME_FW_IDENTIFIER_BOOT_A, PARTITION_NAME_FW_IDENTIFER_BOOT_B,
                                  PARTITION_NAME_FW_BOOT_A, PARTITION_NAME_FW_BOOT_B, "Boot");
        }
        //本地identifier
        ULOG_INFO("------------------------");
        ULOG_INFO("Local file header:");
        ULOG_INFO("Identification: %s", tempIdentifer.identification);
        ULOG_INFO("Image size: %lu", tempIdentifer.image_size);
        ULOG_INFO("Image version: %lu", tempIdentifer.image_version);
        ULOG_INFO("Enabled: 0x%x", tempIdentifer.enabled);
        ULOG_INFO("Transfer status: %d", tempIdentifer.transfer_status);
        if(Section_Flag != SECTIONNONE)
        {
            memcpy(&upgradeStorage, &tempIdentifer, sizeof(tImageIdentifer));
        }
        else
        {
            ULOG_INFO("[Error] Section_Flag invalid");
            return false;
        }
        first_packet_received = true;
        return true;
    }
    ULOG_INFO("[Error] not the first frame");
    return false;
}

static uint16_t HttpCrc16Caculate(void *buf,uint16_t len)
{
    return DL_CRC16(buf, len);
}

static void HttpExFlashWrite(uint32_t writeAddr, uint8_t *buffer, uint32_t len)
{
    if (first_packet_received && (2 == fwType))
    {
        if(SECTIONA == Section_Flag)
        {
            exflash_write(PARTITION_NAME_FW_APP_A, writeAddr, buffer, len);
        }
        else
        {
            exflash_write(PARTITION_NAME_FW_APP_B, writeAddr, buffer, len);
        }
    }
    else if (first_packet_received && (1 == fwType))
    {
        if(SECTIONA == Section_Flag)
        {
            exflash_write(PARTITION_NAME_FW_BOOT_A, writeAddr, buffer, len);
        }
        else
        {
            exflash_write(PARTITION_NAME_FW_BOOT_B, writeAddr, buffer, len);
        }
    }
}

static bool HttpGetSingeFrame(uint8_t blocknum, http_t *httppara)
{
    if((!EXFLASH_PAGE_SIZE % httppara->fileLen) || httppara->fileLen > EXFLASH_PAGE_SIZE)
    {
        ULOG_INFO("[ERROR] package size incorrect");
        return false;
    }
    uint8_t pack_num = EXFLASH_PAGE_SIZE / httppara->fileLen;
    ULOG_ALWAYS("frame index = %d",blocknum);
    for(uint8_t i = 0; i < pack_num; i++)
    {
        httppara->BlockNum = pack_num * blocknum + i;
        if(HttpCmdExecute(CMD_HTTPGET,httppara))
        {
            memcpy(framedata + (httppara->fileLen * i), httppara->fileData, httppara->fileLen);      
        }
        else
        {
            ULOG_INFO("[ERROR] http pack %u get failed",httppara->BlockNum);
            return false; 
        }
        ULOG_INFO("Get HTTP Pakage : %d",httppara->BlockNum);
    }
    return true;
}

void HttpFlagClear(void)
{
    reset_flash_state();
    memset(framedata,0,EXFLASH_PAGE_SIZE);
    memset(&upgradeStorage,0,sizeof(upgradeStorage));
}

//http upgrade
bool HttpUpdateProgram(http_t *httppara)
{
    uint16_t pre_crc16 = 0, cur_crc16 = 0;
    uint16_t data_frame_index = 0, data_frame_number = 0;
    //resue ymodem.buf
    framedata = ymodem.buf;
    memset(framedata, 0, EXFLASH_PAGE_SIZE);

    //http connect
    if(!HttpCmdExecute(CMD_HTTPCONNECT,NULL))
    {
        ULOG_INFO("[Error] http connect failed");
        return false;
    }
    ULOG_INFO("http connect successfully,upgrade start...");

    //get identifier
    tImageIdentifer tempStorage;
	uint8_t cnt = 0;
    while (cnt < FRAME_GET_RETRY_TIMES)
    {
        if(user_debug_uart_is_len())
        {
            uint8_t ch;
            debug_uart_read_byte(&ch);
            if(ch == 'q')
            {
                ULOG_INFO("User stop upgrade");
                HttpFlagClear();
                return false;
            }
            else
            {
                debug_uart_rxclear();
            }
        }

        if(!HttpGetSingeFrame(data_frame_index,httppara)) 
        {
            HttpFlagClear();
            return false;
        }
        memcpy(&tempStorage, framedata, sizeof(tImageIdentifer));
        pre_crc16 = tempStorage.crc16;
        tempStorage.crc16 = 0;
        cur_crc16 = HttpCrc16Caculate(&tempStorage,sizeof(tImageIdentifer));
        ULOG_INFO("pre_crc16 = %04X, cur_crc16 = %04X",pre_crc16, cur_crc16);
        if(pre_crc16 == cur_crc16) 
        {
            tempStorage.crc16 = cur_crc16;
            if(!HttpFirstFrameParse(&tempStorage))
            {
                HttpFlagClear();
                return false;
            }
            break;
        }
        //retry maximum
        if(cnt == FRAME_GET_RETRY_TIMES - 1)
        {
            ULOG_INFO("[Error] frame %u crc16 mismatch",data_frame_index);
            tempStorage.block_status[data_frame_index -1] = 0;
            HttpFlagClear();
            return false;
        }
        cnt++;
        MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
    }

    //breakpoint check
    if(!strncmp((const char *)upgradeStorage.identification,(const char *)tempStorage.identification,IMAGE_IDENTIFIER_CMP_LENGTH) &&
        tempStorage.image_size == upgradeStorage.image_size &&
        upgradeStorage.enabled == 0xFF &&
        upgradeStorage.transfer_status ==  transferring &&
        tempStorage.image_size == upgradeStorage.image_size &&
        tempStorage.image_version == upgradeStorage.image_version)
    {
        //brakpoint find
        uint16_t temp_index = 0;
        while(upgradeStorage.block_status[temp_index]) temp_index++;
        data_frame_index = temp_index;
        ULOG_INFO("------------------------");
        ULOG_INFO("breakpoint resume, breakpoint index = %d",data_frame_index);
    }
    else
    {
        //no brakpoint
        HttpEraseFlash();
        data_frame_index = 1;
        ULOG_INFO("------------------------");
        ULOG_INFO("a new download:");
        memcpy(&upgradeStorage,&tempStorage,sizeof(tImageIdentifer));
    }

    //frame number caculate
    data_frame_number = upgradeStorage.image_size;
    ULOG_INFO("frame number = %d",data_frame_number);

    //start get frame
    upgradeStorage.transfer_status = transferring;
    while(data_frame_index <= data_frame_number)
    {
        uint8_t cnt = 0;
        Iwdg_reloader();
        while (cnt < FRAME_GET_RETRY_TIMES)
        {
            if(user_debug_uart_is_len())
            {
                uint8_t ch;
                debug_uart_read_byte(&ch);
                if(ch == 'q')
                {
                    ULOG_INFO("User stop upgrade");
                    HttpFlagClear();
                    return false;
                }
                else
                {
                    debug_uart_rxclear();
                }
            }
            if(!HttpGetSingeFrame(data_frame_index,httppara)) 
            {
                upgradeStorage.transfer_status = transfer_failed;
                HttpFlagClear();
                return false;
            }
            pre_crc16 = upgradeStorage.block_crc16[data_frame_index -1];
            cur_crc16 = HttpCrc16Caculate(framedata,EXFLASH_PAGE_SIZE);
            if(cur_crc16 == pre_crc16) 
            {
                upgradeStorage.block_status[data_frame_index -1] = 1;
                HttpExFlashWrite((data_frame_index -1) * EXFLASH_PAGE_SIZE, framedata, EXFLASH_PAGE_SIZE);
                break;
            }
            else
            {
                ULOG_INFO("crc16 incorrect, retry...");
                ULOG_INFO("pre_crc16 = %04X, cur_crc16 = %04X",pre_crc16, cur_crc16);
            }
            //retry maximum
            if(cnt == FRAME_GET_RETRY_TIMES - 1)
            {
                ULOG_INFO("[Error] frame %u crc16 mismatch",data_frame_index);
                upgradeStorage.block_status[data_frame_index -1] = 0;
                upgradeStorage.transfer_status = transfer_failed;
                HttpFlagClear();
                return false;
            }
			memset(framedata,0,EXFLASH_PAGE_SIZE);
            cnt++;
            MCT_DELAY(WAIT_SCHEDULE_TIME_MS);
        }
        upgradeStorage.enabled = 0xFF;
        SaveIdentify();
        memset(framedata,0,EXFLASH_PAGE_SIZE);
        data_frame_index++;
    }
    Iwdg_reloader();
    //receved over
    upgradeStorage.enabled = PART_ENABLE;
    upgradeStorage.transfer_status = transfer_successful;
    UpgradeInfoPrint();
    //bootloader transmission
    if(fwType == 1)
    {
        ULOG_INFO("BootLoader upgrade start...");
        BootLoaderDataTransfer();
    }
    else
    {
        //software reset
        ULOG_INFO("HTTP transmission done, please restart to upgrade");
    }
    HttpFlagClear();
    Iwdg_reloader();
    HttpCmdExecute(CMD_HTTPCLOSE, httppara);
    return true;
}

//identifer read
void readUpdateIdendiferAddress(tImageIdentifer *identifier, const char *partitionIdent)
{
    exflash_read(partitionIdent, 0, (uint8_t*)identifier, sizeof(tImageIdentifer));
}
*/
