#include "ring.h"
unsigned int ring_write(ring_buf_t *rbsend, void *buf, unsigned int len)
{
    unsigned int ret;
    ret = ring_buf_put(rbsend, (unsigned char *)buf, len);
    // enable__uart_IT_TXE();  need this operation
    return ret;
}
unsigned int ring_read(ring_buf_t *rbrev, void *buf, unsigned int len)
{
    return ring_buf_get(rbrev, (unsigned char *)buf, len);
}

void ring_rxclear(ring_buf_t *rbrev)
{
    ring_buf_clr(rbrev);
}
void ring_txclear(ring_buf_t *rbsend)
{
    ring_buf_clr(rbsend);
}

void ring_init(ring_buf_t *rbsend,unsigned char *bufsend,ring_buf_t *rbrev,unsigned char *bufrev)
{
    /*初始化环形缓冲区 */
    ring_buf_init(rbsend, bufsend, sizeof(bufsend)); 
    ring_buf_init(rbrev, bufrev, sizeof(bufrev));
}



