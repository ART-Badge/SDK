/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-06-10     tyx          first version.
 */

#include <rthw.h>
#include <string.h>

#include "kdb_core.h"
#include "kdb_type.h"
#include "kdb_sys.h"

#define KDB_PRINT(...) kdb_printf_output(__VA_ARGS__)

#define __KDB2STR(v) #v
#define _KDB2STR(v)  __KDB2STR(v)

extern void kdb_output(char *buf, int len);

void kdb_printf_output(const char *fmt, ...)
{
    va_list args;
    rt_size_t length;
    char log_buf[KDB_PRINT_BUFF_SIZE + 8];

    va_start(args, fmt);
    length = rt_vsnprintf(log_buf, sizeof(log_buf) - 1, fmt, args);
    if (length > KDB_PRINT_BUFF_SIZE - 1)
        length = KDB_PRINT_BUFF_SIZE - 1;
    kdb_output(log_buf, length);
    va_end(args);
}

#ifdef KDB_SCHEDULER_DUMP
static void kdb_scheduler_hook(struct rt_thread* from, struct rt_thread* to)
{
    struct kdb_scheduler_info info;

    strncpy(&info.from.name[0], from->name, sizeof(info.from.name));
    strncpy(&info.to.name[0], to->name, sizeof(info.to.name));

    kdb_sys_push(KDB_TYPE_SCHEDULER, &info, sizeof(struct kdb_scheduler_info));
}

static void kdb_scheduler_dump(kdb_head_t head, void *data)
{
    struct kdb_scheduler_info *info = data;

    KDB_PRINT("[tick]:%d thread scheduler %." _KDB2STR(KDB_SYS_NAME_LEN) \
        "s -> %." _KDB2STR(KDB_SYS_NAME_LEN) "s\r\n",
        head->tick, info->from.name, info->to.name);
}
#endif

#ifdef KDB_THREAD_DUMP

#if defined(KDB_THREAD_SUSPEND_DUMP) || \
    defined(KDB_THREAD_RESUME_DUMP) || \
    defined(KDB_THREAD_INITED_DUMP)
static void kdb_thread_hook(rt_uint16_t type_id, rt_thread_t thread)
{
    struct kdb_thread_info info;

    strncpy(&info.name[0], thread->name, sizeof(info.name));
    kdb_sys_push(type_id, &info, sizeof(struct kdb_thread_info));
}

static void kdb_thread_dump(kdb_head_t head, void *data)
{
    struct kdb_thread_info *info = data;
    char *thread_status;

    switch (head->type)
    {
    case KDB_TYPE_THREAD_SUSPEND:
        thread_status = "suspend";
        break;
    case KDB_TYPE_THREAD_RESUME:
        thread_status = "resume";
        break;
    case KDB_TYPE_THREAD_INITED:
        thread_status = "inited";
        break;
    default:
        thread_status = "unknown";
        break;
    }
    KDB_PRINT("[tick]:%d thread %s %." _KDB2STR(KDB_SYS_NAME_LEN) "s\r\n",
        head->tick, thread_status, info->name);
}
#endif

#ifdef KDB_THREAD_SUSPEND_DUMP
static void kdb_thread_suspend_hook(rt_thread_t thread)
{
    kdb_thread_hook(KDB_TYPE_THREAD_SUSPEND, thread);
}
#endif

#ifdef KDB_THREAD_RESUME_DUMP
static void kdb_thread_resume_hook(rt_thread_t thread)
{
    kdb_thread_hook(KDB_TYPE_THREAD_RESUME, thread);
}
#endif

#ifdef KDB_THREAD_INITED_DUMP
static void kdb_thread_inited_hook(rt_thread_t thread)
{
    kdb_thread_hook(KDB_TYPE_THREAD_INITED, thread);
}
#endif
#endif

#ifdef KDB_HEAP_DUMP

#ifdef KDB_HEAP_MALLOC_DUMP
static void kdb_malloc_hook(void *ptr, rt_size_t size)
{
    struct kdb_mem_info info;

    info.ptr = ptr;
    info.size = size;

    kdb_sys_push(KDB_TYPE_MALLOC, &info, sizeof(struct kdb_mem_info));
}

