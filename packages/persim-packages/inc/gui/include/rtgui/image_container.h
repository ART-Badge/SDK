/*
 * File      : image_container.h
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
 * 2010-04-10     Bernard      first version
 */

#ifndef __RTGUI_IMAGE_CONTAINER_H__
#define __RTGUI_IMAGE_CONTAINER_H__

#include <rtgui/rtgui.h>
#include <rtgui/image.h>
#include <rtgui/hash_table.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(GUIENGINE_IMAGE_CONTAINER)
/* image item in image container */
struct rtgui_image_item
{
    rtgui_image_t *image;
    char *filename;

    rt_uint32_t refcount;
};
typedef struct rtgui_image_item rtgui_image_item_t;

void rtgui_system_image_container_init(void);
void rtgui_system_image_container_deinit(void);
rtgui_hash_table_t *rtgui_system_image_container_get(void);

rtgui_hash_table_t *rtgui_image_container_table_create(void);
void rtgui_image_container_table_delete(rtgui_hash_table_t *table);

struct rtgui_image_item *rtgui_image_container_get(rtgui_hash_table_t *table, const char *filename);
struct rtgui_image_item *rtgui_image_container_find(rtgui_hash_table_t *table, const char *filename);
struct rtgui_image_item *rtgui_image_container_create(rtgui_hash_table_t *table, const char *filename);
rtgui_image_item_t *rtgui_image_container_refer(rtgui_image_item_t *item);
void rtgui_image_container_put(rtgui_hash_table_t *table, rtgui_image_item_t *item);
void rtgui_image_container_dump(rtgui_hash_table_t *table);

#endif

#ifdef __cplusplus
}
#endif

#endif
