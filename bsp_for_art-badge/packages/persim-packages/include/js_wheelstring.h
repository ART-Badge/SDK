#pragma once

#include <pm_wheel.h>
#include "js_utility.h"

class JsWheelString : public persim::WheelString
{
public:
    JsWheelString();
    virtual ~JsWheelString();

    void onChanged(unsigned int);
    void onTap(unsigned int);
    void onChangedS(const persim::String &);
    void onTapS(const persim::String &);

    virtual void setData(JsPage *page, const JsValue &data);

    void bindChangeCallback(const JsEventCallback &cb);
    void bindTapCallback(const JsEventCallback &cb);

private:
    JsEventCallback _changed, _tap;
};
