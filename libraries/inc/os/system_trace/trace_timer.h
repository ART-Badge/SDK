/****************************************************************************************************//**
 * @file     trace_timer.h
 *
 * @brief
 *
 * @version  v0.1
 * @date     2018-11-05
 *
 * @note
 *******************************************************************************************************/
#ifndef _TRACE_TIMER_H_
#define _TRACE_TIMER_H_

#include "trace_common.h"
#include "trace_port_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _TIMER_INFO
{
    uint16_t curr_used_num;
    uint16_t minimum_ever_remain_num;
} TIMER_INFO;

extern bool trace_timer_init(void);
extern void trace_timer(void);

#ifdef __cplusplus
}
#endif

#endif /*_TRACE_TIMER_H_*/

/******************* (C) COPYRIGHT 2015 Realtek Semiconductor Corporation *****END OF FILE****/
