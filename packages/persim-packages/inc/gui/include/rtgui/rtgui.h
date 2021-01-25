/*
 * File      : rtgui.h
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
 * 2009-10-04     Bernard      first version
 */
#ifndef __RT_GUI_H__
#define __RT_GUI_H__

#include <rtthread.h>
#include <rtgui/rtgui_config.h>

#ifdef __cplusplus
extern "C" {
#endif

#define _UI_MIN(x, y)           (((x)<(y))?(x):(y))
#define _UI_MAX(x, y)           (((x)>(y))?(x):(y))
#define _UI_BITBYTES(bits)      ((bits + 7)/8)
#define _UI_ABS(x)              ((x)>=0? (x):-(x))

/* MDK, GCC and MSVC all support __restrict keyword. */
#define RTGUI_RESTRICT      __restrict
#ifdef _MSC_VER
#define RTGUI_PURE
#else
/* GCC and MDK share the same attributes.
 * TODO: IAR attributes. */
#define RTGUI_PURE __attribute__((pure))
#endif

struct rtgui_event;
struct rtgui_font;

/**
 * Coordinate point
 */
struct rtgui_point
{
    rt_int16_t x, y;
};
typedef struct rtgui_point rtgui_point_t;
extern rtgui_point_t rtgui_empty_point;

/**
* line segment
*/
struct rtgui_line
{
    rtgui_point_t start, end;
};
typedef struct rtgui_line rtgui_line_t;

/**
 * Rectangle structure
 */
struct rtgui_rect
{
    rt_int16_t x1, y1, x2, y2;
};
typedef struct rtgui_rect rtgui_rect_t;
extern rtgui_rect_t rtgui_empty_rect;

#define rtgui_rect_width(r)     ((r).x2 - (r).x1)
#define rtgui_rect_height(r)    ((r).y2 - (r).y1)

typedef unsigned long rtgui_color_t;

/**
 * Graphic context
 */
struct rtgui_gc
{
    /* foreground and background color */
    rtgui_color_t foreground, background;
    rt_uint16_t textstyle; /* text style */
    rt_uint16_t textalign; /* text align */
    rtgui_rect_t clip; /* drawing clip rect */
    struct rtgui_font *font; /* font */
};
typedef struct rtgui_gc rtgui_gc_t;

enum RTGUI_MARGIN_STYLE
{
    RTGUI_MARGIN_LEFT   = 0x01,
    RTGUI_MARGIN_RIGHT  = 0x02,
    RTGUI_MARGIN_TOP    = 0x04,
    RTGUI_MARGIN_BOTTOM = 0x08,
    RTGUI_MARGIN_ALL = RTGUI_MARGIN_LEFT | RTGUI_MARGIN_RIGHT | RTGUI_MARGIN_TOP | RTGUI_MARGIN_BOTTOM
};

/**
 * Border style
 */
enum RTGUI_BORDER_STYLE
{
    RTGUI_BORDER_NONE = 0,
    RTGUI_BORDER_SIMPLE,
    RTGUI_BORDER_RAISE,
    RTGUI_BORDER_SUNKEN,
    RTGUI_BORDER_BOX,
    RTGUI_BORDER_STATIC,
    RTGUI_BORDER_EXTRA,
    RTGUI_BORDER_UP,
    RTGUI_BORDER_DOWN
};
#define RTGUI_BORDER_DEFAULT_WIDTH  2
#define RTGUI_WIDGET_DEFAULT_MARGIN 3

/**
 * Blend mode
 */
enum RTGUI_BLENDMODE
{
    RTGUI_BLENDMODE_NONE = 0x00,
    RTGUI_BLENDMODE_BLEND,
    RTGUI_BLENDMODE_ADD,
    RTGUI_BLENDMODE_MOD,
};

/**
 * Orientation
 */
enum RTGUI_ORIENTATION
{
    RTGUI_HORIZONTAL        = 0x01,
    RTGUI_VERTICAL          = 0x02,
    RTGUI_ORIENTATION_BOTH  = RTGUI_HORIZONTAL | RTGUI_VERTICAL
};

enum RTGUI_ALIGN
{
    RTGUI_ALIGN_NOT                 = 0x00,
    RTGUI_ALIGN_CENTER_HORIZONTAL   = 0x01,
    RTGUI_ALIGN_LEFT                = RTGUI_ALIGN_NOT,
    RTGUI_ALIGN_TOP                 = RTGUI_ALIGN_NOT,
    RTGUI_ALIGN_RIGHT               = 0x02,
    RTGUI_ALIGN_BOTTOM              = 0x04,
    RTGUI_ALIGN_CENTER_VERTICAL     = 0x08,
    RTGUI_ALIGN_CENTER              = RTGUI_ALIGN_CENTER_HORIZONTAL | RTGUI_ALIGN_CENTER_VERTICAL,
    RTGUI_ALIGN_EXPAND              = 0x10,
    RTGUI_ALIGN_STRETCH             = 0x20,
    RTGUI_ALIGN_TTF_SIZE            = 0x40,
};

enum RTGUI_TEXTSTYLE
{
    RTGUI_TEXTSTYLE_NORMAL          = 0x00,
    RTGUI_TEXTSTYLE_DRAW_BACKGROUND = 0x01,
    RTGUI_TEXTSTYLE_SHADOW          = 0x02,
    RTGUI_TEXTSTYLE_OUTLINE         = 0x04,
};

enum RTGUI_MODAL_CODE
{
    RTGUI_MODAL_OK,
    RTGUI_MODAL_CANCEL,
    RTGUI_MODAL_MAX = 0xFFFF,
};
typedef enum RTGUI_MODAL_CODE rtgui_modal_code_t;

#if defined(PKG_USING_PERSIMMON_SRC) && defined(PM_USE_RENDER_TIME_COUNTER)
void pm_add_render_time(const char *msg, int us);
int pm_begin_render_time(void);
void pm_end_render_time(int index, const char *msg, int us);
rt_uint64_t rtgui_micro_sec_get(void);

#define PM_PERF_COUNTER_BEGIN() \
    rt_uint64_t __pm_tick__ = rtgui_micro_sec_get()

#define PM_PERF_COUNTER_END(msg) \
    pm_add_render_time((msg), rtgui_micro_sec_get() - __pm_tick__)

#define PM_PERF_COUNTER_REC_BEGIN() \
    rt_uint64_t __pm_tick__ = rtgui_micro_sec_get(); \
    int __pm_index__ = pm_begin_render_time()

#define PM_PERF_COUNTER_REC_END(msg) \
    pm_end_render_time(__pm_index__, (msg), rtgui_micro_sec_get() - __pm_tick__)

#else
#define PM_PERF_COUNTER_BEGIN()
#define PM_PERF_COUNTER_END(msg)
#define PM_PERF_COUNTER_REC_BEGIN()
#define PM_PERF_COUNTER_REC_END(msg)
#endif

#ifdef __cplusplus
}
#endif

#endif

