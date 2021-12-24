#pragma once

#ifdef __cplusplus

#include "jsvm/jsvm.h"

namespace jsvm
{
bool clearModuleCache();
bool clearModuleCache(const String &name);
} // namespace jsvm

#endif

#ifdef __cplusplus
extern "C" {
#endif

void jsvm_module_dump(int argc, char **argv);
void jsvm_module_clear(int argc, char **argv);

#ifdef __cplusplus
}
#endif
