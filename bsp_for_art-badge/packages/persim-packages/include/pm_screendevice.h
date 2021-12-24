#pragma once

#include "pm_texture.h"
#include "pm_paintdevice.h"

PERSIMMON_NAMESPACE_BEGIN

class ScreenDevice : public PaintDevice, public Texture
{
public:
    ScreenDevice();
    virtual ~ScreenDevice();
    virtual PaintEngine *paintEngine();
    using Texture::height;
    using Texture::width;
};

PERSIMMON_NAMESPACE_END
