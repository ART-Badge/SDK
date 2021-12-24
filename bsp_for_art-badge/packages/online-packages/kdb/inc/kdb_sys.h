/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-06-10     tyx          first version.
 */

#ifndef __KDB_SYS_H__
#define __KDB_SYS_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KDB_TYPE_PRINT            (0x20)

#define KDB_TYPE_SCHEDULER        (0x10)
#define KDB_TYPE_THREAD_SUSPEND   (0x11)
#define KDB_TYPE_THREAD_RESUME    (0x12)
#define KDB_TYPE_THREAD_INITED    (0x13)
#define KDB_TYPE_MALLOC           (0x14)
#define KDB_TYPE_FREE             (0x15)
#define KDB_TYPE_MP_ALLOC         (0x16)
#define KDB_TYPE_MP_FREE          (0x17)
#define KDB_TYPE_OBJECT_ATTACH    (0x18)
#define KDB_TYPE_OBJECT_DETACH    (0x19)
#define KDB_TYPE_OBJECT_TRYTAKE   (0x1A)
#define KDB_TYPE_OBJECT_TAKE      (0x1B)
#define KDB_TYPE_OBJECT_PUT       (0x1C)
#define KDB_TYPE_TIMER_TIMEOUT    (0x1D)
#define KDB_TYPE_INTERRUPT        (0x1E)
#define KDB_TYPE_THREAD_IDLE      (0x1F)

#if !defined(KDB_SYS_MEM_CUSTOM) && !defined(KDB_SYS_BUFF_SIZE)
#define KDB_SYS_BUFF_SIZE         2048
#endif

#ifndef KDB_SYS_NAME_LEN
#define KDB_SYS_NAME_LEN          4
#endif

#define KDB_PRINT_BUFF_SIZE       128

struct kdb_thread_info
{
    char name[KDB_SYS_NAME_LEN];
};

struct kdb_object_info
{
    char name[KDB_SYS_NAME_LEN];
    rt_uint8_t type;
    rt_uint8_t flag;
};

struct kdb_mem_info
{
    void *ptr;
    rt_size_t size;
};

struct kdb_memp_info
{
    char name[KDB_SYS_NAME_LEN];
    void *ptr;
};

struct kdb_timer_info
{
    char name[KDB_SYS_NAME_LEN];
    rt_uint32_t flags;
};

struct kdb_scheduler_info
{
    struct kdb_thread_info from;
    struct kdb_thread_info to;
};

struct kdb_interrupt_info
{
    rt_bool_t enter;
};

extern struct kdb *sys_kdb;
extern rt_uint8_t *sys_kdb_buff;
extern rt_uint32_t sys_kdb_buff_size;

void kdb_printf(const char *fmt, ...);
void kdb_printf_output(const char *fmt, ...);
void kdb_type_rtthread_register(void);
int kdb_sys_init(void);
void kdb_sys_start(void); 

void kdb_sys_stop(void);
void kdb_sys_push(rt_uint16_t type, void *buff, rt_uint16_t len);
void kdb_sys_dump(void);
void kdb_sys_stop_dump(void);
rt_uint32_t kdb_sys_ccode(void);
int kdb_sys_isvalid(void);

#ifdef __cplusplus
}
#endif

#endif
