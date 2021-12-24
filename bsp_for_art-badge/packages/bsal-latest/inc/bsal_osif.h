/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-28     Supperthomas the first version
 */


#ifndef __BSAL_OSIF_H__
#define __BSAL_OSIF_H__
#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <stdint.h>
#include <rtthread.h>
/**
* BSAL OSIF malloc the buffer
*
* @param len the buffer size of the buffer
* @Note  malloc the buffer
*/
void *bsal_osif_malloc(uint32_t len);

/**
* BSAL OSIF free the buffer
*
* @param p the point of the buffer
* @Note  free the buffer
*/
void bsal_osif_free(void *p);

/**
* BSAL OSIF delay
*
* @param ms the ms of delay time
* @Note  delay the time
*/
void bsal_osif_delay(uint32_t ms);



#if 0
int bsal_osif_printf_dbg(const char *fmt, ...);
int bsal_osif_printf_err(const char *fmt, ...);
int bsal_osif_printf_info(const char *fmt, ...);
int bsal_osif_printf_warn(const char *fmt, ...);
#else
#define bsal_osif_printf_dbg rt_kprintf
#define bsal_osif_printf_err rt_kprintf
#define bsal_osif_printf_info rt_kprintf
#define bsal_osif_printf_warn rt_kprintf

#endif

#ifdef BSAL_ASSERT_DEBUG

#define BSAL_ASSERT_PTR(x)        if(x==NULL) \
    {bsal_osif_printf_err("%s line:%d: the point is NULL:%p",__FUNCTION__, __LINE__,x); while(1);}
#else
#define BSAL_ASSERT_PTR(x)
#endif
#ifdef __cplusplus
}
#endif
#endif
