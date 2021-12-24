#pragma once

#include <pm_wheel.h>
#include "js_utility.h"

class JsCarouselImage : public Persimmon::CarouselImage
{
public:
    JsCarouselImage();
    virtual ~JsCarouselImage();

    void onChanged(int value);
    void onTap(int value);

private:
};
