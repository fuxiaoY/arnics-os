/**
  ******************************************************************************
  * @attention

  ******************************************************************************
  * @file     ymodem.h
  * @author   
  * @version  V1.0
  * @date     2017/12/04
  * @brief    Description
  * @note     Modified_History
  *
  ******************************************************************************
  */


#ifndef __YMODEM_H_
#define __YMODEM_H_

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Ymodem对象定义 */
#define FILE_LEN_CAL_SUPPORT     1  // 支持计算文件长度
#define FILENAME_MAX_LEN        32  // 支持文件名最大长度
typedef struct
{
#if FILE_LEN_CAL_SUPPORT
    unsigned long filelen;          // Ymodem文件长度
#endif 
    unsigned char filename[FILENAME_MAX_LEN]; // Ymodem文件名称
    unsigned char rec_err;          // 数据块接收状态
    unsigned int  nxt_num;          // 下一数据块序号
    unsigned int  len;              // 数据包指示长度
    unsigned char buf[1024];        // 数据包缓存
}modem_struct;

/* Ymodem对象私有发送接收方法 */
typedef void (*modemSendByte)(unsigned char ch);  // Ymodem串口字节发送函数
typedef char (*modemRecvByte)(unsigned char* ch, unsigned int time_out); // Ymodem串口字节接收函数
typedef void (*Delay_MS)(unsigned long time);  // 
/* Exported defines ----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported Variable ---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* @Ymodem触发文件传输请求, 调用前先外部申请生成一个Ymodem对象.
    return 0: 已选择文件  -1: 未选择文件 */
int ymodem_init(modem_struct* mblock, modemSendByte modem_send, modemRecvByte modem_recv,Delay_MS delay_ms);

/* @Ymodem文件数据包接收 */
int ymodem_recv(modem_struct* mblock);

/* @Ymodem取消文件传输 */
void ymodem_cancle(void);

/* @Ymodem字符串打印 */
void ymodem_print(const unsigned char* s);


#endif

/******* (C) *****END OF FILE****/
