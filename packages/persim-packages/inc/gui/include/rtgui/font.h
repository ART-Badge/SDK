/*
 * File      : font.h
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
#ifndef __RTGUI_FONT_H__
#define __RTGUI_FONT_H__

#include <rtgui/rtgui.h>
#include <rtgui/list.h>


#ifdef __cplusplus
extern "C" {
#endif

struct rtgui_font;
struct rtgui_dc;
struct rtgui_rect;

struct rtgui_font_engine
{
    /* font engine function */
    int(*font_init)(struct rtgui_font *font);
    int(*font_load)(struct rtgui_font *font);
    void(*font_deinit)(struct rtgui_font *font);

    void(*font_draw_text)(struct rtgui_font *font, struct rtgui_dc *dc, const char *text,
                          rt_ubase_t len, struct rtgui_rect *rect);
    void(*font_get_metrics)(struct rtgui_font *font, const char *text,
                            rt_ubase_t len, struct rtgui_rect *rect);
    int(*font_get_strlen)(struct rtgui_font *font, const char *text,
                          rt_ubase_t len, rt_uint16_t width);
};

/*
 * default bitmap font engine
 */
struct rtgui_font_bitmap
{
    const rt_uint8_t *bmp;      /* bitmap font data */

    rt_uint16_t width;          /* font width  */
    rt_uint16_t height;         /* font height */

    rt_uint8_t first_char;
    rt_uint8_t last_char;
    rt_uint16_t reserved;
};
extern const struct rtgui_font_engine bmp_font_engine;

#include <rtgui/tree.h>
SPLAY_HEAD(cache_tree, hz_cache);
struct hz_cache
{
    SPLAY_ENTRY(hz_cache) hz_node;

    rt_uint16_t hz_id;
};

struct rtgui_hz_file_font
{
    struct cache_tree cache_root;
    rt_uint16_t cache_size;

    /* font size */
    rt_uint16_t font_size;
    rt_uint16_t font_data_size;

    /* file descriptor */
    struct rtgui_filerw *frw;

    /* font file name */
    const char *font_fn;
};

#ifdef GUIENGINE_USING_HZ_FILE
extern const struct rtgui_font_engine rtgui_hz_file_font_engine;
#endif

struct rtgui_font
{
    /* font name */
    char *family;
    /* font height */
    rt_uint16_t height;
    /* refer count */
    rt_uint32_t refer_count;
    /* font engine */
    const struct rtgui_font_engine *engine;
    /* font private data */
    void *data;
    /* the font list */
    rtgui_list_t list;
};
typedef struct rtgui_font rtgui_font_t;

void rtgui_font_system_init(void);
void rtgui_font_system_deinit(void);
int rtgui_font_system_add_font(struct rtgui_font *font);
void rtgui_font_system_remove_font(struct rtgui_font *font);
struct rtgui_font *rtgui_font_default(void);
void rtgui_font_set_defaut(struct rtgui_font *font);

struct rtgui_font *rtgui_font_create(const char *file, const char *family, rt_uint16_t height);
struct rtgui_font *rtgui_font_refer(const char *family, rt_uint16_t height);
void rtgui_font_derefer(struct rtgui_font *font);

/* draw a text */
void rtgui_font_draw(struct rtgui_font *font, struct rtgui_dc *dc, const char *text, rt_ubase_t len, struct rtgui_rect *rect);
int  rtgui_font_get_string_width(struct rtgui_font *font, const char *text, rt_ubase_t len);
void rtgui_font_get_metrics(struct rtgui_font *font, const char *text, rt_ubase_t len, rtgui_rect_t *rect);
int rtgui_font_get_strlen(struct rtgui_font *font, const char *text, rt_ubase_t len, rt_uint16_t width);

/* used by stract font */
#define FONT_BMP_DATA_BEGIN
#define FONT_BMP_DATA_END

struct rtgui_char_position
{
    /* Keep the size of this struct within 4 bytes so it can be passed by
     * value. */
    /* How long this char is. */
    rt_uint16_t char_width;
    /* How many bytes remaining from current pointer. At least, it will be 1. */
    rt_uint16_t remain;
};

/*
 * @len the length of @str.
 * @offset the char offset on the string to check with.
 */
struct rtgui_char_position _string_char_width(char *str, rt_size_t len, rt_size_t offset);

#ifdef __cplusplus
}
#endif

#endif

