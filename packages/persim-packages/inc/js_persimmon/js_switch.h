#pragma once

#include "js_object.h"
#include <pm_switch.h>

class JsSwitch : public Persimmon::Switch
{
public:
    JsSwitch();
    virtual ~JsSwitch();

    void setJsFunction(js_value_t object, const char* func);

    void onSignal(int id, bool s);

    virtual void setData(void* data);

private:
    js_value_t jsCallFunc;
    js_value_t jsObj;
};