/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     module_uart_debug.c
* @brief
* @details
* @author   howie
* @date
* @version  v0.1
*********************************************************************************************************
*/
#include "board.h"
#include "string.h"
#include "os_timer.h"
#include "os_sched.h"
#include "hub_task.h"
#include "rtl876x_pinmux.h"
#include "rtl876x_uart.h"
#include "module_uart_debug.h"
#include "stdlib.h"
#include "hub_uart_debug.h"

#define ENTER_DLPS_TIMER_INTERVAL           10000
#define UART_DEBUG_RX_TRIGGER_VALUE           14


static void  *allowed_enter_dlps_timer = NULL;
extern bool allowed_uart_debug_enter_dlps;

//uint8_t *raw_package.buf = NULL;
//static uint32_t raw_package.write_index = 0;

T_DEBUG_PACKAGE raw_package;


void DEBUG_UARTIntrHandler(void);


static void allowed_enter_dlps_callback(void *pxTimer)
{
    allowed_uart_debug_enter_dlps = true;
}

void uart_debug_buf_reset(void)
{
    raw_package.write_index = 0;
    raw_package.read_index = 0;
    memset(raw_package.buf, 0x00, RECEIVE_BUF_MAX_LENGTH);
}

void uart_debug_init(void)
{
    raw_package.buf = malloc(RECEIVE_BUF_MAX_LENGTH * sizeof(uint8_t));//todo
    raw_package.write_index = 0;
    raw_package.read_index = 0;
    memset(raw_package.buf, 0x00, RECEIVE_BUF_MAX_LENGTH);
    Pad_Config(UART_DEBUG_TX, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_HIGH);
    Pad_Config(UART_DEBUG_RX, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,
               PAD_OUT_HIGH);

    Pinmux_Config(UART_DEBUG_TX, UART0_TX);
    Pinmux_Config(UART_DEBUG_RX, UART0_RX);


    /* Modify UART interrupt handle */
    RamVectorTableUpdate(Uart0_VECTORn, DEBUG_UARTIntrHandler);
    /* Enable UART clock */
    RCC_PeriphClockCmd(APBPeriph_UART0, APBPeriph_UART0_CLOCK, ENABLE);
    /* Initialize UART */
    UART_InitTypeDef uartInitStruct;
    UART_StructInit(&uartInitStruct);
    /* Baudrate = 115200 */
    uartInitStruct.UART_Div         = 20;
    uartInitStruct.UART_Ovsr        = 12;
    uartInitStruct.UART_OvsrAdj     = 0x252;
    uartInitStruct.UART_RxThdLevel  = UART_DEBUG_RX_TRIGGER_VALUE;
    uartInitStruct.UART_IdleTime    = UART_RX_IDLE_2BYTE;//idle interrupt wait time
    UART_Init(DEBUG_UART, &uartInitStruct);
    UART_INTConfig(DEBUG_UART, UART_INT_RD_AVA | UART_INT_RX_LINE_STS, ENABLE);

    /* NVIC init */
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = UART0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    os_timer_create(&allowed_enter_dlps_timer, "debug uart check dlps timer", 1,
                    ENTER_DLPS_TIMER_INTERVAL,
                    false, allowed_enter_dlps_callback);

    System_WakeUpPinEnable(UART_DEBUG_TX, PAD_WAKEUP_POL_LOW, PAD_WK_DEBOUNCE_DISABLE);
}



void uart_debug_enter_dlps(void)
{
    Pad_Config(UART_DEBUG_TX, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(UART_DEBUG_RX, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
}

void uart_debug_exit_dlps(void)
{
    Pad_Config(UART_DEBUG_TX, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_HIGH);
    Pad_Config(UART_DEBUG_RX, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,
               PAD_OUT_HIGH);
}

void DEBUG_UARTIntrHandler(void)
{
    /* Read interrupt type */
    uint32_t int_status = UART_GetIID(DEBUG_UART);

    /* Disable interrupt */
    UART_INTConfig(DEBUG_UART, UART_INT_RD_AVA | UART_INT_RX_LINE_STS, DISABLE);

    /* Interrupt handle */
    if (int_status == UART_INT_ID_RX_LEVEL_REACH)
    {
        UART_ReceiveData(DEBUG_UART, raw_package.buf + raw_package.write_index,
                         UART_DEBUG_RX_TRIGGER_VALUE);
        raw_package.write_index = raw_package.write_index + UART_DEBUG_RX_TRIGGER_VALUE;
    }
    else if (int_status == UART_INT_ID_RX_DATA_TIMEOUT)
    {
        while (UART_GetFlagStatus(DEBUG_UART, UART_FLAG_RX_DATA_AVA) == SET)
        {
            *(raw_package.buf + raw_package.write_index) = UART_ReceiveByte(DEBUG_UART);
            raw_package.write_index++;
        }
        //APP_PRINT_INFO2("[DEBUG CMD] buf[0] = %d;buf[1] = %d", raw_package.buf[0], raw_package.buf[1]);
    }
    else if (int_status == UART_INT_ID_TX_EMPTY)
    {
        /* Tx FIFO empty, not enable */
        return;
    }
    else
    {
        /* Receive line status interrupt */
        if (int_status == UART_INT_ID_LINE_STATUS)
        {
            UART_GetFlagStatus(DEBUG_UART, UART_FLAG_RX_OVERRUN);
            UART_ClearRxFIFO(DEBUG_UART);
            /* Enable interrupt */
            UART_INTConfig(DEBUG_UART, UART_INT_RD_AVA | UART_INT_RX_LINE_STS, ENABLE);
            return ;
        }
    }

    T_IO_MSG msg;
    msg.type = HUB_MSG_UART_DEBUG;
    msg.subtype = UART_DEBUG_MSG_INTERRUPT;
    if (send_msg_to_ui_task(&msg, __LINE__) == false)
    {
        APP_PRINT_ERROR0("send uart cmd to io queue fail !\n");
    }
    else
    {
        os_timer_restart(&allowed_enter_dlps_timer, ENTER_DLPS_TIMER_INTERVAL);
    }

    /* Enable interrupt */
    UART_INTConfig(DEBUG_UART, UART_INT_RD_AVA | UART_INT_RX_LINE_STS, ENABLE);
}


