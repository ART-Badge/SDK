#pragma once

#include "pm_rect.h"

PERSIMMON_NAMESPACE_BEGIN

class PixelBlitter
{
public:
    // for information about pixel format, refer to the PixelFormat class
    struct BlitInfo
    {
        int width, height; // the width and height of the operating area (source and destination are
                           // the same)
        struct
        {                    // destination pixels information
            uint8_t *pixels; // destination pixels address
            int pitch;       // destination bytes per line
            uint8_t pixfmt;  // destination pixel format
        } dst;
        struct
        {                          // source pixels information
            const uint8_t *pixels; // source pixels address (only blit mode)
            int pitch;             // source bytes per line (only blit mode)
            uint8_t pixfmt;        // source pixel format (only blit mode)
            uint16_t lutSize;      // source LUT index count (only blit mode @ alpha source format)
            uint32_t mask;         // fill mode: source color (ARGB8888)
                           // blit mode: source transparency ([0, 255] -> transparent to opaque),
                           //            mask color (ARB8888) @ alpha source format
            uint32_t
                *lut; // LUT address, each value is ARGB8888 (only blit mode @ alpha source format)
        } src;
    };

    virtual void fill(const BlitInfo &info) = 0;
    virtual void blit(const BlitInfo &info) = 0;

    static PixelBlitter *instance() { return _instance; }
    void registered();

    PixelBlitter() {}
    virtual ~PixelBlitter() {}

private:
    static PixelBlitter *_instance;
};

#ifndef PKG_PERSIMMON_DISABLE_DEFAULT_BLITTER
class PixelBlitterDefault : public PixelBlitter
{
public:
    PixelBlitterDefault() {}
    virtual void fill(const BlitInfo &info);
    virtual void blit(const BlitInfo &info);
};
#endif

PERSIMMON_NAMESPACE_END
