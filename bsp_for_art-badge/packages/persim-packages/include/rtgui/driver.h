/*
 * File      : driver.h
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
#ifndef __RTGUI_DRIVER_H__
#define __RTGUI_DRIVER_H__

#include <rtthread.h>

typedef unsigned long rtgui_color_t;

/* graphic driver operations */
struct rtgui_graphic_driver_ops
{
    /* set and get pixel in (x, y) */
    void (*set_pixel)(rtgui_color_t *c, int x, int y);
    void (*get_pixel)(rtgui_color_t *c, int x, int y);

    void (*draw_hline)(rtgui_color_t *c, int x1, int x2, int y);
    void (*draw_vline)(rtgui_color_t *c, int x, int y1, int y2);
    void (*fill_rect)(rtgui_color_t *c, int x1, int y1, int x2, int y2);

    /* draw raw hline */
    void (*draw_raw_hline)(rt_uint8_t *pixels, int x1, int x2, int y);
};

/* graphic extension operations */
struct rtgui_graphic_ext_ops
{
    /* some 2D operations */
    void (*draw_line)(rtgui_color_t *c, int x1, int y1, int x2, int y2);

    void (*draw_rect)(rtgui_color_t *c, int x1, int y1, int x2, int y2);
    void (*fill_rect)(rtgui_color_t *c, int x1, int y1, int x2, int y2);

    void (*draw_circle)(rtgui_color_t *c, int x, int y, int r);
    void (*fill_circle)(rtgui_color_t *c, int x, int y, int r);

    void (*draw_ellipse)(rtgui_color_t *c, int x, int y, int rx, int ry);
    void (*fill_ellipse)(rtgui_color_t *c, int x, int y, int rx, int ry);
};

struct rtgui_graphic_driver
{
    /* pixel format and byte per pixel */
    rt_uint8_t pixel_format;
    rt_uint8_t bits_per_pixel;
    rt_uint16_t pitch;

    /* screen width and height */
    rt_uint16_t width;
    rt_uint16_t height;

    /* framebuffer address and ops */
    rt_uint8_t *framebuffer;
    struct rt_device *device;

    const struct rtgui_graphic_driver_ops *ops;
    const struct rtgui_graphic_ext_ops *ext_ops;
};

#ifdef __cplusplus
extern "C" {
#endif

void pm_set_graphic_device(rt_device_t device);
void pm_vsync_send(void);
void pm_touch_send(int x, int y, rt_bool_t pressed);

static inline void rtgui_send_vsync(void)
{
    pm_vsync_send();
}

static inline void rtgui_send_touch(int x, int y, int pressed)
{
    pm_touch_send(x, y, pressed);
}

static inline rt_err_t rtgui_graphic_set_device(rt_device_t device)
{
    pm_set_graphic_device(device);
    return RT_EOK;
}

#ifdef __cplusplus
}
#endif

#endif
