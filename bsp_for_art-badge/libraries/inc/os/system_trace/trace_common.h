/****************************************************************************************************//**
 * @file     trace_common.h
 *
 * @brief
 *
 * @version  v0.1
 * @date     2018-11-05
 *
 * @note
 *******************************************************************************************************/
#ifndef _TRACE_COMMON_H_
#define _TRACE_COMMON_H_

#include <stdint.h>
#include <stdbool.h>

#ifndef NULL
#define NULL (void *)0
#endif

#define portSTACK_TYPE  uint32_t
#define portBASE_TYPE   long

//typedef uint32_t size_t;
typedef portSTACK_TYPE StackType_t;
typedef unsigned long UBaseType_t;

typedef struct tskTaskControlBlock
{
    uint32_t *pxTopOfStack;
    uint32_t xGenericListItem[5];
    uint32_t xEventListItem[5];
    uint32_t uxPriority;
    uint32_t *pxStack;
    char pcTaskName[ 12 ];
    uint32_t uxTCBNumber;
    uint32_t uxTaskNumber;
    uint32_t uxBasePriority;
    uint32_t uxMutexesHeld;
} TCB_t;

typedef bool (*BOOL_PATCH_FUNC)();

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /*_TRACE_COMMON_H_*/

/******************* (C) COPYRIGHT 2015 Realtek Semiconductor Corporation *****END OF FILE****/
