/****************************************************************************************************//**
 * @file     trace_stack.h
 *
 * @brief
 *
 * @version  v0.1
 * @date     2018-11-05
 *
 * @note
 *******************************************************************************************************/
#ifndef _TRACE_STACK_H_
#define _TRACE_STACK_H_

#include "trace_common.h"
#include "trace_port_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _TASK_STACK_INFO_BLOCK
{
    uint32_t task_id;
    uint32_t minimum_ever_remain_size;
    char task_name[configMAX_TASK_NAME_LEN];
} TASK_STACK_INFO_BLOCK_t;


typedef struct _STACK_INFO
{
    uint32_t task_number;
    TASK_STACK_INFO_BLOCK_t task_stack_info[MAX_TASK_COUNT_FOR_DEBUG];
    uint32_t main_stack_minimum_ever_remain_size;
} STACK_INFO;

extern void trace_task_stack(void);

#ifdef __cplusplus
}
#endif

#endif /*_TRACE_STACK_H_*/

/******************* (C) COPYRIGHT 2015 Realtek Semiconductor Corporation *****END OF FILE****/
