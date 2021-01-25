/**
*********************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      dataTrans_uart.h
* @brief    This file provides uart transport layer driver for dataTrans.
* @details
* @author    elliot chen
* @date      2017-04-28
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef __DATATRANS_UART_H
#define __DATATRANS_UART_H

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"

/* Defines ------------------------------------------------------------------*/

/**
 * @brief print log or not
 */

#ifdef PRINT_DATATRANS_UART_LOG
#define DATATRANS_UART_DBG_BUFFER(MODULE, LEVEL, pFormat, para_num,...) DBG_BUFFER_##LEVEL(TYPE_BEE2, SUBTYPE_FORMAT, MODULE, pFormat, para_num, ##__VA_ARGS__)
#else
#define DATATRANS_UART_DBG_BUFFER(MODULE, LEVEL, pFormat, para_num,...) ((void)0)
#endif

/**  @brief IO subtype definitions for @ref IO_MSG_TYPE_UART type */
typedef enum
{
    IO_MSG_UART_RX_PKT         = 1, /**< UART RX data event */
    IO_MSG_UART_RETRANS        = 2, /**< UART retransmission event*/
    IO_MSG_UART_RETRANS_FAIL   = 3, /**< UART retransmission event*/
} T_IO_MSGUART;

/**
 * @brief dataTrans UART ExportedFunctions
 */

void DataTrans_Board_UART_Init(uint8_t rx_pin_num, uint8_t tx_pin_num);
void DataTrans_Driver_UART_Init(void);
void DataTrans_Driver_UART_DeInit(void);
void DataTrans_uart_send(uint8_t *pbuf, uint16_t len);
#ifdef __cplusplus
}
#endif

#endif /*__DATATRANS_UART_H*/

/******************* (C) COPYRIGHT 2017 Realtek Semiconductor Corporation *****END OF FILE****/


