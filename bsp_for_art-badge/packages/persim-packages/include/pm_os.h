#pragma once

#include "pm_internal.h"
#include <rtthread.h>

PERSIMMON_NAMESPACE_BEGIN

namespace os
{
uint32_t clock_ms();
uint64_t clock_us();
} // namespace os

PERSIMMON_NAMESPACE_END
