#pragma once

#include <jsvm/utils.h>
#include "pm_string.h"

PERSIMMON_NAMESPACE_BEGIN

class JsAppX;

class Launcher
{
public:
    Launcher();
    ~Launcher();
    static bool launch(const String &url, bool isLauncher);
    static JsAppX *loadApp(const String &url, bool isLauncher);
    static bool loadScript(const String &url);
    static bool launch(const char *const paths[], size_t count);

private:
    jsvm::global_vm _vm;
};

PERSIMMON_NAMESPACE_END
