/*
 * File      : jsvm_heap_port.h
 * COPYRIGHT (C) 2008 - 2020, RT-Thread Development Team
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-02     xxx          the first version
 */
#pragma once

#include <stddef.h>

namespace jsvm
{

class HeapAllocator
{
public:
    virtual void *alloc(size_t size) = 0;
    virtual void free(void *ptr)     = 0;
    virtual size_t size()            = 0;
    void registered();

    static HeapAllocator *instance();
};

} // namespace jsvm
