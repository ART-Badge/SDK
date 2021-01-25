/*
 * File      : dc.h
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
 */
#ifndef __RTGUI_DC_XYZ_H__
#define __RTGUI_DC_XYZ_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <rtgui/rtgui.h>
#include <rtgui/font.h>
#include <rtgui/driver.h>
#include <rtgui/region.h>

struct rtgui_dc
{
    /* pixel format */
    rt_uint8_t pixel_format;
    /* pixel alpha */
    rt_uint8_t pixel_alpha;
    /* pitch */
    rt_uint16_t pitch;
    /* width and height */
    rt_uint16_t width, height;
    /* pixel data */
    rt_uint8_t *pixel;
    /* graphic context */
    rtgui_gc_t *gc;
    /* clip information */
    rtgui_region_t *clip;
    rtgui_rect_t extent;
    rt_uint8_t flags;
#ifdef GUIENGINE_IMAGE_CONTAINER
    struct rtgui_image_item *image;
#endif
};

void rtgui_dc_init(struct rtgui_dc *dc, rt_uint8_t pixel_format, int w, int h, rt_uint8_t *pixel);

void rtgui_dc_set_gc(struct rtgui_dc *dc, rtgui_gc_t *gc);
rtgui_gc_t *rtgui_dc_get_gc(struct rtgui_dc *dc);

void rtgui_dc_set_clip(struct rtgui_dc *dc, rtgui_region_t *clip);
void rtgui_dc_get_rect(struct rtgui_dc *dc, rtgui_rect_t *rect);

rt_uint8_t rtgui_dc_get_pixel_format(struct rtgui_dc *dc);

/*
 * draw a point on dc
 */
void rtgui_dc_draw_point(struct rtgui_dc *dc, int x, int y);

/*
 * draw a color point on dc
 */
void rtgui_dc_draw_color_point(struct rtgui_dc *dc, int x, int y, rtgui_color_t color);

/*
 * draw a vertical line on dc
 */
void rtgui_dc_draw_vline(struct rtgui_dc *dc, int x, int y1, int y2);

/*
 * draw a horizontal line on dc
 */
void rtgui_dc_draw_hline(struct rtgui_dc *dc, int x1, int x2, int y);

/*
 * fill a rect with background color
 */
void rtgui_dc_fill_rect(struct rtgui_dc *dc, struct rtgui_rect *rect);

/*
 * blit a dc (x, y) on another dc (rect)
 */
void rtgui_dc_blit(struct rtgui_dc *src, struct rtgui_point *point, struct rtgui_dc *dest, rtgui_rect_t *rect);

void rtgui_color_blit(rtgui_color_t color, struct rtgui_dc *dc, struct rtgui_rect *rect);


struct rtgui_dc* rtgui_dc_create(int w, int h);
struct rtgui_dc* rtgui_dc_create_pixformat(rt_uint8_t pixel_format, int w, int h);
void rtgui_dc_clear(struct rtgui_dc *dc);
struct rtgui_dc *rtgui_dc_create_from_dc(struct rtgui_dc* dc);
struct rtgui_dc *rtgui_dc_create_by_rect(struct rtgui_dc *dc, struct rtgui_rect *rect);
void rtgui_color_blit(rtgui_color_t color, struct rtgui_dc *dc, struct rtgui_rect *rect);

struct rtgui_dc *test_xyz(void);

#ifdef __cplusplus
}
#endif

#endif

