#pragma once

#include "pm_internal.h"

PERSIMMON_NAMESPACE_BEGIN

template<class _Lock> class LockGuard : utils::noncopyable<LockGuard<_Lock> >
{
public:
    typedef _Lock lock_type;
    typedef _Lock mutex_type;
    struct AdoptLock
    {
    };
    const static AdoptLock adopt;

    explicit LockGuard(lock_type &lock) : _lock(lock) { _lock.lock(); }
    LockGuard(lock_type &lock, AdoptLock) : _lock(lock) {}
    ~LockGuard() { _lock.unlock(); }

private:
    lock_type &_lock;
};

PERSIMMON_NAMESPACE_END
