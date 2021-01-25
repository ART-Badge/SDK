#pragma once

#include "js_object.h"
#include <pm_wheel.h>

class JsWheelString : public Persimmon::WheelString
{
public:
    JsWheelString();
    virtual ~JsWheelString();

    void setJsFuncChanged(js_value_t object, const char* func);
    void setJsFuncTap(js_value_t object, const char* func);

    void onChanged(unsigned int);
    void onTap(unsigned int);
    void onChangedS(std::string);
    void onTapS(std::string);

    virtual void setData(void* data);

private:
    js_value_t jsCallFuncChanged, jsCallFuncTap;
    js_value_t jsObj;
};
