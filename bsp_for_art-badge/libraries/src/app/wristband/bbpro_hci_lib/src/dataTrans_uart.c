/**
*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file     dataTrans_uart.c
* @brief    This file provides uart transport layer driver.
* @details
* @author   elliot_chen
* @date     2018-11-16
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "dataTrans_uart.h"
#include "dataTrans_queue.h"
#include "os_msg.h"
#include "rtl876x_pinmux.h"
#include "rtl876x_rcc.h"
#include "rtl876x_uart.h"
#include "rtl876x_nvic.h"
#include "os_timer.h"
#include "board.h"

#define ENTER_DLPS_TIMER_INTERVAL           10000

static void  *allowed_enter_dlps_timer = NULL;
extern bool  allowed_bbpro_hci_enter_dlps;

/* Internal defines ----------------------------------------------------------*/
#define         UART_RX_TRIGGER_VALUE           14

/* Globals -------------------------------------------------------------------*/

/* Function declaration ------------------------------------------------------*/
void DataTrans_UARTIntrHandler(void);


static void allowed_enter_dlps_callback(void *pxTimer)
{
    allowed_bbpro_hci_enter_dlps = true;
}
/**
  * @brief  initialization of pinmux settings and pad settings.
  * @param   No parameter.
  * @return  void
  */
void DataTrans_Board_UART_Init(uint8_t rx_pin_num, uint8_t tx_pin_num)
{
    Pad_Config(rx_pin_num, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_HIGH);
    Pad_Config(tx_pin_num, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,
               PAD_OUT_HIGH);

    Pinmux_Config(rx_pin_num, BBPRO_UART_FUNC_RX);
    Pinmux_Config(tx_pin_num, BBPRO_UART_FUNC_TX);
}

/**
  * @brief  Initializes UART peripheral.
  * @param  None.
  * @retval None.
  */
void DataTrans_Driver_UART_Init(void)
{
    /* Initialize queue */
    //DataTrans_InitQueue();

    /* Modify UART interrupt handle */
    RamVectorTableUpdate(BBPRO_UART_VECTORn, DataTrans_UARTIntrHandler);
    /* Enable UART clock */
    RCC_PeriphClockCmd(BBPRO_UART_APBPeriph, BBPRO_UART_APBClock, ENABLE);
    /* Initialize UART */
    UART_InitTypeDef uartInitStruct;
    UART_StructInit(&uartInitStruct);
    /* Baudrate = 115200 */
    uartInitStruct.UART_Div              = 20;
    uartInitStruct.UART_Ovsr             = 12;
    uartInitStruct.UART_OvsrAdj         = 0x252;
    uartInitStruct.UART_RxThdLevel   = UART_RX_TRIGGER_VALUE;
    uartInitStruct.UART_IdleTime        = UART_RX_IDLE_2BYTE;//idle interrupt wait time
    UART_Init(BBPRO_UART_BUS, &uartInitStruct);
    UART_INTConfig(BBPRO_UART_BUS, UART_INT_RD_AVA | UART_INT_RX_LINE_STS, ENABLE);
    /* Register callback function */
    DataTrans_RegisterPeriphReadFnCB((WriteQueueFnCB)UART_ReceiveData);

    /* NVIC init */
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = BBPRO_UART_IRQ;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    os_timer_create(&allowed_enter_dlps_timer, "bbpro hci check dlps timer", 1,
                    ENTER_DLPS_TIMER_INTERVAL,
                    false, allowed_enter_dlps_callback);
}

void DataTrans_Driver_UART_DeInit(void)
{
    RCC_PeriphClockCmd(BBPRO_UART_APBPeriph, BBPRO_UART_APBClock, DISABLE);
}

void DataTrans_uart_send(uint8_t *pbuf, uint16_t len)
{
    uint32_t i = 0;

    for (i = 0; i < (len / UART_TX_FIFO_SIZE); i++)
    {
        UART_SendData(BBPRO_UART_BUS, pbuf, UART_TX_FIFO_SIZE);
        while (UART_GetFlagStatus(BBPRO_UART_BUS, UART_FLAG_TX_EMPTY) != SET);
        pbuf += UART_TX_FIFO_SIZE;
    }

    UART_SendData(BBPRO_UART_BUS, pbuf, len % UART_TX_FIFO_SIZE);
    while (UART_GetFlagStatus(BBPRO_UART_BUS, UART_FLAG_TX_EMPTY) != SET);
    allowed_bbpro_hci_enter_dlps = false;
    os_timer_restart(&allowed_enter_dlps_timer, ENTER_DLPS_TIMER_INTERVAL);
}


/**
  * @brief  Data UART interrupt handle.
  * @param  None.
  * @retval None.
  */
void DataTrans_UARTIntrHandler(void)
{
    /* Read interrupt type */
    uint32_t int_status = UART_GetIID(BBPRO_UART_BUS);

    /* Disable interrupt */
    UART_INTConfig(BBPRO_UART_BUS, UART_INT_RD_AVA | UART_INT_RX_LINE_STS, DISABLE);

    /* Interrupt handle */
    if (int_status == UART_INT_ID_RX_LEVEL_REACH)
    {
        DataTrans_WriteQueue(BBPRO_UART_BUS, UART_RX_TRIGGER_VALUE);
    }
    else if (int_status == UART_INT_ID_RX_DATA_TIMEOUT)
    {
        while (UART_GetFlagStatus(BBPRO_UART_BUS, UART_FLAG_RX_DATA_AVA) == SET)
        {
            DataTrans_WriteQueue(BBPRO_UART_BUS, 1);
        }
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
            UART_GetFlagStatus(BBPRO_UART_BUS, UART_FLAG_RX_OVERRUN);
            UART_ClearRxFIFO(BBPRO_UART_BUS);
            DATATRANS_UART_DBG_BUFFER(MODULE_APP, LEVEL_INFO, "line status error!", 0);
            /* Enable interrupt */
            UART_INTConfig(BBPRO_UART_BUS, UART_INT_RD_AVA | UART_INT_RX_LINE_STS, ENABLE);
            return ;
        }
    }

    extern void *bbppro_hci_receive_queue_handle;
    uint8_t event = IO_MSG_UART_RX_PKT;
    if (os_msg_send(bbppro_hci_receive_queue_handle, &event, 0) == false)
    {
        APP_PRINT_ERROR0("[talkband]bbppro hci receive queue send fail");
    }
    else
    {
        //APP_PRINT_ERROR0("[talkband]bbppro hci receive queue send success");
        os_timer_restart(&allowed_enter_dlps_timer, ENTER_DLPS_TIMER_INTERVAL);
    }

    /* Enable interrupt */
    UART_INTConfig(BBPRO_UART_BUS, UART_INT_RD_AVA | UART_INT_RX_LINE_STS, ENABLE);
}

/******************* (C) COPYRIGHT 2017 Realtek Semiconductor Corporation *****END OF FILE****/

