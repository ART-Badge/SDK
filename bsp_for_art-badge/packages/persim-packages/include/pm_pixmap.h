#pragma once

#include "pm_texture.h"
#include "pm_paintdevice.h"

PERSIMMON_NAMESPACE_BEGIN

class Pixmap : public Texture, public PaintDevice
{
public:
    Pixmap();
    Pixmap(int width, int height, int pixfmt = PixelFormat::RGB888);
    Pixmap(const Pixmap &other);
    virtual ~Pixmap();
    Pixmap &operator=(const Pixmap &other);
    Pixmap copy() const;
    virtual PaintEngine *paintEngine();
    using Texture::height;
    using Texture::size;
    using Texture::width;
    void clear();

private:
    struct PixmapData *m_data;
};

PERSIMMON_NAMESPACE_END
