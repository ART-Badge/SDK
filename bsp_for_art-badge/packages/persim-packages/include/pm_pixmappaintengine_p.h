#pragma once

#include "pm_drawfuncs.h"

#define swap_ge(a, b) \
    if (a > b)        \
    {                 \
        int _t = a;   \
        a      = b;   \
        b      = _t;  \
    }

PERSIMMON_NAMESPACE_BEGIN

typedef void (*DrawLineFunc)(PixelBlitter::BlitInfo &, Texture &, const Line &, const Rect &);
typedef uint8_t *(*FetchPixelsFuncEx)(uint8_t *buffer, const Texture &t, int x, int y, int width);

template<PixelFormat::Format format>
static uint8_t *fetchPixels(uint8_t *buffer, const Texture &t, int x, int y, int width)
{
    return nullptr;
}

template<>
uint8_t *fetchPixels<PixelFormat::Mono>(uint8_t *buffer, const Texture &t, int x, int y, int width)
{
    int mask           = 0x80 >> (x & 7);
    uint8_t *dst       = buffer;
    const uint8_t *src = t.pixels() + y * t.pitch() + (x >> 3);
    for (const uint8_t *end = src + width; src < end; ++src)
    {
        unsigned byte = *src;
        for (; mask; mask >>= 1) *dst = byte & mask ? 0xff : 0;
        mask = 0x80;
    }
    return buffer;
}

template<>
uint8_t *fetchPixels<PixelFormat::A2>(uint8_t *buffer, const Texture &t, int x, int y, int width)
{
    uint8_t *dst       = buffer;
    int pos            = 6 - ((x & 3) << 1);
    const uint8_t *src = t.pixels() + y * t.pitch() + (x >> 2);
    for (width += x & 3; width > 0; width -= 4)
    {
        unsigned byte = *src++;
        for (; pos >= 0; pos -= 2) *dst = ((byte >> pos) & 3) * 85;
        pos = 6;
    }
    return buffer;
}

template<>
uint8_t *fetchPixels<PixelFormat::A4>(uint8_t *buffer, const Texture &t, int x, int y, int width)
{
    uint8_t *dst       = buffer;
    int pos            = 4 - ((x & 1) << 2);
    const uint8_t *src = t.pixels() + y * t.pitch() + (x >> 1);
    for (width += x & 1; width > 0; width -= 2)
    {
        unsigned byte = *src++;
        for (; pos >= 0; pos -= 4) *dst = ((byte >> pos) & 15) * 17;
        pos = 4;
    }
    return buffer;
}

static void drawHLine(PixelBlitter::BlitInfo &bi, Texture &dst, const Line &line, const Rect &clip)
{
    int y = line.y1();
    if (clip.top() <= y && clip.bottom() >= y)
    {
        int x1 = line.x1(), x2 = line.x2();
        swap_ge(x1, x2);
        x1 = pmax(x1, clip.left());
        x2 = pmin(x2, clip.right());
        if (x1 <= x2)
        {
            bi.width      = x2 - x1 + 1;
            bi.height     = 1;
            bi.dst.pixels = fetchPixels(dst, x1, y);
            PixelBlitter::instance()->fill(bi);
        }
    }
}

static void drawVLine(PixelBlitter::BlitInfo &bi, Texture &dst, const Line &line, const Rect &clip)
{
    int x = line.x1();
    if (clip.left() <= x && clip.right() >= x)
    {
        int y1 = line.y1(), y2 = line.y2();
        swap_ge(y1, y2);
        y1 = pmax(y1, clip.top());
        y2 = pmin(y2, clip.bottom());
        if (y1 <= y2)
        {
            bi.width      = 1;
            bi.height     = y2 - y1 + 1;
            bi.dst.pixels = fetchPixels(dst, x, y1);
            PixelBlitter::instance()->fill(bi);
        }
    }
}

#define drawLineSetPixel(x, y)                  \
    if (clip.contains(x, y))                    \
    {                                           \
        bi.dst.pixels = fetchPixels(dst, x, y); \
        PixelBlitter::instance()->fill(bi);     \
    }

