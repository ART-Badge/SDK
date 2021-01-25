#pragma once

#include <animated_image.h>
#include "js_util.h"

class JsAnimatedImage : public Persimmon::AnimatedImage
{
public:
    JsAnimatedImage();
    JsAnimatedImage(const JsAnimatedImage &other);
    virtual ~JsAnimatedImage();

    virtual Widget *depthCopy(void) const;
    void setJsFunction(js_value_t object, const char *func);
    void onSignal(int id);
    virtual void setData(void *data);

private:
    js_value_t jsObj, jsCallFunc;
};
