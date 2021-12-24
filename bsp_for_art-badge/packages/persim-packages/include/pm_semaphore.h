#pragma once

#include "pm_internal.h"
#include <rtthread.h>

PERSIMMON_NAMESPACE_BEGIN

class Semaphore
{
public:
    typedef rt_semaphore *native_handle_type;
    explicit Semaphore(int value = 0) { rt_sem_init(&_sem, "pm", value, RT_IPC_FLAG_FIFO); }
    explicit Semaphore(const char *name) { rt_sem_init(&_sem, name, 0, RT_IPC_FLAG_FIFO); }
    Semaphore(int value, const char *name) { rt_sem_init(&_sem, name, value, RT_IPC_FLAG_FIFO); }
    ~Semaphore() { rt_sem_detach(&_sem); }
    bool take(int timeout = -1) { return rt_sem_take(&_sem, uint32_t(timeout)) == RT_EOK; }
    bool try_take() { return rt_sem_trytake(&_sem) == RT_EOK; }
    bool release() { return rt_sem_release(&_sem) == RT_EOK; }
    native_handle_type ntaive_handle() { return &_sem; }

private:
    rt_semaphore _sem;
};

PERSIMMON_NAMESPACE_END
