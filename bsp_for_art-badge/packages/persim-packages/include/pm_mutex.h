#pragma once

#include "pm_lockguard.h"
#include <rtthread.h>

PERSIMMON_NAMESPACE_BEGIN

class Mutex : utils::noncopyable<Mutex>
{
public:
    typedef rt_mutex *native_handle_type;

    Mutex();
    Mutex(const char *name);
    ~Mutex();
    void lock();
    bool try_lock(int time = 0);
    void unlock();
    native_handle_type native_handle() { return &_m; }

    void* operator new(size_t size);
    void operator delete(void *ptr);

private:
    rt_mutex _m;
};

PERSIMMON_NAMESPACE_END