static void kdb_malloc_dump(kdb_head_t head, void *data)
{
    struct kdb_mem_info *info = data;
    KDB_PRINT("[tick]:%d malloc addr:0x%08x size:%d\r\n",
        head->tick, info->ptr, info->size);
}

#endif

#ifdef KDB_HEAP_FREE_DUMP
static void kdb_free_hook(void *ptr)
{
    struct kdb_mem_info info;

    info.ptr = ptr;
    info.size = 0;

    kdb_sys_push(KDB_TYPE_FREE, &info, sizeof(struct kdb_mem_info));
}

static void kdb_free_dump(kdb_head_t head, void *data)
{
    struct kdb_mem_info *info = data;
    KDB_PRINT("[tick]:%d free addr:0x%08x\r\n", head->tick, info->ptr);
}
#endif
#endif

#ifdef KDB_MP_DUMP
#ifdef KDB_MP_ALLOC_DUMP
static void kdb_mp_alloc_hook(struct rt_mempool* mp, void *ptr)
{
    struct kdb_memp_info info;

    strncpy(&info.name[0], mp->parent.name, sizeof(info.name));
    info.ptr = ptr;
    kdb_sys_push(KDB_TYPE_MP_ALLOC, &info, sizeof(struct kdb_memp_info));
}

static void kdb_mp_alloc_dump(kdb_head_t head, void *data)
{
    struct kdb_memp_info *info = data;

    KDB_PRINT("[tick]:%d mp alloc name:%." _KDB2STR(KDB_SYS_NAME_LEN) \
        "s addr:0x%08x\r\n",
        head->tick, info->name, info->ptr);
}
#endif

#ifdef KDB_MP_FREE_DUMP
static void kdb_mp_free_hook(struct rt_mempool* mp, void *ptr)
{
    struct kdb_memp_info info;

    strncpy(&info.name[0], mp->parent.name, sizeof(info.name));
    info.ptr = ptr;
    kdb_sys_push(KDB_TYPE_MP_FREE, &info, sizeof(struct kdb_memp_info));
}

static void kdb_mp_free_dump(kdb_head_t head, void *data)
{
    struct kdb_memp_info *info = data;
    KDB_PRINT("[tick]:%d mp free name:%." _KDB2STR(KDB_SYS_NAME_LEN) \
        "s addr:0x%08x\r\n",
        head->tick, info->name, info->ptr);
}
#endif
#endif

#ifdef KDB_OBJECT_DUMP
static void kdb_object_hook(rt_uint16_t type_id, struct rt_object* object)
{
    struct kdb_object_info info;

    strncpy(&info.name[0], object->name, sizeof(info.name));

    info.type = object->type;
    info.flag = object->flag;

    switch (rt_object_get_type(object))
    {
    case RT_Object_Class_Thread:
#ifndef KDB_THREAD_OBJECT_DUMP
        return;
#else
        break;
#endif
    case RT_Object_Class_Semaphore:
#ifndef KDB_SEMAPHORE_OBJECT_DUMP
        return;
#else
        break;
#endif
    case RT_Object_Class_Mutex:
#ifndef KDB_MUTEX_OBJECT_DUMP
        return;
#else
        break;
#endif
    case RT_Object_Class_Event:
#ifndef KDB_EVENT_OBJECT_DUMP
        return;
#else
        break;
#endif
    case RT_Object_Class_MailBox:
#ifndef KDB_MAILBOX_OBJECT_DUMP
        return;
#else
        break;
#endif
    case RT_Object_Class_MessageQueue:
#ifndef KDB_MESSAGEQUEUE_OBJECT_DUMP
        return;
#else
        break;
#endif
    case RT_Object_Class_MemHeap:
#ifndef KDB_MEMHEAP_OBJECT_DUMP
        return;
#else
        break;
#endif
    case RT_Object_Class_MemPool:
#ifndef KDB_MEMPOOL_OBJECT_DUMP
        return;
#else
        break;
#endif
    case RT_Object_Class_Device:
#ifndef KDB_DEVICE_OBJECT_DUMP
        return;
#else
        break;
#endif
    case RT_Object_Class_Timer:
#ifndef KDB_TIMER_OBJECT_DUMP
        return;
#else
        break;
#endif
    case RT_Object_Class_Module:
#ifndef KDB_MODULE_OBJECT_DUMP
        return;
#else
        break;
#endif
    case RT_Object_Class_Unknown:
#ifndef KDB_UNKNOWN_OBJECT_DUMP
        return;
#else
        break;
#endif
    case RT_Object_Class_Static:
#ifndef KDB_STATIC_OBJECT_DUMP
        return;
#else
        break;
#endif
    default:
#ifndef KDB_DEFAULT_OBJECT_DUMP
    return;
#else
    break;
#endif
    }

    kdb_sys_push(type_id, &info, sizeof(struct kdb_object_info));
}

