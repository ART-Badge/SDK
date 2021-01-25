#pragma once

#include "js_object.h"
#include <pm_slider.h>

class JsSlider : public Persimmon::Slider
{
public:
    JsSlider();
    virtual ~JsSlider();

    void setJsFunction(js_value_t object, const char* func);

    void onSignal(int id, int value);

    virtual void setData(void* data);

private:
    js_value_t jsCallFunc;
    js_value_t jsObj;
};
