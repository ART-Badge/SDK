#pragma once

#include "pm_blendpixel.h"
#include "pm_texture.h"
#include "pm_pixelblitter.h"

PERSIMMON_NAMESPACE_BEGIN

template<PixelFormat::Format format> struct ColorBPP
{
    enum
    {
        bpp = 0
    };
};

template<> struct ColorBPP<PixelFormat::ARGB8888>
{
    enum
    {
        bpp = 4
    };
};

template<> struct ColorBPP<PixelFormat::XRGB8888>
{
    enum
    {
        bpp = 4
    };
};

template<> struct ColorBPP<PixelFormat::RGB888>
{
    enum
    {
        bpp = 3
    };
};

template<> struct ColorBPP<PixelFormat::BGR888>
{
    enum
    {
        bpp = 3
    };
};

template<> struct ColorBPP<PixelFormat::RGB565>
{
    enum
    {
        bpp = 2
    };
};

template<> class ColorBPP<PixelFormat::ARGB4444>
{
public:
    enum
    {
        bpp = 2
    };
};

template<> struct ColorBPP<PixelFormat::A8>
{
    enum
    {
        bpp = 1
    };
};

template<PixelFormat::Format format> inline uint32_t fetchColor(const uint8_t *pixel)
{
    return 0;
}

template<> inline uint32_t fetchColor<PixelFormat::ARGB8888>(const uint8_t *pixel)
{
    return *reinterpret_cast<const uint32_t *>(pixel);
}

template<> inline uint32_t fetchColor<PixelFormat::XRGB8888>(const uint8_t *pixel)
{
    return *reinterpret_cast<const uint32_t *>(pixel);
}

template<> inline uint32_t fetchColor<PixelFormat::RGB888>(const uint8_t *pixel)
{
    return 0xff000000 | (pixel[0] << 16) | (pixel[1] << 8) | pixel[2];
}

template<> inline uint32_t fetchColor<PixelFormat::BGR888>(const uint8_t *pixel)
{
    return 0xff000000 | (pixel[2] << 16) | (pixel[1] << 8) | pixel[0];
}

template<> inline uint32_t fetchColor<PixelFormat::RGB565>(const uint8_t *pixel)
{
    uint32_t c = *reinterpret_cast<const uint16_t *>(pixel);
    return 0xff000000 | ((c << 3) & 0xf8) | ((c >> 2) & 0x7) | ((c << 5) & 0xfc00)
         | ((c >> 1) & 0x300) | ((c << 8) & 0xf80000) | ((c << 3) & 0x70000);
}

template<> inline uint32_t fetchColor<PixelFormat::ARGB4444>(const uint8_t *pixel)
{
    uint32_t c = *reinterpret_cast<const uint16_t *>(pixel);
    return ((c << 4) & 0xf0) | ((c >> 4) & 0xf) | ((c << 8) & 0xf000) | ((c << 4) & 0x0f00)
         | ((c << 12) & 0xf00000) | ((c << 8) & 0x0f0000) | ((c << 16) & 0xf0000000)
         | ((c << 12) & 0x0f000000);
}

template<PixelFormat::Format format> inline void blitColor(uint8_t *pixel, uint32_t color) {}

template<> inline void blitColor<PixelFormat::ARGB8888>(uint8_t *pixel, uint32_t color)
{
    *reinterpret_cast<uint32_t *>(pixel) = color;
}

template<> inline void blitColor<PixelFormat::XRGB8888>(uint8_t *pixel, uint32_t color)
{
    *reinterpret_cast<uint32_t *>(pixel) = color;
}

template<> inline void blitColor<PixelFormat::RGB888>(uint8_t *pixel, uint32_t color)
{
    pixel[0] = (color >> 16) & 0xff;
    pixel[1] = (color >> 8) & 0xff;
    pixel[2] = color & 0xff;
}

template<> inline void blitColor<PixelFormat::BGR888>(uint8_t *pixel, uint32_t color)
{
    pixel[0] = color & 0xff;
    pixel[1] = (color >> 8) & 0xff;
    pixel[2] = (color >> 16) & 0xff;
}

template<> inline void blitColor<PixelFormat::RGB565>(uint8_t *pixel, uint32_t color)
{
    uint16_t c = ((color >> 3) & 0x001f) | ((color >> 5) & 0x07e0) | ((color >> 8) & 0xf800);
    *reinterpret_cast<uint16_t *>(pixel) = c;
}

template<> inline void blitColor<PixelFormat::ARGB4444>(uint8_t *pixel, uint32_t color)
{
    uint16_t c = ((color >> 4) & 0x000f) | ((color >> 8) & 0x00f0) | ((color >> 12) & 0x0f00)
               | ((color >> 16) & 0xf000);
    *reinterpret_cast<uint16_t *>(pixel) = c;
}

template<PixelFormat::Format format, bool isBlend> struct PixelDrawColorCore
{
    static inline void draw(uint8_t *pixels, uint32_t color) {}
};

template<PixelFormat::Format format> struct PixelDrawColorCore<format, true>
{
    static inline void draw(uint8_t *pixels, uint32_t color)
    {
        uint32_t c = fetchColor<format>(pixels);
        blitColor<format>(pixels, ALPHA_BLEND(c, color));
    }
};

