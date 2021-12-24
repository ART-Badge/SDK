#pragma once

#include "pm_internal.h"
#include <rtthread.h>

PERSIMMON_NAMESPACE_BEGIN

class InterruptLocker
{
public:
    InterruptLocker();
    ~InterruptLocker();

private:
    rt_base_t level;
};

PERSIMMON_NAMESPACE_END