static void kdb_object_attach_hook(struct rt_object* object)
{
    kdb_object_hook(KDB_TYPE_OBJECT_ATTACH, object);
}

static void kdb_object_detach_hook(struct rt_object* object)
{
    kdb_object_hook(KDB_TYPE_OBJECT_DETACH, object);
}

static void kdb_object_trytake_hook(struct rt_object* object)
{
    kdb_object_hook(KDB_TYPE_OBJECT_TRYTAKE, object);
}

static void kdb_object_take_hook(struct rt_object* object)
{
    kdb_object_hook(KDB_TYPE_OBJECT_TAKE, object);
}

static void kdb_object_put_hook(struct rt_object* object)
{
    kdb_object_hook(KDB_TYPE_OBJECT_PUT, object);
}

static void kdb_object_dump(kdb_head_t head, void *data)
{
    struct kdb_object_info *info = data;
    char *object_status;
    char *object_type;

    if (head == RT_NULL)
    {
        return;
    }

    switch (head->type)
    {
    case KDB_TYPE_OBJECT_ATTACH:
        object_status = "attach";
        break;
    case KDB_TYPE_OBJECT_DETACH:
        object_status = "detach";
        break;
    case KDB_TYPE_OBJECT_TRYTAKE:
        object_status = "trytake";
        break;
    case KDB_TYPE_OBJECT_TAKE:
        object_status = "take";
        break;
    case KDB_TYPE_OBJECT_PUT:
        object_status = "put";
        break;
    default:
        object_status = "unknown";
        break;
    }

    switch (info->type & (~RT_Object_Class_Static))
    {
    case RT_Object_Class_Thread:
        object_type = "thread";
        break;
    case RT_Object_Class_Semaphore:
        object_type = "semaphore";
        break;
    case RT_Object_Class_Mutex:
        object_type = "mutex";
        break;
    case RT_Object_Class_Event:
        object_type = "event";
        break;
    case RT_Object_Class_MailBox:
        object_type = "mailbox";
        break;
    case RT_Object_Class_MessageQueue:
        object_type = "messagequeue";
        break;
    case RT_Object_Class_MemHeap:
        object_type = "memheap";
        break;
    case RT_Object_Class_MemPool:
        object_type = "mempool";
        break;
    case RT_Object_Class_Device:
        object_type = "device";
        break;
    case RT_Object_Class_Timer:
        object_type = "timer";
        break;
    case RT_Object_Class_Module:
        object_type = "module";
        break;
    case RT_Object_Class_Unknown:
        object_type = "Unknown";
        break;
    case RT_Object_Class_Static:
        object_type = "Static";
        break;
    default:
        object_type = "unknown";
        break;
    }

    KDB_PRINT("[tick]:%d %s object %s %." _KDB2STR(KDB_SYS_NAME_LEN) "s\r\n",
        head->tick, object_type, object_status, info->name);
}
#endif

#ifdef KDB_TIMER_DUMP
#ifdef KDB_TIMER_ENTER_DUMP
static void kdb_timer_timeout_enter_hook(struct rt_timer* timer)
{
    struct kdb_timer_info info;

    strncpy(&info.name[0], timer->parent.name, sizeof(info.name));
    info.flags = 0x1;
    kdb_sys_push(KDB_TYPE_TIMER_TIMEOUT, &info, sizeof(struct kdb_timer_info));
}
#endif

#ifdef KDB_TIMER_EXIT_DUMP
static void kdb_timer_timeout_exit_hook(struct rt_timer* timer)
{
    struct kdb_timer_info info;

    strncpy(&info.name[0], timer->parent.name, sizeof(info.name));
    info.flags = 0;
    kdb_sys_push(KDB_TYPE_TIMER_TIMEOUT, &info, sizeof(struct kdb_timer_info));
}
#endif

