#ifndef  RIL_RING_H
#define RIL_RING_H
#include "../Inc/projDefine.h"
#include "../Inc/typedef.h"
#include "../thirdParty/thirdPartyInclude.h"

#ifdef __cplusplus
extern "C" {
#endif
#ifdef _USE_RING_

#define DEBUG_UART_RXBUF_SIZE  512 /*接收缓冲区大小 ------------*/
#define DEBUG_UART_TXBUF_SIZE  512 /*发送缓冲区大小 ------------*/


extern unsigned char debug_rxbuf[DEBUG_UART_RXBUF_SIZE]; /*接收缓冲区 ------------*/
extern unsigned char debug_txbuf[DEBUG_UART_TXBUF_SIZE]; /*发送缓冲区 ------------*/
extern ring_buf_t debug_rbsend, debug_rbrecv;            /*收发缓冲区管理 --------*/



extern unsigned int user_debug_uart_is_len(void);
extern unsigned int user_debug_uart_write(const void *buf, unsigned int len);
extern unsigned int user_debug_uart_read(void *buf, unsigned int len);
extern unsigned int debug_uart_write_byte(unsigned char* byteData);
extern unsigned int debug_uart_read_byte(unsigned char *byteData);
extern void debug_uart_send_String(const char *Str);

extern void ring_init(void);


#endif
#ifdef __cplusplus
}
#endif

#endif // ! RIL_RING_H




