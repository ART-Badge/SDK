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
#ifndef __RTGUI_DC_H__
#define __RTGUI_DC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <rtgui/rtgui.h>
#include <rtgui/font.h>
#include <rtgui/driver.h>

#include <rtgui/dc_buffer.h>

#define RTGUI_DC(dc)        ((struct rtgui_dc*)(dc))

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif

enum rtgui_dc_type
{
    RTGUI_DC_HW = 0x01,
    RTGUI_DC_CLIENT = 0x02,
    RTGUI_DC_BUFFER = 0x04,
    RTGUI_DC_BUFFER_RECT = 0x08,
    RTGUI_DC_MAX = 0xFFFF,
};

struct rtgui_dc_engine
{
    /* interface */
    void (*draw_point)(struct rtgui_dc *dc, int x, int y);
    void (*draw_color_point)(struct rtgui_dc *dc, int x, int y, rtgui_color_t color);
    void (*draw_vline)(struct rtgui_dc *dc, int x, int y1, int y2);
    void (*draw_hline)(struct rtgui_dc *dc, int x1, int x2, int y);
    void (*fill_rect)(struct rtgui_dc *dc, rtgui_rect_t *rect);
    void (*blit_line)(struct rtgui_dc *dc, int x1, int x2, int y, rt_uint8_t *line_data);
    void (*blit)(struct rtgui_dc *dc, struct rtgui_point *dc_point, struct rtgui_dc *dest, rtgui_rect_t *rect);

    rt_bool_t (*fini)(struct rtgui_dc *dc);
};

/*
 * The abstract device context
 *
 * Normally, a DC is a drawable canvas, user can draw point/line/cycle etc
 * on the DC.
 *
 * There are several kinds of DC:
 * - Hardware DC;
 * - Client DC;
 * - Buffer DC;
 */
//struct rtgui_dc
//{
//    /* type of device context */
//    rt_uint32_t type;
//
//    /* dc engine */
//    const struct rtgui_dc_engine *engine;
//};

/*
 * The hardware device context
 *
 * The hardware DC is a context based on hardware device, for examle the
 * LCD device. The operations on the hardware DC are reflected to the real
 * hardware.
 *
 */
//struct rtgui_dc_hw
//{
//    struct rtgui_dc parent;
//    struct rtgui_graphic_driver hw_driver;
//    rtgui_widget_t *owner;
//};

#define RTGUI_DC_FC(dc)         (rtgui_dc_get_gc(RTGUI_DC(dc))->foreground)
#define RTGUI_DC_BC(dc)         (rtgui_dc_get_gc(RTGUI_DC(dc))->background)
#define RTGUI_DC_FONT(dc)       (rtgui_dc_get_gc(RTGUI_DC(dc))->font)
#define RTGUI_DC_TEXTALIGN(dc)  (rtgui_dc_get_gc(RTGUI_DC(dc))->textalign)

void rtgui_gc_set_default(rtgui_gc_t *gc);

/* destroy a dc */
void rtgui_dc_destory(struct rtgui_dc *dc);

rt_uint8_t *rtgui_dc_buffer_get_pixel(struct rtgui_dc *dc);

void rtgui_dc_draw_line(struct rtgui_dc *dc, int x1, int y1, int x2, int y2);
void rtgui_dc_draw_rect(struct rtgui_dc *dc, struct rtgui_rect *rect);
void rtgui_dc_fill_rect_forecolor(struct rtgui_dc *dc, struct rtgui_rect *rect);
void rtgui_dc_draw_round_rect(struct rtgui_dc *dc, struct rtgui_rect *rect, int r);
void rtgui_dc_fill_round_rect(struct rtgui_dc *dc, struct rtgui_rect *rect, int r);
/** Fill a vertical gradient rect from @c1 to @c2 */
void rtgui_dc_fill_gradient_rectv(struct rtgui_dc *dc, rtgui_rect_t *rect,
                                  rtgui_color_t c1, rtgui_color_t c2);
void rtgui_dc_draw_annulus(struct rtgui_dc *dc, rt_int16_t x, rt_int16_t y, rt_int16_t r1, rt_int16_t r2, rt_int16_t start, rt_int16_t end);
void rtgui_dc_draw_pie(struct rtgui_dc *dc, rt_int16_t x, rt_int16_t y, rt_int16_t r, rt_int16_t start, rt_int16_t end);
void rtgui_dc_fill_pie(struct rtgui_dc *dc, rt_int16_t x, rt_int16_t y, rt_int16_t r, rt_int16_t start, rt_int16_t end);

