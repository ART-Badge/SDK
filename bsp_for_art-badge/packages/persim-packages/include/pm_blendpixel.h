#pragma once

#include "pm_internal.h"

PERSIMMON_NAMESPACE_BEGIN

inline uint32_t PIXEL_ALPHA(uint32_t c, int alpha)
{
    return ((c & 0xff000000) / 255 * alpha & 0xff000000) | (c & 0x00ffffff);
}

inline uint32_t COLOR_PREMUL(uint32_t c, int alpha)
{
    uint32_t a = (c >> 24) * alpha >> 8;
    return (a << 24) | (((c & 0xff00) * a >> 8) & 0xff00) | (((c & 0xff00ff) * a >> 8) & 0xff00ff);
}

inline uint32_t ALPHA_BLEND(uint32_t dst, uint32_t src)
{
    uint32_t a  = src >> 24;
    if ((a & 0xF8) == 0xF8)
    {
        return src;
    }
    else
    {
        uint32_t ia = 0xff - a;
        uint32_t rb = (dst & 0xff00ff) * ia + (src & 0xff00ff) * a;
        uint32_t g  = (dst & 0xff00) * ia + (src & 0xff00) * a;
        return ((rb >> 8) & 0xff00ff) | (g >> 8 & 0xff00)
             | ((a + (ia * (dst >> 24)) / 0xFFU) << 24);
    }
}

inline uint32_t COLOR_BLEND_PREMUL(uint32_t bg, uint32_t fg)
{
    uint32_t ia = 0xff - (fg >> 24);
    uint32_t rb = (bg & 0xff00ff) * ia + ((fg & 0xff00ff) << 8);
    uint32_t g  = (bg >> 8 & 0xff) * ia + (fg & 0xff00);
    return ((rb >> 8) & 0xff00ff) | (g & 0xff00) | 0xFF000000;
}

#ifdef PKG_PERSIMMON_SWAP_RB_CHANNEL
static inline uint32_t SWAP_RB_IF(uint32_t c)
{
    const uint32_t rb = c & 0x00ff00ff;
    return (c & 0xff00ff00) | (rb >> 16) | (rb << 16);
}
#else
#define SWAP_RB_IF(c) (c)
#endif

PERSIMMON_NAMESPACE_END
