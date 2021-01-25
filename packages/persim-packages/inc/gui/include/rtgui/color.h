/*
 * File      : color.h
 * This file is part of RT-Thread GUI Engine
 * COPYRIGHT (C) 2006 - 2020, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-10-16     Bernard      first version
 * 2012-01-24     onelife      add mono color support
 */
#ifndef __RTGUI_COLOR_H__
#define __RTGUI_COLOR_H__

#include <rtgui/rtgui.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The color used in the GUI:
 *
 *         bit        bit
 * RGB565  15 R,G,B   0
 * BGR565  15 B,G,R   0
 * RGB888  23 R,G,B   0
 * ARGB888 31 A,R,G,B 0
 * RGBA888 31 R,G,B,A 0
 * ABGR888 31 A,B,G,R 0
 *
 * The rtgui_color is defined as ARGB888.
 *        bit31 A,R,G,B bit0
 */
#define RTGUI_ARGB(a, r, g, b)                      \
        ((rtgui_color_t)(((rt_uint8_t)(b)|          \
        (((rtgui_color_t)(rt_uint8_t)(g))<<8))|     \
        (((rtgui_color_t)(rt_uint8_t)(r))<<16)|     \
        (((rtgui_color_t)(rt_uint8_t)(a))<<24)))
#define RTGUI_RGB(r, g, b)  RTGUI_ARGB(255, (r), (g), (b))

#ifndef RTGUI_COLOR_SWAP_RB
#define RTGUI_RGB_B(c)  ((c) & 0xff)
#define RTGUI_RGB_G(c)  (((c) >> 8)  & 0xff)
#define RTGUI_RGB_R(c)  (((c) >> 16) & 0xff)
#define RTGUI_RGB_A(c)  (((c) >> 24) & 0xff)
#else
#define RTGUI_RGB_B(c)  (((c) >> 16) & 0xff)
#define RTGUI_RGB_G(c)  (((c) >> 8)  & 0xff)
#define RTGUI_RGB_R(c)  ((c) & 0xff)
#define RTGUI_RGB_A(c)  (((c) >> 24) & 0xff)
#endif

 /* Assemble R-G-B values into a specified pixel format and store them */
#ifdef RTGUI_RGB565_CHANGE_ENDIAN
#define RGB565_FROM_RGB(Pixel, r, g, b)                         \
{                                                               \
    Pixel = (((r>>3)<<3)|((g>>5)|((g>>2)<<13))|((b>>3)<<8));    \
}
#else
#define RGB565_FROM_RGB(Pixel, r, g, b)                         \
{                                                               \
    Pixel = ((r>>3)<<11)|((g>>2)<<5)|(b>>3);                    \
}
#endif

#define BGR565_FROM_RGB(Pixel, r, g, b)         \
{                                               \
    Pixel = ((b>>3)<<11)|((g>>2)<<5)|(r>>3);    \
}

#ifdef PKG_USING_RGB888_PIXEL_BITS_32
#define RGB888_FROM_RGB(Pixel, r, g, b)     \
{                                           \
    Pixel = (r<<16)|(g<<8)|b;               \
}
#else
#define RGB888_FROM_RGB(Pixel, r, g, b)     \
{                                           \
    rt_uint8_t *p = &Pixel;                 \
    *p++ = r;                               \
    *p++ = g;                               \
    *p = b;                                 \
}
#endif

#define ARGB8888_FROM_RGBA(Pixel, r, g, b, a)   \
{                                               \
    Pixel = (a<<24)|(r<<16)|(g<<8)|b;           \
}

/* Load pixel of the specified format from a buffer and get its R-G-B values */
#ifdef RTGUI_RGB565_CHANGE_ENDIAN
#define RGB_FROM_RGB565(Pixel, r, g, b)     \
{                                           \
    rt_uint16_t swap_data;                  \
    swap_data = (((Pixel & 0x00ff) << 8)    \
                + ((Pixel & 0xff00) >> 8)); \
    r = ((swap_data&0xF800)>>8);            \
    g = ((swap_data&0x07E0)>>3);            \
    b = (swap_data&0x001F)<<3;              \
}
#else
#define RGB_FROM_RGB565(Pixel, r, g, b)     \
{                                           \
    r = ((Pixel&0xF800)>>8);                \
    g = ((Pixel&0x07E0)>>3);                \
    b = (Pixel&0x001F)<<3;                  \
}
#endif

#define RGB_FROM_BGR565(Pixel, r, g, b)     \
{                                           \
    b = ((Pixel&0xF800)>>8);                \
    g = ((Pixel&0x07E0)>>3);                \
    r = (Pixel&0x001F)<<3;                  \
}

