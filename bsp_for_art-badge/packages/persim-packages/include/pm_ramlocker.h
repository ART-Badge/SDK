#pragma once

#include "pm_internal.h"

PERSIMMON_NAMESPACE_BEGIN

#ifndef PKG_PERSIMMON_RAM_LOCKER
class RamLocker
{
public:
    struct Locker
    {
        virtual void lock()   = 0;
        virtual void unlock() = 0;
    };

    static void lock() {}
    static void unlock() {}
    static void setLocker(Locker *) {}

    RamLocker() {}
    ~RamLocker() {}
};
#else
class RamLocker
{
public:
    struct Locker
    {
        virtual void lock() = 0;
        virtual void unlock() = 0;
    };

    RamLocker() { lock(); }
    ~RamLocker() { unlock(); }

    static void lock() { _locker->lock(); }
    static void unlock() { _locker->unlock(); }
    static void setLocker(Locker *locker);

private:
    static Locker *_locker;
};
#endif

PERSIMMON_NAMESPACE_END