static void kdb_timer_timeout_dump(kdb_head_t head, void *data)
{
    struct kdb_timer_info *info = data;

    KDB_PRINT("[tick]:%d timeout %s %." _KDB2STR(KDB_SYS_NAME_LEN) "s\r\n",
        head->tick, (info->flags & 0x1) ? "enter" : "exit", info->name);
}
#endif

#ifdef KDB_INTERRUPT_DUMP

static void kdb_interrupt_dump(kdb_head_t head, void *data)
{
    struct kdb_interrupt_info *info = data;

    KDB_PRINT("[tick]:%d interrupt %s\r\n",
        head->tick, info->enter ? "enter" : "level");
}

#ifdef KDB_INTERRUPT_ENTER_DUMP
static void kdb_interrupt_enter_hook(void)
{
    struct kdb_interrupt_info info;

    info.enter = RT_TRUE;
    kdb_sys_push(KDB_TYPE_INTERRUPT, &info, sizeof(struct kdb_interrupt_info));
}
#endif

#ifdef KDB_INTERRUPT_LEAVE_DUMP
static void kdb_interrupt_leave_hook(void)
{
    struct kdb_interrupt_info info;

    info.enter = RT_FALSE;
    kdb_sys_push(KDB_TYPE_INTERRUPT, &info, sizeof(struct kdb_interrupt_info));
}
#endif
#endif

static void kdb_print_dump(kdb_head_t head, void *data)
{
    unsigned int len = head->len - KDB_HEAD_SIZE;
    char *str = data;
    char c;

    KDB_PRINT("[tick]:%d print:", head->tick);
    while (len > (KDB_PRINT_BUFF_SIZE - 1))
    {
        c = str[(KDB_PRINT_BUFF_SIZE - 1)];
        str[(KDB_PRINT_BUFF_SIZE - 1)] = '\0';
        KDB_PRINT("%s", str);
        str += (KDB_PRINT_BUFF_SIZE - 1);
        len -= (KDB_PRINT_BUFF_SIZE - 1);
        str[0] = c;
    }
    KDB_PRINT("%s\r\n", str);
}

void kdb_type_rtthread_register(void)
{
    kdb_type_register(KDB_TYPE_PRINT, kdb_print_dump);
    /* scheduler hook */
#ifdef KDB_SCHEDULER_DUMP
    rt_scheduler_sethook(kdb_scheduler_hook);
    kdb_type_register(KDB_TYPE_SCHEDULER, kdb_scheduler_dump);
#endif

#ifdef KDB_THREAD_DUMP
#ifdef KDB_THREAD_SUSPEND_DUMP
    rt_thread_suspend_sethook(kdb_thread_suspend_hook);
    kdb_type_register(KDB_TYPE_THREAD_SUSPEND, kdb_thread_dump);
#endif
#ifdef KDB_THREAD_RESUME_DUMP
    rt_thread_resume_sethook(kdb_thread_resume_hook);
    kdb_type_register(KDB_TYPE_THREAD_RESUME, kdb_thread_dump);
#endif
#ifdef KDB_THREAD_INITED_DUMP
    rt_thread_inited_sethook(kdb_thread_inited_hook);
    kdb_type_register(KDB_TYPE_THREAD_INITED, kdb_thread_dump);
#endif
#endif

    /* heap hook */
#ifdef KDB_HEAP_DUMP
#ifdef KDB_HEAP_MALLOC_DUMP
    rt_malloc_sethook(kdb_malloc_hook);
    kdb_type_register(KDB_TYPE_MALLOC, kdb_malloc_dump);
#endif

#ifdef KDB_HEAP_FREE_DUMP
    rt_free_sethook(kdb_free_hook);
    kdb_type_register(KDB_TYPE_FREE, kdb_free_dump);
#endif
#endif

    /* memory pool hook */
#ifdef KDB_MP_DUMP
#ifdef KDB_MP_ALLOC_DUMP
    rt_mp_alloc_sethook(kdb_mp_alloc_hook);
    kdb_type_register(KDB_TYPE_MP_ALLOC, kdb_mp_alloc_dump);
#endif

#ifdef KDB_MP_FREE_DUMP
    rt_mp_free_sethook(kdb_mp_free_hook);
    kdb_type_register(KDB_TYPE_MP_FREE, kdb_mp_free_dump);
#endif
#endif

    /* object hook */
#ifdef KDB_OBJECT_DUMP
     rt_object_attach_sethook(kdb_object_attach_hook);
     rt_object_detach_sethook(kdb_object_detach_hook);
     rt_object_trytake_sethook(kdb_object_trytake_hook);
     rt_object_take_sethook(kdb_object_take_hook);
     rt_object_put_sethook(kdb_object_put_hook);
     kdb_type_register(KDB_TYPE_OBJECT_ATTACH, kdb_object_dump);
     kdb_type_register(KDB_TYPE_OBJECT_DETACH, kdb_object_dump);
     kdb_type_register(KDB_TYPE_OBJECT_TRYTAKE, kdb_object_dump);
     kdb_type_register(KDB_TYPE_OBJECT_TAKE, kdb_object_dump);
     kdb_type_register(KDB_TYPE_OBJECT_PUT, kdb_object_dump);
#endif

    /* timer timeout hook */
#ifdef KDB_TIMER_DUMP
#ifdef KDB_TIMER_ENTER_DUMP
     rt_timer_enter_sethook(kdb_timer_timeout_enter_hook);
#endif
#ifdef KDB_TIMER_EXIT_DUMP
     rt_timer_exit_sethook(kdb_timer_timeout_exit_hook);
#endif
     kdb_type_register(KDB_TYPE_TIMER_TIMEOUT, kdb_timer_timeout_dump);
#endif

    /* interrupt hook */
#ifdef KDB_INTERRUPT_DUMP
#ifdef KDB_INTERRUPT_ENTER_DUMP
    rt_interrupt_enter_sethook(kdb_interrupt_enter_hook);
#endif
#ifdef KDB_INTERRUPT_LEAVE_DUMP
    rt_interrupt_leave_sethook(kdb_interrupt_leave_hook);
#endif
    kdb_type_register(KDB_TYPE_INTERRUPT, kdb_interrupt_dump);
#endif
}