#ifdef PKG_USING_RGB888_PIXEL_BITS_32
#define RGB_FROM_RGB888(Pixel, r, g, b)     \
{                                           \
    r = ((Pixel>>16)&0xFF);                 \
    g = ((Pixel>>8)&0xFF);                  \
    b = (Pixel&0xFF);                       \
}
#else
#define RGB_FROM_RGB888(Pixel, r, g, b)     \
{                                           \
    rt_uint8_t *p = &Pixel;                 \
    r = *p++;                               \
    g = *p++;                               \
    b = *p;                                 \
}
#endif

#define RGBA_FROM_ARGB8888(Pixel, r, g, b, a)   \
{                                               \
    a = (Pixel>>24);                            \
    r = ((Pixel>>16)&0xFF);                     \
    g = ((Pixel>>8)&0xFF);                      \
    b = (Pixel&0xFF);                           \
}


extern const rtgui_color_t default_foreground;
extern const rtgui_color_t default_background;

/* it's better use these color definitions */
#define RED                RTGUI_RGB(0xff, 0x00, 0x00)
#define GREEN              RTGUI_RGB(0x00, 0xff, 0x00)
#define BLUE               RTGUI_RGB(0x00, 0x00, 0xff)
#define BLACK              RTGUI_RGB(0x00, 0x00, 0x00)
#define WHITE              RTGUI_RGB(0xff, 0xff, 0xff)
#define HIGH_LIGHT         RTGUI_RGB(0xfc, 0xfc, 0xfc)
#define DARK_GREY          RTGUI_RGB(0x7f, 0x7f, 0x7f)
#define LIGHT_GREY         RTGUI_RGB(0xc0, 0xc0, 0xc0)

/*
 * RTGUI default color format: ARGB
 * AAAA AAAA RRRR RRRR GGGG GGGG BBBB BBBB
 * 31                                    0
 */

/* convert rtgui color to mono */
rt_inline rt_uint8_t rtgui_color_to_mono(rtgui_color_t c)
{
    rt_uint8_t pixel;

    pixel = (RTGUI_RGB_R(c) | RTGUI_RGB_G(c) | RTGUI_RGB_B(c)) ? 0x01 : 0x00;
    return pixel;
}

rt_inline rtgui_color_t rtgui_color_from_mono(rt_uint8_t pixel)
{
    rtgui_color_t color;

    if (pixel)
    {
        color = WHITE;
    }
    else
    {
        color = BLACK;
    }
    return color;
}

/* convert rtgui color to RRRRRGGGGGGBBBBB */
rt_inline rt_uint16_t rtgui_color_to_565(rtgui_color_t c)
{
    rt_uint16_t pixel;

    RGB565_FROM_RGB(pixel, RTGUI_RGB_R(c), RTGUI_RGB_G(c), RTGUI_RGB_B(c));
    return pixel;
}

rt_inline rtgui_color_t rtgui_color_from_565(rt_uint16_t pixel)
{
    rt_uint8_t r, g, b;
    rtgui_color_t color;

    RGB_FROM_RGB565(pixel, r, g, b);
    color = RTGUI_RGB(r, g, b);
    return color;
}

/* convert rtgui color to BBBBBGGGGGGRRRRR */
rt_inline rt_uint16_t rtgui_color_to_565p(rtgui_color_t c)
{
    rt_uint16_t pixel;

    BGR565_FROM_RGB(pixel, RTGUI_RGB_R(c), RTGUI_RGB_G(c), RTGUI_RGB_B(c));
    return pixel;
}

rt_inline rtgui_color_t rtgui_color_from_565p(rt_uint16_t pixel)
{
    rt_uint8_t r, g, b;
    rtgui_color_t color;

    RGB_FROM_BGR565(pixel, r, g, b);
    color = RTGUI_RGB(r, g, b);
    return color;
}

/* convert rtgui color to RGB */
rt_inline rt_uint32_t rtgui_color_to_888(rtgui_color_t c)
{
    rt_uint32_t pixel;

    pixel = RTGUI_RGB(RTGUI_RGB_R(c), RTGUI_RGB_G(c), RTGUI_RGB_B(c));
    return pixel;
}

rt_inline rtgui_color_t rtgui_color_from_888(rt_uint32_t pixel)
{
    rtgui_color_t color;

    color = RTGUI_RGB(RTGUI_RGB_R(pixel), RTGUI_RGB_G(pixel), RTGUI_RGB_B(pixel));
    return color;
}

/* get the bits of specified pixle format */
rt_uint8_t rtgui_color_get_bits(rt_uint8_t pixel_format) RTGUI_PURE;
/* get the bytes of specified pixle format */
rt_uint8_t rtgui_color_get_bpp(rt_uint8_t pixel_format) RTGUI_PURE;

#ifdef __cplusplus
}
#endif

#endif

