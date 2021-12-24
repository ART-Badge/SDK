/****************************************************************************************************//**
 * @file     trace_port_config.h
 *
 * @brief
 *
 * @version  v0.1
 * @date     2018-11-05
 *
 * @note
 *******************************************************************************************************/
#ifndef _TRACE_PORT_CONFIG_H_
#define _TRACE_PORT_CONFIG_H_

#include "otp.h"
/******************* OS CONFIGURATION  *******************/
#define MAX_TASK_COUNT_FOR_DEBUG                ( 8 )
#define configMAX_TASK_NAME_LEN                 ( 10 )

/******************* HEAP CONFIGURATION *******************/

/* Bee2 has 2 types: data and buffer */
#define TRACE_HEAP_TYPE_NUM                     ( 2 )
#define TRACE_HEAP_MAX_NUMBER_IN_BLOCK_LIST     ( 16 )

/******************* STACK CONFIGURATION *******************/

#define MAIN_STACK_START_ADDRESS                ( 0x00203400 )

/******************* TIMER CONFIGURATION *******************/

#define MAX_TIMER_NUM                           ( otp.os_cfg.timerMaxNumber )
#define TIMER_COUNTER_ADDR                      ( 0x00200f0c )

/******************* QUEUE CONFIGURATION *******************/

#define MAX_QUEUE_OBJECTS_NUM                   ( 8 )

/******************* DLPS CONFIGURATION *******************/
#define LPS_WK_COUNT_ADDR                       ( 0x0020171c )
#define LPS_WK_TIME_ADDR                        ( 0x00201720 )
#define LPS_TOTAL_TIME                          ( 0x00201724 )

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /*_TRACE_PORT_CONFIG_H_*/

/******************* (C) COPYRIGHT 2015 Realtek Semiconductor Corporation *****END OF FILE****/
