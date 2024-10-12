#include "ril_ring.h"
#include "../port/arnicsPort.h"


#ifdef _USE_RING_
unsigned char debug_rxbuf[DEBUG_UART_RXBUF_SIZE]; /*���ջ����� ------------*/
unsigned char debug_txbuf[DEBUG_UART_TXBUF_SIZE]; /*���ͻ����� ------------*/
ring_buf_t debug_rbsend, debug_rbrecv;            /*�շ����������� --------*/



/**-����̨--------------------------------------------------------------------------------*/
static void debug_uart_rxclear()
{
    ring_buf_clr(&debug_rbrecv);
}
static void debug_uart_txclear()
{
    ring_buf_clr(&debug_rbsend);
}
/**
 * @brief	    �򴮿ڷ��ͻ�������д�����ݲ���������
 * @param[in]   buf       -  ���ݻ���
 * @param[in]   len       -  ���ݳ���
 * @return 	    ʵ��д�볤��(�����ʱ��������,�򷵻�len)
 */
static unsigned int debug_uart_write(const void *buf, unsigned int len)
{
    unsigned int ret;
    ret = ring_buf_put(&debug_rbsend, (unsigned char *)buf, len);
    enable_debug_uart_IT_TXE();
    return ret;
}
/**
 * @brief	    ��ȡ���ڽ��ջ�����������
 * @param[in]   buf       -  ���ݻ���
 * @param[in]   len       -  ���ݳ���
 * @return 	    (ʵ�ʶ�ȡ����)������ջ���������Ч���ݴ���len�򷵻�len���򷵻ػ���
 *              ����Ч���ݵĳ���
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
 * @brief	    �򴮿ڷ��ͻ�������д�����ݲ���������
 * @param[in]   buf       -  ���ݻ���
 * @param[in]   len       -  ���ݳ���
 * @return 	    ʵ��д�볤��(�����ʱ��������,�򷵻�len)
 */
unsigned int user_debug_uart_write(const void *buf, unsigned int len)
{

    unsigned int ret;
    ret = debug_uart_write(buf, len);
    return ret;
}
/**
 * @brief	    ��ȡ���ڽ��ջ�����������
 * @param[in]   buf       -  ���ݻ���
 * @param[in]   len       -  ���ݳ���
 * @return 	    (ʵ�ʶ�ȡ����)������ջ���������Ч���ݴ���len�򷵻�len���򷵻ػ���
 *              ����Ч���ݵĳ���
 */
unsigned int user_debug_uart_read(void *buf, unsigned int len)
{
    return debug_uart_read(buf, len);
}


/**
 * @brief	    �򴮿ڷ��͵����ֽ�����
 * @param[in]   byteData  -  Ҫ���͵ĵ����ֽ�
 * @return 	    ʵ��д�볤�ȣ�Ӧʼ��Ϊ1�����Ƿ������ش���
 */
unsigned int debug_uart_write_byte(unsigned char* byteData)
{
    return debug_uart_write(byteData, 1);
}

/**
 * @brief	    �Ӵ��ڽ��ջ�������ȡ�����ֽ�����
 * @param[out]  byteData  -  ���յ��ĵ����ֽڽ�����ŵı�����ַ
 * @return 	    ʵ�ʶ�ȡ���ȣ�ӦΪ1��ʾ�ɹ���ȡ�����ֽڣ�0��ʾ�����ݿɶ�
 */
unsigned int debug_uart_read_byte(unsigned char *byteData)
{
    return debug_uart_read(byteData, 1);
}

// �����ַ����ĺ�����һ���Է��������ַ���
void debug_uart_send_String(const char *Str)
{
    // �����ַ����ĳ��ȣ�������ĩβ�Ŀ��ַ���
    unsigned int strLen = strlen(Str);

    // ֱ��ʹ��user_debug_uart_write���������ַ���
    user_debug_uart_write(Str, strLen);
}
void ring_init()
{
    debug_uart_rxclear();
    debug_uart_txclear();
    ring_buf_init(&debug_rbsend, debug_txbuf, sizeof(debug_txbuf)); /*��ʼ�����λ����� */
    ring_buf_init(&debug_rbrecv, debug_rxbuf, sizeof(debug_rxbuf));
}

#endif

