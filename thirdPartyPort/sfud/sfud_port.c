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

#ifndef STM32CHIP
static sfud_err spi_write_read(const sfud_spi *spi, const uint8_t *write_buf, size_t write_size, uint8_t *read_buf,
                               size_t read_size) {
    sfud_err result = SFUD_SUCCESS;

    /**
     * add your qspi read flash data code
     */

    return result;                   
}
#else
#include "stm32l4xx_hal.h"
#include "gpio.h"
#include "spi.h"
#include "iwdg.h"


/* about 100 microsecond delay */
static void retry_delay_100us(void)
{
    uint32_t delay = 2400;
    while (delay--);
}

/*---接口定义------------------------------------------------------------------------------------*/
typedef struct
{
    SPI_HandleTypeDef *spix;
    GPIO_TypeDef *cs_gpiox;
    uint16_t cs_gpio_pin;
} spi_user_data, *spi_user_data_t;
/*---------------------------------------------------------------------------------------------*/
/**
 * SPI write data then read data using STM32L4 HAL Library.
 */
static spi_user_data spi_user = { .spix = &hspi1, .cs_gpiox = GPIOB, .cs_gpio_pin = GPIO_PIN_0 };
static sfud_err spi_write_read(const sfud_spi *spi, const uint8_t *write_buf, size_t write_size, uint8_t *read_buf,
                               size_t read_size) {
    sfud_err result = SFUD_SUCCESS;
    spi_user_data_t spi_dev = (spi_user_data_t) spi->user_data;
    HAL_StatusTypeDef state = HAL_OK;
    int timeout = 0;
    IWDG_ReloadCounter(); // 喂狗，防止看门狗复位
    // Check for valid pointers and sizes
    if (write_size) {
        SFUD_ASSERT(write_buf);
    }
    if (read_size) {
        SFUD_ASSERT(read_buf);
    }

    // 选择 SPI 设备
    /* reset cs pin */
    if (spi_dev->cs_gpiox != NULL)
        HAL_GPIO_WritePin(spi_dev->cs_gpiox, spi_dev->cs_gpio_pin, GPIO_PIN_RESET);

    if (write_size) {
        state = HAL_SPI_Transmit(spi_dev->spix, (uint8_t *)write_buf, write_size, 1000);
        timeout = 0;
        while (HAL_SPI_GetState(spi_dev->spix) != HAL_SPI_STATE_READY)
        {
            Delay_ms(100);
            if(timeout > 20)
            {
                break;
            }
            timeout++;
        }
    }

    if (state != HAL_OK) {
        goto __exit;
    }

    if (read_size) {
        memset((uint8_t *)read_buf, 0xFF, read_size);
        state = HAL_SPI_Receive(spi_dev->spix, read_buf, read_size, 1000);
        timeout = 0;
        while (HAL_SPI_GetState(spi_dev->spix) != HAL_SPI_STATE_READY)
        {
            Delay_ms(100);
            if(timeout > 20)
            {
                break;
            }
            timeout++;
        }
    }


    // Check for errors
    if (state != HAL_OK) {
        goto __exit;
    }

__exit:
    // 取消选择 SPI 设备
    /* set cs pin */
    if (spi_dev->cs_gpiox != NULL)
        HAL_GPIO_WritePin(spi_dev->cs_gpiox, spi_dev->cs_gpio_pin, GPIO_PIN_SET);

    return result;
}
#endif
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
    switch (flash->index)
    {
        case SFUD_W25Q128_DEVICE_INDEX:
        {
            /* set the interfaces and data */
            flash->spi.wr = spi_write_read;
            flash->spi.lock = spi_lock;
            flash->spi.unlock = spi_unlock;
            flash->spi.user_data = &spi_user;
            /* about 100 microsecond delay */
            flash->retry.delay = retry_delay_100us;
            /* adout 60 seconds timeout */
            flash->retry.times = 60 * 10000;

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
