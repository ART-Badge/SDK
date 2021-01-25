#pragma once

#include "js_object.h"
#include <pm_wheel.h>

class JsCarouselImage : public Persimmon::CarouselImage
{
public:
    JsCarouselImage();
    virtual ~JsCarouselImage();

    void setJsFuncChanged(js_value_t object, const char* func);
    void setJsFuncTap(js_value_t object, const char* func);

    void onChanged(int value);
    void onTap(int value);

    virtual void setData(void* data);

private:
    js_value_t jsCallFuncChanged, jsCallFuncTap;
    js_value_t jsObj;
};
