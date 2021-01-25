#pragma once

#include "js_object.h"
#include <pm_checkbox.h>


class JsCheckbox : public Persimmon::Checkbox
{
public:
    JsCheckbox();
    virtual ~JsCheckbox();

    void setJsFunction(js_value_t object, const char* func);

    void onSignal(int id, bool s);

    virtual void setData(void* data);

private:
    js_value_t jsCallFunc;
    js_value_t jsObj;
};

