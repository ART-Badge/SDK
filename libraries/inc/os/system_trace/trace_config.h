/****************************************************************************************************//**
 * @file     trace_config.h
 *
 * @brief
 *
 * @version  v0.1
 * @date     2018-11-05
 *
 * @note
 *******************************************************************************************************/
#ifndef _TRACE_CONFIG_H_
#define _TRACE_CONFIG_H_

#define TRACE_HEAP_EN                           1
#define TRACE_STACK_EN                          1
#define TRACE_TIMER_EN                          1
#define DEBUG_TASK_HANG_EN                      1
#define DEBUG_DLPS_ERROR_EN                     1

/******************* CONFIGURATION *******************/
#define TRACE_PERIOD_TIME                       ( 10 * 1000 )

/******************* QUEUE CONFIGURATION *******************/
#define QUEUE_OBJECTS_NUM                       ( 3 )
#define TRACE_QUEUE_PERIOD_TIME                 ( 10 * 1000 )

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /*_TRACE_CONFIG_H_*/

/******************* (C) COPYRIGHT 2015 Realtek Semiconductor Corporation *****END OF FILE****/
