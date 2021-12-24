#pragma once

#include "pm_internal.h"
#include "pm_rect.h"

PERSIMMON_NAMESPACE_BEGIN

class Texture
{
public:
    virtual ~Texture();
    bool isEmpty() const;
    int pixelFormat() const { return data.pixfmt; }
    int pitch() const { return data.pitch; }
    int width() const { return data.width; }
    int height() const { return data.height; }
    uint8_t *pixels() const { return data.pixels; }
    uint8_t *lineSpan(int line) const { return &data.pixels[line * data.pitch]; }
    uint32_t *lut() const { return data.lut; }
    int lutSize() const { return int(data.lutSize); }
    Size size() const { return Size(width(), height()); }
    Rect rect() const { return Rect(0, 0, width(), height()); }

protected:
    Texture();
    void swap(Texture &src);
    void assign(const Texture &src);
    struct Data
    {
        uint64_t pixfmt : 5;
        uint64_t lutSize : 9;
        uint64_t pitch : 16;
        uint64_t extra : 6; // can be used as flags
        uint64_t width : 14;
        uint64_t height : 14;
        uint8_t *pixels;
        uint32_t *lut;
    } data;
};

PERSIMMON_NAMESPACE_END