static void drawSLine(PixelBlitter::BlitInfo &bi, Texture &dst, const Line &line, const Rect &clip)
{
    bi.width  = 1;
    bi.height = 1;
    int dx, dy, addx, addy;
    int x1 = line.x1(), y1 = line.y1(), x2 = line.x2(), y2 = line.y2();
    if (x1 < x2)
    {
        dx   = x2 - x1;
        addx = 1;
    }
    else
    {
        dx   = x1 - x2;
        addx = -1;
    }
    if (y1 < y2)
    {
        dy   = y2 - y1;
        addy = 1;
    }
    else
    {
        dy   = y1 - y2;
        addy = -1;
    }
    if (dx >= dy)
    {
        dy <<= 1;
        int p = dy - dx, diff = p - dx;
        for (int i = 0; i <= dx; ++i)
        {
            drawLineSetPixel(x1, y1);
            x1 += addx;
            if (p < 0)
            {
                p += dy;
            }
            else
            {
                y1 += addy;
                p += diff;
            }
        }
    }
    else
    {
        dx <<= 1;
        int p = dx - dy, diff = p - dy;
        for (int i = 0; i <= dy; ++i)
        {
            drawLineSetPixel(x1, y1);
            y1 += addy;
            if (p < 0)
            {
                p += dx;
            }
            else
            {
                x1 += addx;
                p += diff;
            }
        }
    }
}

static inline DrawLineFunc fetchDrawLineFunc(const Line &line)
{
    if (!line.dy())
        return drawHLine;
    if (!line.dx())
        return drawVLine;
    return drawSLine;
}

static void drawLines(Texture &dst, const Point &origin, const Line *lines, int count,
                      uint32_t color, const Rect &clip)
{
    if (!clip.isEmpty())
    {
        PixelBlitter::BlitInfo bi;
        bi.src.mask   = color;
        bi.dst.pitch  = dst.pitch();
        bi.dst.pixfmt = uint8_t(dst.pixelFormat());
        for (int i = 0; i < count; ++i)
        {
            const Line line = lines[i] + origin;
            fetchDrawLineFunc(line)(bi, dst, line, clip);
        }
    }
}

static void fillRects(Texture &dst, const Point &origin, const Rect *rects, int count,
                      uint32_t color, const Rect &clip)
{
    PixelBlitter::BlitInfo bi;
    bi.src.mask   = color;
    bi.dst.pitch  = dst.pitch();
    bi.dst.pixfmt = uint8_t(dst.pixelFormat());
    for (int i = 0; i < count; ++i)
    {
        Rect r((rects[i] + origin) & clip);
        if (r.isEmpty())
            continue;
        bi.width      = r.width();
        bi.height     = r.height();
        bi.dst.pixels = fetchPixels(dst, r.x(), r.y());
        PixelBlitter::instance()->fill(bi);
    }
}

static void blitTexture(Texture &dst, const Texture &src, const Rect &clip, const Point &pos,
                        uint32_t mask)
{
    if (!clip.isEmpty() && src.rect().contains(pos))
    {
        PixelBlitter::BlitInfo bi;
        bi.width       = clip.width();
        bi.height      = clip.height();
        bi.dst.pitch   = dst.pitch();
        bi.dst.pixfmt  = uint8_t(dst.pixelFormat());
        bi.dst.pixels  = fetchPixels(dst, clip.x(), clip.y());
        bi.src.pitch   = src.pitch();
        bi.src.mask    = mask;
        bi.src.lut     = src.lut();
        bi.src.lutSize = uint16_t(src.lutSize());

        FetchPixelsFuncEx fetch;
        switch (src.pixelFormat())
        {
        case PixelFormat::Mono: fetch = fetchPixels<PixelFormat::Mono>; break;
        case PixelFormat::A2: fetch = fetchPixels<PixelFormat::A2>; break;
        case PixelFormat::A4: fetch = fetchPixels<PixelFormat::A4>; break;
        default:
            bi.src.pixfmt = uint8_t(src.pixelFormat());
            bi.src.pixels = fetchPixels(src, pos.x(), pos.y());
            PixelBlitter::instance()->blit(bi);
            return;
        }
        bi.src.pixfmt = uint8_t(PixelFormat::A8);
        for (int y = pos.y(), bot = y + bi.height; y < bot; ++y)
        {
            uint8_t buf[256];
            bi.src.pixels = fetch(buf, src, pos.x(), y, bi.width);
            PixelBlitter::instance()->blit(bi);
        }
    }
}

PERSIMMON_NAMESPACE_END
