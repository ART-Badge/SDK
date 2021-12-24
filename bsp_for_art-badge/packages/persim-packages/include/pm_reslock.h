#pragma once

#include "pm_respack.h"
#include "pm_mutex.h"

PERSIMMON_NAMESPACE_BEGIN

class ResPack::Locker : public LockGuard<Mutex>
{
public:
    Locker();
};

PERSIMMON_NAMESPACE_END
