#pragma once

#include "js_utility.h"
#include <pm_wheel.h>

class JsWheelImage : public persim::WheelImage
{
public:
    JsWheelImage();
    virtual ~JsWheelImage();

    void onChanged(unsigned int value);
    void onTap(unsigned int value);

    virtual void setData(JsPage *page, const JsValue &data);

    void bindChangeCallback(const JsEventCallback &cb);
    void bindTapCallback(const JsEventCallback &cb);

private:
    JsEventCallback _changed, _tap;
};
