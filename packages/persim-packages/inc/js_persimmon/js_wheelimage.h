#pragma once

#include "js_object.h"
#include <pm_wheel.h>

class JsWheelImage : public Persimmon::WheelImage
{
public:
    JsWheelImage();
    virtual ~JsWheelImage();

    void setJsFuncChanged(js_value_t object, const char* func);
    void setJsFuncTap(js_value_t object, const char* func);

    void onChanged(unsigned int value);
    void onTap(unsigned int value);

    virtual void setData(void* data);

private:
    js_value_t jsCallFuncChanged, jsCallFuncTap;
    js_value_t jsObj;
};