#ifndef KDB_SYS_MEM_CUSTOM
/* mem */
static struct kdb __sys_kdb;
static rt_uint8_t __sys_kdb_buff[KDB_SYS_BUFF_SIZE];
struct kdb *sys_kdb = &__sys_kdb;
rt_uint8_t *sys_kdb_buff = __sys_kdb_buff;
rt_uint32_t sys_kdb_buff_size = sizeof(__sys_kdb_buff);
#endif

void kdb_printf(const char *fmt, ...)
{
    va_list args;
    rt_size_t length;
    char *log_buf;

    if (kdb_isactive(sys_kdb))
    {
        log_buf = sys_kdb->data_buff;
        va_start(args, fmt);
        length = rt_vsnprintf(log_buf, KDB_MAX_DATA_LEN - 1, fmt, args);
        if (length > KDB_MAX_DATA_LEN - 1)
            length = KDB_MAX_DATA_LEN - 1;
        log_buf[KDB_MAX_DATA_LEN - 1] = '\0';
        kdb_sys_push(KDB_TYPE_PRINT, log_buf, length + 1);
        va_end(args);
    }
}

int kdb_sys_init(void)
{
    kdb_type_rtthread_register();
    kdb_init(sys_kdb, sys_kdb_buff, sys_kdb_buff_size);
    return 0;
}
#ifdef KDB_SYS_AUTO_INIT
INIT_PREV_EXPORT(kdb_sys_init);
#endif

void kdb_sys_start(void)
{
    kdb_start(sys_kdb);
}

void kdb_sys_stop(void)
{
    kdb_stop(sys_kdb);
}

void kdb_sys_push(rt_uint16_t type, void *buff, rt_uint16_t len)
{
    kdb_record(sys_kdb, type, buff, len);
}

void kdb_sys_dump(void)
{
    kdb_trace(sys_kdb);
}

void kdb_sys_stop_dump(void)
{
    kdb_sys_stop();
    kdb_sys_dump();
}

rt_uint32_t kdb_sys_ccode(void)
{
    return kdb_ccode(sys_kdb);
}

int kdb_sys_isvalid(void)
{
    return kdb_isvalid(sys_kdb);
}
