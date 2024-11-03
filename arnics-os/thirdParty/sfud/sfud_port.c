/*
 * This file is part of the Serial Flash Universal Driver Library.
 *
 * Copyright (c) 2016-2018, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2016-04-23
 */
#include <stdarg.h>
#include "../../Inc/projDefine.h"
#include "inc/sfud.h"
#include "../../drivers/driversInclude.h"
#include "../../port/portInclude.h"  //delay.h


typedef struct 
{
    device_t* cs_ds;
    device_t* spi_ds;
    /* data */
}w25q_device_t;

w25q_device_t w25q_dev = 
{
    .cs_ds = &w25q_cs_ds,
    .spi_ds = &w25q_spi_ds
};

/* about 1 microsecond delay */
static void retry_delay_1ms(void)
{
    Delay_ms(1);
}

/**
 * SPI write data then read data using STM32L4 HAL Library.
 */

static sfud_err spi_write_read(const sfud_spi *spi, 
                                const uint8_t *write_buf, 
                                size_t write_size, 
                                uint8_t *read_buf,
                                size_t read_size) 
{
    sfud_err result = SFUD_SUCCESS;
    int state = 0;
    int timeout = 0;

    w25q_device_t *w25q =  (w25q_device_t *)spi->user_data;
    /* reset cs pin */
    dev_ctl(w25q->cs_ds,IO_RESET,NULL);
    if (write_size) {
        dev_write(w25q->spi_ds,(uint8_t *)write_buf, write_size);
        timeout = 0;
        while (dev_ctl(w25q->spi_ds,SPI_GETSATATE,NULL) != HAL_SPI_STATE_READY)
        {
            Delay_ms(10);
            if(timeout > 10)
            {
                break;
            }
            timeout++;
        }
    }
    if (state != 0) {
        goto __exit;
    }
    if (read_size) {
        memset((uint8_t *)read_buf, 0xFF, read_size);
        state = dev_read(w25q->spi_ds,read_buf,read_size);
        timeout = 0;
        while (dev_ctl(w25q->spi_ds,SPI_GETSATATE,NULL)!= HAL_SPI_STATE_READY)
        {
            Delay_ms(10);
            if(timeout > 10)
            {
                break;
            }
            timeout++;
        }
    }
    // Check for errors
    if (state != 0) {
        goto __exit;
    }

__exit:
    // 取消选择 SPI 设备
    /* set cs pin */
    dev_ctl(w25q->cs_ds,IO_SET,NULL);
    return result;

}


#ifdef SFUD_USING_QSPI
/**
 * read flash data by QSPI
 */
static sfud_err qspi_read(const struct __sfud_spi *spi, uint32_t addr, sfud_qspi_read_cmd_format *qspi_read_cmd_format,
        uint8_t *read_buf, size_t read_size) {
    sfud_err result = SFUD_SUCCESS;

    /**
     * add your qspi read flash data code
     */

    return result;
}
#endif /* SFUD_USING_QSPI */

static char log_buf[256];

void sfud_log_debug(const char *file, const long line, const char *format, ...);

static void spi_lock(const sfud_spi *spi)
{

}
static void spi_unlock(const sfud_spi *spi)
{
}

sfud_err sfud_spi_port_init(sfud_flash *flash) {
    sfud_err result = SFUD_SUCCESS;

    /**
     * add your port spi bus and device object initialize code like this:
     * 1. rcc initialize
     * 2. gpio initialize
     * 3. spi device initialize
     * 4. flash->spi and flash->retry item initialize
     *    flash->spi.wr = spi_write_read; //Required
     *    flash->spi.qspi_read = qspi_read; //Required when QSPI mode enable
     *    flash->spi.lock = spi_lock; 
     *    flash->spi.unlock = spi_unlock;
     *    flash->spi.user_data = &spix;
     *    flash->retry.delay = null;
     *    flash->retry.times = 10000; //Required
     */
    dev_reg("w25q_cs",w25q_dev.cs_ds);
    dev_open(w25q_dev.cs_ds);
    dev_reg("w25q_spi",w25q_dev.spi_ds);
    dev_open(w25q_dev.spi_ds);

    switch (flash->index)
    {
        case SFUD_W25Q64_DEVICE_INDEX:
        {
            /* set the interfaces and data */
            flash->spi.wr = spi_write_read;
            flash->spi.lock = spi_lock;
            flash->spi.unlock = spi_unlock;
            flash->spi.user_data = &w25q_dev;
            flash->retry.delay = retry_delay_1ms;
            flash->retry.times =  1000;

            break;
        }
    }
    return result;
}

/**
 * This function is print debug info.
 *
 * @param file the file which has call this function
 * @param line the line number which has call this function
 * @param format output format
 * @param ... args
 */
void sfud_log_debug(const char *file, const long line, const char *format, ...) {
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    printf("[SFUD](%s:%ld) ", file, line);
    /* must use vprintf to print */
    vsnprintf(log_buf, sizeof(log_buf), format, args);
    printf("%s\n", log_buf);
    va_end(args);
}

/**
 * This function is print routine info.
 *
 * @param format output format
 * @param ... args
 */
void sfud_log_info(const char *format, ...) {
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    printf("[SFUD]");
    /* must use vprintf to print */
    vsnprintf(log_buf, sizeof(log_buf), format, args);
    printf("%s\n", log_buf);
    va_end(args);
}
