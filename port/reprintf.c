#include "Inc/projDefine.h"
#include "Inc/typedef.h"
#include "arnicsPort.h"
#include "drivers/driversInclude.h"
#ifdef STM32CHIP

/*
今天我用到RTX里面使用printf ，发现程序死掉了

我发现很多人遇到了这样的问题

找了网上很多的文章，说是这个是RTX的一个先天不足的问题

我发现了正点原子的 原子哥的解决方案，如下所示：

-------------------------------------------------------------------------------

产生原因：

   使用库里的printf函数在输出结束后会调用__used void _mutex_acquire (OS_ID *mutex)这个方法，

   但是传入的参数mutex并未进行初始化，所有内存访问地址出错。造成hardfault
*/

// 加入以下代码,支持printf函数,而不需要选择use MicroLIB	         
// 标准库需要的支持函数                 
// 定义无半主机模式
#pragma import(__use_no_semihosting)
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;   
// 重定义 _sys_exit 函数
void _sys_exit(int x) {
    (void)x;  // 避免未使用警告
}

// 重定义 _ttywrch 函数
int _ttywrch(int ch) {
    // 实现你的输出逻辑，例如通过 UART 发送字符
    uart_dirct_t bufcache = {0};
    bufcache.buf = (uint8_t *)&ch;
    bufcache.size = 1;
    dev_ctl(&debug_ds, UART_DSEND, &bufcache);
    return ch;
}

// 重定义 fputc 函数
int fputc(int ch, FILE *stream) {
    return _ttywrch(ch);
}

#endif 



