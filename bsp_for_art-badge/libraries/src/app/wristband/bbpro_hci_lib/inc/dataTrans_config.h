/**
*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     dataTrans_config.h
* @brief
* @details
* @author   elliot chen
* @date     2018-11-16
* @version  v1.0
*********************************************************************************************************
*/

#ifndef __DATATRANS_HCI_CONFIG_H
#define __DATATRANS_HCI_CONFIG_H

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stdbool.h"
#include "stdint.h"
#include "string.h"
#include "rtl876x.h"
#include "rtl876x_bitfields.h"
#include "os_mem.h"
#include "rtl876x_pinmux.h"
#include "rtl876x_rcc.h"
#include "rtl876x_uart.h"
#include "rtl876x_tim.h"
#include "rtl876x_adc.h"
#include "rtl876x_gpio.h"
#include "rtl876x_nvic.h"
#include "trace.h"
#include "app_msg.h"

/* Defines ------------------------------------------------------------------*/

/**
 * @brief Configure loop queue parameters
 */

#define LOOP_QUEUE_MAX_SIZE             (1024)
#define QUEUE_CAPABILITY                (LOOP_QUEUE_MAX_SIZE-1)

/**
  * @brief  HCI packet payload length.
    Please modified by actual packet length if need.
  */

#define HCI_CMD_PACK_PAYLOAD_MAX_SIZE       (50)

/**
  * @brief HCI protocol retransmit detection mechanism.
  */
#define HCI_RETREANS_DETECTION
#define HCI_RETREANS_TIMEOUT        300
#define HCI_RETRANS_COUNT           3

//#define PRINT_DATATRANS_UART_LOG
//#define PRINT_DATATRANS_HCI_LOG
#define PRINT_DATATRANS_QUEUE_LOG

/* Configure OLT test mode dynamic detection*/
//DYNAMIC_SWITCH_MODE

typedef T_IO_MSG   BEE_IO_MSG;
typedef unsigned char   BOOL;
typedef long BaseType_t;
typedef void *plt_timer_t;

#define pdFALSE         ( ( BaseType_t ) 0 )
#define pdTRUE          ( ( BaseType_t ) 1 )
#define pdPASS          ( pdTRUE )
#define pdFAIL          ( pdFALSE )
#define portBASE_TYPE   long

#ifndef FALSE
#define FALSE           0
#endif

#ifndef TRUE
#define TRUE            1
#endif

#define UART_RX_TRIGGER_VALUE           14

/**
  * @brief Function pointer type used by interrupt handle to send events to application.
  */
typedef bool (*pfnSendMsgFromIntrHandlerCB)(T_IO_MSG *p_msg);

#ifdef __cplusplus
}
#endif

#endif /* __DATATRANS_HCI_CONFIG_H */

/******************* (C) COPYRIGHT 2018 Realtek Semiconductor Corporation *****END OF FILE****/

