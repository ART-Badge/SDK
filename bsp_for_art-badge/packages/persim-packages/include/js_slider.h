#pragma once

#include <pm_slider.h>
#include "js_utility.h"

class JsSlider : public persim::Slider, public JsEventHandler
{
public:
    JsSlider();
    virtual ~JsSlider();
    virtual void setData(JsPage *page, const JsValue &data);

private:
    void onSignal(Slider::Type, int value);
};
