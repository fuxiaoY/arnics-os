#include "ril_ring.h"
#include "../port/arnicsPort.h"


#ifdef _USE_RING_
unsigned char debug_rxbuf[DEBUG_UART_RXBUF_SIZE]; /*接收缓冲区 ------------*/
unsigned char debug_txbuf[DEBUG_UART_TXBUF_SIZE]; /*发送缓冲区 ------------*/
ring_buf_t debug_rbsend, debug_rbrecv;            /*收发缓冲区管理 --------*/



/**-控制台--------------------------------------------------------------------------------*/
static void debug_uart_rxclear()
{
    ring_buf_clr(&debug_rbrecv);
}
static void debug_uart_txclear()
{
    ring_buf_clr(&debug_rbsend);
}
/**
 * @brief	    向串口发送缓冲区内写入数据并启动发送
 * @param[in]   buf       -  数据缓存
 * @param[in]   len       -  数据长度
 * @return 	    实际写入长度(如果此时缓冲区满,则返回len)
 */
static unsigned int debug_uart_write(const void *buf, unsigned int len)
{
    unsigned int ret;
    ret = ring_buf_put(&debug_rbsend, (unsigned char *)buf, len);
    enable_debug_uart_IT_TXE();
    return ret;
}
/**
 * @brief	    读取串口接收缓冲区的数据
 * @param[in]   buf       -  数据缓存
 * @param[in]   len       -  数据长度
 * @return 	    (实际读取长度)如果接收缓冲区的有效数据大于len则返回len否则返回缓冲
 *              区有效数据的长度
 */
static unsigned int debug_uart_read(void *buf, unsigned int len)
{
    return ring_buf_get(&debug_rbrecv, (unsigned char *)buf, len);
}
unsigned int user_debug_uart_is_len(void)
{
    return ring_buf_len(&debug_rbrecv); 
}


/**
 * @brief	    向串口发送缓冲区内写入数据并启动发送
 * @param[in]   buf       -  数据缓存
 * @param[in]   len       -  数据长度
 * @return 	    实际写入长度(如果此时缓冲区满,则返回len)
 */
unsigned int user_debug_uart_write(const void *buf, unsigned int len)
{

    unsigned int ret;
    ret = debug_uart_write(buf, len);
    return ret;
}
/**
 * @brief	    读取串口接收缓冲区的数据
 * @param[in]   buf       -  数据缓存
 * @param[in]   len       -  数据长度
 * @return 	    (实际读取长度)如果接收缓冲区的有效数据大于len则返回len否则返回缓冲
 *              区有效数据的长度
 */
unsigned int user_debug_uart_read(void *buf, unsigned int len)
{
    return debug_uart_read(buf, len);
}


/**
 * @brief	    向串口发送单个字节数据
 * @param[in]   byteData  -  要发送的单个字节
 * @return 	    实际写入长度，应始终为1（除非发生严重错误）
 */
unsigned int debug_uart_write_byte(unsigned char* byteData)
{
    return debug_uart_write(byteData, 1);
}

/**
 * @brief	    从串口接收缓冲区读取单个字节数据
 * @param[out]  byteData  -  接收到的单个字节将被存放的变量地址
 * @return 	    实际读取长度，应为1表示成功读取单个字节，0表示无数据可读
 */
unsigned int debug_uart_read_byte(unsigned char *byteData)
{
    return debug_uart_read(byteData, 1);
}

// 发送字符串的函数，一次性发送整个字符串
void debug_uart_send_String(const char *Str)
{
    // 计算字符串的长度（不包括末尾的空字符）
    unsigned int strLen = strlen(Str);

    // 直接使用user_debug_uart_write发送整个字符串
    user_debug_uart_write(Str, strLen);
}
void ring_init()
{
    debug_uart_rxclear();
    debug_uart_txclear();
    ring_buf_init(&debug_rbsend, debug_txbuf, sizeof(debug_txbuf)); /*初始化环形缓冲区 */
    ring_buf_init(&debug_rbrecv, debug_rxbuf, sizeof(debug_rxbuf));
}

#endif

