/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2020-06-29     supperthomas  first version
 */

#ifndef __DRV_USART_H__
#define __DRV_USART_H__

#include <rtthread.h>
#include "rtdevice.h"
#include <rthw.h>
#include "board.h"

struct rtl_uart_config
{
    const char *name;
    UART_TypeDef *Instance;
    IRQn_Type irq_type;
    uint8_t tx_pin;
    uint8_t rx_pin;
    uint8_t tx_pin_func;
    uint8_t rx_pin_func;
    uint32_t periph;
    uint32_t periph_clock;
};

struct rtl_uart
{
    UART_InitTypeDef *handle;
    struct rtl_uart_config *config;
    rt_size_t last_index;
    rt_uint16_t uart_dma_flag;
    struct rt_serial_device serial;
};

typedef enum
{
    RTL_BAUD_RATE_9600,
    RTL_BAUD_RATE_19200,
    RTL_BAUD_RATE_115200,
    RTL_BAUD_RATE_230400,
    RTL_BAUD_RATE_460800,
    RTL_BAUD_RATE_921600,
    RTL_BAUD_RATE_1000000,
    RTL_BAUD_RATE_2000000,
    RTL_BAUD_RATE_3000000,
    RTL_BAUD_RATE_4000000,
    RTL_BAUD_RATE_6000000
} UartBaudRate_TypeDef;

typedef struct
{
    uint16_t div;
    uint16_t ovsr;
    uint16_t ovsr_adj;
} UART_BaudRate_TypeDef;

#define UART0_CONFIG                                               \
    {                                                              \
        .name = "uart0",                                           \
        .Instance = UART0,                                         \
        .irq_type = UART0_IRQn,                                    \
        .tx_pin = P3_0,                                            \
        .rx_pin = P3_1,                                            \
        .tx_pin_func = UART0_TX,                                   \
        .rx_pin_func = UART0_RX,                                   \
        .periph = APBPeriph_UART0,                                 \
        .periph_clock = APBPeriph_UART0_CLOCK,                     \
    }
#define UART1_CONFIG                                               \
    {                                                              \
        .name = "uart1",                                           \
        .Instance = UART1,                                         \
        .irq_type = UART1_IRQn,                                    \
        .tx_pin = P4_0,                                            \
        .rx_pin = P4_1,                                            \
        .tx_pin_func = UART1_TX,                                   \
        .rx_pin_func = UART1_RX,                                   \
        .periph = APBPeriph_UART1,                                 \
        .periph_clock = APBPeriph_UART1_CLOCK,                     \
    }

int rt_hw_usart_init(void);

#endif  /* __DRV_USART_H__ */

/******************* end of file ***************/
