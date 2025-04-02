/**
  ******************************************************************************
  * @attention

  ******************************************************************************
  * @file     ymodem.c
  * @author   
  * @version  V1.0
  * @date     2017/12/04
  * @brief    Description
  * @note     Modified_History
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "ymodem.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MODEM_MAX_RETRIES         60      // 接收重试次数
#define MODEM_NAK_TIMEOUT         1000    // 单位MS

#define MODEM_SOH  0x01        // 数据块起始字符
#define MODEM_STX  0x02
#define MODEM_EOT  0x04
#define MODEM_ACK  0x06
#define MODEM_NAK  0x15
#define MODEM_CAN  0x18
#define MODEM_C    0x43
#define MODEM_ESC  0x1B

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static modemSendByte send;
static modemRecvByte recv;
static Delay_MS delayms;
/* Private constants ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static unsigned int buf_filelen(unsigned char *ptr)
{
    int datatype=10, result=0;

    if (ptr[0] == '0' && (ptr[1] == 'x' && ptr[1] == 'X'))
    {
        datatype = 16;
        ptr += 2;
    }

    for ( ; *ptr != '\0'; ptr++)
    {
        if (*ptr >= '0' && *ptr <= '9')
        {
            result = result * datatype + *ptr - '0';
        }
        else
        {
            if (datatype == 10)
            {
                return result;
            }
            else
            {
                if (*ptr >= 'A' && *ptr <= 'F')
                {
                    result = result * 16 + *ptr - 55;             //55 = 'A'-10
                }
                else if (*ptr >= 'a' && *ptr <= 'f')
                {
                    result = result * 16 + *ptr - 87;             //87 = 'a'-10
                }
                else
                {
                    return result;
                }
            }
        }
    }
    return result;
}
/*
发送端                                     接收端
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   C   //接收端发送大写字母C启动传输
SOH 00 FF "foo.c" 1064 NUL[118] CRCH CRCL>>>>
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   ACK  
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   C  //收到第0帧数据后，反馈ACK 以及C启动第一帧传输
STX 01 FE data[1024] CRCH CRCL>>>>>>>>>>>>>>>
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   ACK
STX 02 FD data[1024] CRCH CRCL>>>>>>>>>>>>>>>
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   ACK
STX 03 FC data[1024] CRCH CRCL>>>>>>>>>>>>>>>
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   ACK
STX 04 FB data[1024] CRCH CRCL>>>>>>>>>>>>>>>
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   ACK
SOH 05 FA data[100] 1A[28] CRCH CRCL>>>>>>>>
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   ACK
EOT>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  NAK
EOT>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  ACK
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  C
SOH 00 FF NUL[128] CRCH CRCL>>>>>>>>>>>>>>>>
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  ACK
*/
static int modem_recvdata(modem_struct* mblock)
{
    unsigned char hd_found = 0;
    unsigned char ch, blk, cblk, crch, crcl;
    int cksum = 0;

    while(!hd_found) // 头字节
    {
        if(!(recv)(&ch, MODEM_NAK_TIMEOUT)) return -1;
        switch(ch)
        {
            case MODEM_SOH:
            hd_found = 1;
            mblock->len = 128;
            break;
            
            case MODEM_STX:
            hd_found = 1;
            mblock->len = 1024;
            break;
            
            case MODEM_CAN:
            case MODEM_ESC: return 2;
            
            case MODEM_EOT: // 文件传输结束
                (delayms)(100);
                (send)(MODEM_NAK);
                if (!(recv)(&ch, MODEM_NAK_TIMEOUT)) return -1; // 等待另一个MODEM_EOT
                if (ch == MODEM_EOT) {
                    (delayms)(100);
                    (send)(MODEM_ACK);
                }
                (send)(MODEM_C);
                if (!(recv)(&ch, MODEM_NAK_TIMEOUT)) return -1; // 等待另一个MODEM_EOT
                if (ch == MODEM_SOH) {
                    (delayms)(100);
                    (send)(MODEM_ACK);
                }
                return 1;// 文件强制传输结束

        }
    }
    
    if(!(recv)(&blk, MODEM_NAK_TIMEOUT))  return -1; // 数据块序号
    if(!(recv)(&cblk, MODEM_NAK_TIMEOUT)) return -1; // 数据块序号补码
    
    for(int i = 0; i < mblock->len ; i++)
    {
        if(!(recv)(&mblock->buf[i], MODEM_NAK_TIMEOUT)) return -1; // 数据内容
    }
        
    if(!(recv)(&crch, MODEM_NAK_TIMEOUT)) return -1; // CRC16 High byte
    if(!(recv)(&crcl, MODEM_NAK_TIMEOUT)) return -1; // CRC16 Low byte

    /* 块序号检查 */
    if((blk ^ cblk) != 0xff)                       
    {
        mblock->rec_err = 1;
        return -2;
    }
    if(blk != (unsigned char)mblock->nxt_num)  
    {
        return -3;
    }

    /* 校验CRC检查 */
    for(int i = 0; i < mblock->len; i++)
    {
        cksum = cksum ^ (int)mblock->buf[i] << 8;
        for(unsigned char j = 8; j != 0; j--)
        {
            if (cksum & 0x8000)
                cksum = (cksum << 1) ^ 0x1021;
            else
                cksum = cksum << 1;
        }
    }
    if((cksum & 0xffff) != (((int)crch << 8) | crcl))
    {
        mblock->rec_err = 1;
        return -2;
    }

    mblock->nxt_num++;
    mblock->rec_err = 0;
    return 0;
}

