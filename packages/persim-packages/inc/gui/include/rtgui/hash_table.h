/*
 * File      : hash_table.h
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
 * 2020-09-10     realthread
 */

#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int (*rtgui_hash_func_t)(const void *key);
typedef rt_bool_t (*rtgui_equal_func_t)(const void *a, const void *b);
typedef void (*rtgui_user_func_t)(const void *value, const void *data);

typedef struct _gui_hash_node rtgui_hash_node_t;
typedef struct _rtgui_hash_table  rtgui_hash_table_t;

/*
 *Hash tables
 */
rtgui_hash_table_t *hash_table_create(rtgui_hash_func_t hash_func, rtgui_equal_func_t key_equal_func);
void hash_table_destroy(rtgui_hash_table_t *hash_table);

void *hash_table_find(rtgui_hash_table_t *hash_table, const void *key);
void hash_table_insert(rtgui_hash_table_t *hash_table, const void *key, void *value);
rt_bool_t hash_table_remove(rtgui_hash_table_t *hash_table, const void *key);

void hash_table_foreach(rtgui_hash_table_t *hash_table, rtgui_user_func_t user_func, void *data);
unsigned int hash_table_get_size(rtgui_hash_table_t *hash_table);

unsigned int string_hash_func(const void *self);
rt_bool_t string_equal_func(const void *a, const void *b);

#ifdef __cplusplus
}
#endif

#endif
