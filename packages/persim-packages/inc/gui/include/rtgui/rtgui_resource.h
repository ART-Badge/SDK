/*
 * File      : rtgui_resource.h
 * This file is part of RT-Thread GUI Engine
 * COPYRIGHT (C) 2006 - 2020, RT-Thread Development Team
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-12     realthread   first version
 */

#ifndef __RTGUI_RES_H__
#define __RTGUI_RES_H__

#include <rtthread.h>

#ifdef GUIENGINE_USING_RES

#ifdef __cplusplus
extern "C" {
#endif

typedef struct res_lsit
{
    char *path;
    rt_uint8_t *res_table;
    struct res_lsit *prev;
    int fd;
#ifdef RT_USING_DFS_ROMFS
    int romfs;
#endif
    int refcnt;
} res_list_t;

typedef struct res_file_info
{
    rt_uint32_t data_offset;
    rt_uint32_t data_len;
    rt_uint32_t data_pos;
    rt_uint8_t  *path;
    res_list_t *res;
} res_file_info_t;

int resource_load(res_list_t **res_list, const char *path, const char* name);
int resource_unload(res_list_t **res_list, const char *path);
void resource_clear(res_list_t **res_list);
res_file_info_t resource_get_fileinfo(res_list_t *res_list, const char *filename);
int resource_file_read(res_file_info_t* res, void* ptr, rt_uint32_t offset, rt_size_t size);

#if 0
#include <rtgui/hash_table.h>
#include <string/string_table.h>

void res_list_set(res_list_t *res_list);
res_list_t **res_list_get(void);
void res_images_set(rtgui_hash_table_t *images);
rtgui_hash_table_t *res_images_get(void);
void res_xmls_set(struct string_table *table);
struct string_table *res_xmls_get(void);
void res_js_set(struct string_table *table);
struct string_table *res_js_get(void);
#endif

#ifdef __cplusplus
}
#endif

#endif

#endif