/* Public functions ----------------------------------------------------------*/
void ymodem_print(const unsigned char* s)
{
    while(*s != '\0')
    {
        (send)(*s);
        s++;
    }
}

void ymodem_cancle(void)
{
    (send)(MODEM_ESC);
    (send)(MODEM_CAN);
    (send)(MODEM_CAN);
    (send)(MODEM_CAN);
    (send)(MODEM_CAN);
}

int ymodem_init(modem_struct* mblock, modemSendByte modem_send, modemRecvByte modem_recv,Delay_MS delay_ms)
{
    for(int i = 0; i < sizeof(modem_struct); i++)
    {
        ((unsigned char*)mblock)[i] = 0;
    }
    send = modem_send;
    recv = modem_recv;
    delayms = delay_ms;
    
    ymodem_print("\r\nPls select a file to send...\r\n");
    for(int i = 0; i < MODEM_MAX_RETRIES; i++)
    {
        (send)(MODEM_C);
        if(modem_recvdata(mblock) == 0)
        {
            unsigned char* bufptr = mblock->buf;
            unsigned char* namptr = mblock->filename;

            //file name
            while (*bufptr != '\0')
            {
                *namptr++ = *bufptr++;
            }
            *namptr = '\0';
            bufptr++;
            while (*bufptr == ' ')
            {
                bufptr++;
            }

        #if FILE_LEN_CAL_SUPPORT
            mblock->filelen = buf_filelen(bufptr);
        #endif
            (send)(MODEM_ACK);
            return 0;
        }
    }
    ymodem_print("\r\nNo file selected! \r\n");
    return -1;
}

int ymodem_recv(modem_struct* mblock)
{
    for(int i = 0; i < MODEM_MAX_RETRIES; i++)
    {
        if(mblock->nxt_num == 1)
        {
            (send)(MODEM_C);
        }
        else
        {
            (send)(!mblock->rec_err ? MODEM_ACK : MODEM_NAK);
        }
        switch(modem_recvdata(mblock)) 
        {
            case -1: // 接收超时
            break;
            
            case 0:  // 数据包OK
            return 0;
            
            case 1:  // 文件结束
            ymodem_print("\r\nFile trans ok! \r\n");
            return 1;

            case 2:  // 取消发送
            ymodem_print("\r\nFile trans aborted! \r\n");
            return -1;

            default: // 传输错误
            ymodem_print("\r\nFile trans error! \r\n");
            return -1;
        }
    }
    return -1;
}


/******* *****END OF FILE****/
