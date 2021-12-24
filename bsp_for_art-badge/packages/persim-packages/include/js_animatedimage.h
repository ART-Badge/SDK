#pragma once

#include <pm_animatedimage.h>
#include "js_utility.h"

class JsAnimatedImage : public persim::AnimatedImage, public JsEventHandler
{
public:
    JsAnimatedImage();
    void onSignal();
    virtual void setData(JsPage *page, const JsValue &data);
    virtual void getData(JsValue &ret, const persim::String &name) const;
};