void rtgui_dc_draw_text(struct rtgui_dc *dc, const char *text, rt_ubase_t len, struct rtgui_rect *rect);
void rtgui_dc_draw_text_stroke(struct rtgui_dc *dc, const char *text, struct rtgui_rect *rect,
                               rtgui_color_t color_stroke, rtgui_color_t color_core);

void rtgui_dc_draw_mono_bmp(struct rtgui_dc *dc, int x, int y, int w, int h, const rt_uint8_t *data);
void rtgui_dc_draw_byte(struct rtgui_dc *dc, int x, int y, int h, const rt_uint8_t *data);
void rtgui_dc_draw_word(struct rtgui_dc *dc, int x, int y, int h, const rt_uint8_t *data);

void rtgui_dc_draw_border(struct rtgui_dc *dc, rtgui_rect_t *rect, int flag);
void rtgui_dc_draw_horizontal_line(struct rtgui_dc *dc, int x1, int x2, int y);
void rtgui_dc_draw_vertical_line(struct rtgui_dc *dc, int x, int y1, int y2);
void rtgui_dc_draw_focus_rect(struct rtgui_dc *dc, rtgui_rect_t *rect);

void rtgui_dc_draw_polygon(struct rtgui_dc *dc, const int *vx, const int *vy, int count);
void rtgui_dc_fill_polygon(struct rtgui_dc *dc, const int *vx, const int *vy, int count);

void rtgui_dc_draw_circle(struct rtgui_dc *dc, int x, int y, int r);
void rtgui_dc_fill_circle(struct rtgui_dc *dc, rt_int16_t x, rt_int16_t y, rt_int16_t r);
void rtgui_dc_draw_arc(struct rtgui_dc *dc, rt_int16_t x, rt_int16_t y, rt_int16_t r, rt_int16_t start, rt_int16_t end);

void rtgui_dc_draw_ellipse(struct rtgui_dc *dc, rt_int16_t x, rt_int16_t y, rt_int16_t rx, rt_int16_t ry);
void rtgui_dc_fill_ellipse(struct rtgui_dc *dc, rt_int16_t x, rt_int16_t y, rt_int16_t rx, rt_int16_t ry);

/* alpha blending functions */
void rtgui_dc_draw_aa_line(struct rtgui_dc * dst, int x1, int y1, int x2, int y2);
void rtgui_dc_draw_aa_lines(struct rtgui_dc * dst, const struct rtgui_point * points, int count);

void rtgui_dc_blend_point(struct rtgui_dc * dst, int x, int y, enum RTGUI_BLENDMODE blendMode, rt_uint8_t r, rt_uint8_t g, rt_uint8_t b, rt_uint8_t a);
void rtgui_dc_blend_points(struct rtgui_dc * dst, const rtgui_point_t * points, int count, enum RTGUI_BLENDMODE blendMode, rt_uint8_t r, rt_uint8_t g, rt_uint8_t b, rt_uint8_t a);

void rtgui_dc_blend_line(struct rtgui_dc * dst, int x1, int y1, int x2, int y2, enum RTGUI_BLENDMODE blendMode, rtgui_color_t color);
void rtgui_dc_blend_lines(struct rtgui_dc * dst, const rtgui_point_t * points, int count, enum RTGUI_BLENDMODE blendMode, rtgui_color_t color);

void rtgui_dc_blend_fill_rect(struct rtgui_dc * dst, const rtgui_rect_t * rect, enum RTGUI_BLENDMODE blendMode, rtgui_color_t color);
void rtgui_dc_blend_fill_rects(struct rtgui_dc * dst, const rtgui_rect_t * rects, int count, enum RTGUI_BLENDMODE blendMode, rtgui_color_t color);

void rtgui_dc_draw_aa_circle(struct rtgui_dc *dc, rt_int16_t x, rt_int16_t y, rt_int16_t r);
void rtgui_dc_draw_aa_ellipse(struct rtgui_dc *dc, rt_int16_t  x, rt_int16_t y, rt_int16_t rx, rt_int16_t ry);

int rtgui_dc_draw_thick_line(struct rtgui_dc * dst, rt_int16_t x1, rt_int16_t y1, rt_int16_t x2, rt_int16_t y2, rt_uint8_t width);

#ifdef __cplusplus
}
#endif

#endif

