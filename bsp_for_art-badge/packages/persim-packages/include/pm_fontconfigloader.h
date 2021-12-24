#pragma once

#include "pm_internal.h"
#include "pm_bytearray.h"
#include "pm_fontconfigmanager.h"

PERSIMMON_NAMESPACE_BEGIN

namespace fontcfg
{
FontConfigMap *loadConfig(const ByteArray &xml);
}

PERSIMMON_NAMESPACE_END
