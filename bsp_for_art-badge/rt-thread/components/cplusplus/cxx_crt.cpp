/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
* Change Logs:
* Date           Author       Notes
* 2015-03-07     Bernard      Add copyright header.
*/

#include <rtthread.h>
#include "crt.h"

#ifdef PKG_USING_PMEM
extern "C" {
#include <pmem.h>
extern pmem_t pmem_ptr;
}
#endif

void *operator new(size_t size)
{
#ifdef PKG_USING_PMEM
    return pmem_alloc(pmem_ptr, size);
#else
    return rt_malloc(size);
#endif
}

void *operator new[](size_t size)
{
#ifdef PKG_USING_PMEM
    return pmem_alloc(pmem_ptr, size);
#else
    return rt_malloc(size);
#endif
}

void operator delete(void *ptr)
{
#ifdef PKG_USING_PMEM
    pmem_free(pmem_ptr, ptr);
#else
    rt_free(ptr);
#endif
    
}

void operator delete[](void *ptr)
{
#ifdef PKG_USING_PMEM
    pmem_free(pmem_ptr, ptr);
#else
    rt_free(ptr);
#endif
}

void __cxa_pure_virtual(void)
{
    rt_kprintf("Illegal to call a pure virtual function.\n");
}
