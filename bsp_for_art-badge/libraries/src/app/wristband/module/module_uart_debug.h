/**
*********************************************************************************************************
*               Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     module_uart_debug.h
* @brief
* @details
* @author   howie
* @date
* @version  v0.1
*********************************************************************************************************
*/

#ifndef _MODULE_UART_DEBUG_H_
#define _MODULE_UART_DEBUG_H_
#include "rtl876x_gpio.h"
#include "rtl876x_rcc.h"
#include "rtl876x_tim.h"
#include "rtl876x_nvic.h"
#include "rtl876x_pinmux.h"
#include "rtl876x.h"
#include "trace.h"
#include "hub_task.h"


typedef struct
{
    uint8_t *buf;
    uint32_t read_index;
    uint32_t write_index;
    uint8_t *at_cmd_buf;
} T_DEBUG_PACKAGE;

void uart_debug_init(void);
void uart_debug_enter_dlps(void);
void uart_debug_exit_dlps(void);
void uart_debug_event_handler(T_IO_MSG msg);
void uart_debug_buf_reset(void);

extern T_DEBUG_PACKAGE raw_package;

#endif //_MODULE_UART_DEBUG_H_