template<PixelFormat::Format format> struct PixelDrawColorCore<format, false>
{
    static inline void draw(uint8_t *pixels, uint32_t color) { blitColor<format>(pixels, color); }
};

template<PixelFormat::Format format, bool isBlend>
inline void pixelDrawColor(uint8_t *dst, uint32_t color, int pitch, int height, int skip)
{
    while (height--)
    {
        for (uint8_t *end = dst + pitch; dst < end; dst += ColorBPP<format>::bpp)
        {
            PixelDrawColorCore<format, isBlend>::draw(dst, color);
        }
        dst += skip;
    }
}

template<PixelFormat::Format dstfmt, PixelFormat::Format srcfmt> struct PixelDrawTextureCore
{
    static inline void draw(uint8_t *dst, const uint8_t *src)
    {
        uint32_t c1 = fetchColor<dstfmt>(dst);
        uint32_t c2 = fetchColor<srcfmt>(src);
        blitColor<dstfmt>(dst, ALPHA_BLEND(c1, c2));
    }
};

template<PixelFormat::Format dstfmt> struct PixelDrawTextureCore<dstfmt, PixelFormat::RGB565>
{
    static inline void draw(uint8_t *dst, const uint8_t *src)
    {
        uint32_t c = fetchColor<PixelFormat::RGB565>(src);
        blitColor<dstfmt>(dst, c);
    }
};

template<PixelFormat::Format dstfmt> struct PixelDrawTextureCore<dstfmt, PixelFormat::RGB888>
{
    static inline void draw(uint8_t *dst, const uint8_t *src)
    {
        uint32_t c = fetchColor<PixelFormat::RGB888>(src);
        blitColor<dstfmt>(dst, c);
    }
};

template<PixelFormat::Format dstfmt> struct PixelDrawTextureCore<dstfmt, PixelFormat::BGR888>
{
    static inline void draw(uint8_t *dst, const uint8_t *src)
    {
        uint32_t c = fetchColor<PixelFormat::BGR888>(src);
        blitColor<dstfmt>(dst, c);
    }
};

template<PixelFormat::Format dstfmt> struct PixelDrawTextureCore<dstfmt, PixelFormat::XRGB8888>
{
    static inline void draw(uint8_t *dst, const uint8_t *src)
    {
        uint32_t c = fetchColor<PixelFormat::XRGB8888>(src);
        blitColor<dstfmt>(dst, c);
    }
};

template<PixelFormat::Format dstfmt, PixelFormat::Format srcfmt> struct PixelBlendTextureCore
{
    static inline void draw(uint8_t *dst, const uint8_t *src, uint32_t mask)
    {
        uint32_t c1 = fetchColor<dstfmt>(dst);
        uint32_t c2 = fetchColor<srcfmt>(src);
        c2          = PIXEL_ALPHA(c2, mask);
        blitColor<dstfmt>(dst, ALPHA_BLEND(c1, c2));
    }
};

template<PixelFormat::Format dstfmt> struct PixelBlendTextureCore<dstfmt, PixelFormat::A8>
{
    static inline void draw(uint8_t *dst, const uint8_t *src, uint32_t mask)
    {
        uint32_t c1 = fetchColor<dstfmt>(dst);
        uint32_t c2 = PIXEL_ALPHA(mask, *src);
        blitColor<dstfmt>(dst, ALPHA_BLEND(c1, c2));
    }
};

template<PixelFormat::Format srcfmt, PixelFormat::Format dstfmt>
inline void pixelDrawTexture(const uint8_t *src, uint8_t *dst, int, int pitch, int rows,
                             int srcskip, int dstskip)
{
    while (rows--)
    {
        uint8_t *end = dst + pitch;
        while (dst < end)
        {
            PixelDrawTextureCore<dstfmt, srcfmt>::draw(dst, src);
            src += ColorBPP<srcfmt>::bpp;
            dst += ColorBPP<dstfmt>::bpp;
        }
        src += srcskip;
        dst += dstskip;
    }
}

template<PixelFormat::Format srcfmt, PixelFormat::Format dstfmt>
inline void pixelBlendTexture(const uint8_t *src, uint8_t *dst, int, int pitch, int rows,
                              int srcskip, int dstskip, uint32_t mask)
{
    while (rows--)
    {
        uint8_t *end = dst + pitch;
        while (dst < end)
        {
            PixelBlendTextureCore<dstfmt, srcfmt>::draw(dst, src, mask);
            src += ColorBPP<srcfmt>::bpp;
            dst += ColorBPP<dstfmt>::bpp;
        }
        src += srcskip;
        dst += dstskip;
    }
}

inline uint8_t *fetchPixels(const Texture &t, int x, int y)
{
    return t.pixels() + y * t.pitch() + x * PixelFormat::pitch(t.pixelFormat());
}

class SpanBlitter
{
public:
    SpanBlitter(Texture *dst);
    void setOpacity(int opacity);
    void blitSpan(uint32_t *span, int x, int y, int length);

private:
    PixelBlitter::BlitInfo bi;
    Texture *dst;

    friend class SpanBlitterPrivate;
};

PERSIMMON_NAMESPACE_END
