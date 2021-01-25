/*
 * File      : blit.h
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
 * 2013-10-04     Bernard      porting SDL software render to RT-Thread GUI
 */

/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef __RTGUI_BLIT_H__
#define __RTGUI_BLIT_H__

#include <rtgui/rtgui.h>
#include <rtgui/dc.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 4-times unrolled loop */
#define DUFFS_LOOP4(pixel_copy_increment, width)                        \
{ int n = (width+3)/4;                                                  \
    switch (width & 3) {                                                \
    case 0: do {    pixel_copy_increment;                               \
    case 3:     pixel_copy_increment;                                   \
    case 2:     pixel_copy_increment;                                   \
    case 1:     pixel_copy_increment;                                   \
        } while (--n > 0);                                              \
    }                                                                   \
}

/* 8-times unrolled loop */
#define DUFFS_LOOP8(pixel_copy_increment, width)                        \
{ int n = (width+7)/8;                                                  \
    switch (width & 7) {                                                \
    case 0: do {    pixel_copy_increment;                               \
    case 7:     pixel_copy_increment;                                   \
    case 6:     pixel_copy_increment;                                   \
    case 5:     pixel_copy_increment;                                   \
    case 4:     pixel_copy_increment;                                   \
    case 3:     pixel_copy_increment;                                   \
    case 2:     pixel_copy_increment;                                   \
    case 1:     pixel_copy_increment;                                   \
        } while ( --n > 0 );                                            \
    }                                                                   \
}

/* Use the 8-times version of the loop by default */
#define DUFFS_LOOP(pixel_copy_increment, width)                         \
    DUFFS_LOOP8(pixel_copy_increment, width)

struct rtgui_blit_info
{
    rt_uint8_t *src;
    int src_x, src_y;
    int src_w, src_h;
    int src_pitch;
    int src_skip;

    rt_uint8_t *dst;
    int dst_x, dst_y;
    int dst_w, dst_h;
    int dst_pitch;
    int dst_skip;

    rt_uint32_t *lut; /* ARGB32 LUT buffer */
    rt_uint16_t lut_size;

    rt_uint8_t src_fmt;
    rt_uint8_t dst_fmt;
    rt_uint8_t r, g, b, a;
};

struct rtgui_image_info
{
    rt_uint8_t *pixels;
    int src_pitch;
    int x, y;

    rt_uint32_t* lut; /* ARGB32 LUT buffer */
    rt_uint16_t lut_size;

    rt_uint8_t src_fmt;
    rt_uint8_t a;
};

struct rtgui_alpha_info
{
    rt_uint8_t *pixels;
    int src_pitch;
    int x, y;

    rtgui_color_t color;
};

typedef void (*rtgui_blit_line_func)(rt_uint8_t *dst, rt_uint8_t *src, int line);
rtgui_blit_line_func rtgui_blit_line_get(int dst_bpp, int src_bpp);
rtgui_blit_line_func rtgui_blit_line_get_inv(int dst_bpp, int src_bpp);

void rtgui_blit(struct rtgui_blit_info * info);
void rtgui_image_info_blit(struct rtgui_image_info *image, struct rtgui_dc *dc, struct rtgui_rect *rect);
void rtgui_color_info_blit(rtgui_color_t color, struct rtgui_dc *dc, struct rtgui_rect *rect);
void rtgui_alpha_info_blit(struct rtgui_alpha_info *ttf, struct rtgui_dc *dc, struct rtgui_rect *rect);
void rtgui_dc_blit_span(struct rtgui_dc *dc, rtgui_color_t * color, int x, int y, int width);
void rtgui_dc_read_span(struct rtgui_dc *dc, rtgui_color_t *color, int x, int y, int width);

#ifdef __cplusplus
}
#endif

#endif

