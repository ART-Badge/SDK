/*
 * File      : filerw.h
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
 * 2020-09-01     realthread
 */
#ifndef __RTGUI_FILERW_H__
#define __RTGUI_FILERW_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef GUIENGINE_USING_DFS_FILERW
#ifdef _WIN32_NATIVE
#pragma warning(disable: 4996)
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#else
#include <dfs_posix.h>
#endif
#endif

#include <rtgui/rtgui.h>

#define RTGUI_FILE_SEEK_SET 0
#define RTGUI_FILE_SEEK_CUR 1
#define RTGUI_FILE_SEEK_END 2

#define RTGUI_PRC_OK                    RT_EOK
#define RTGUI_PRC_PATH_ERR              1
#define RTGUI_PRC_UNCLOSED_FILE_ERR     2
#define RTGUI_PRC_RELOAD                3
#define RTGUI_PRC_UNKNOW_ERR            RT_ERROR

typedef struct rtgui_filerw rtgui_filerw_t;

struct rtgui_file_ops
{
    int(*seek)(rtgui_filerw_t *context, rt_off_t offset, int whence);
    int(*read)(rtgui_filerw_t *context, void *buffer, rt_size_t size, rt_size_t count);
    int(*write)(rtgui_filerw_t *context, const void *buffer, rt_size_t size, rt_size_t count);
    void*(*get)(rtgui_filerw_t *context);
    int(*tell)(rtgui_filerw_t *context);
    int(*eof)(rtgui_filerw_t *context);
    int(*size)(rtgui_filerw_t *context);
    int(*close)(rtgui_filerw_t *context);
};

struct rtgui_filerw
{
    const struct rtgui_file_ops* ops;
};

int rtgui_filerw_init(void);
int rtgui_filerw_register(const char *path, void *filerw_create);

rtgui_filerw_t *rtgui_filerw_create_std(const char *filename, const char *mode);
#ifdef RT_USING_DFS_ROMFS
rtgui_filerw_t *rtgui_filerw_create_romfs(const char *filename, const char *mode);
#endif
int rtgui_filerw_register_std(const char *path);
int rtgui_filerw_register_romfs(const char *path);

#ifdef GUIENGINE_USING_RES
rtgui_filerw_t *rtgui_filerw_create_res(const char *filename, const char *mode);
rtgui_filerw_t* rtgui_filerw_create_by_res_list(void *list, const char* filename);
const char* rtgui_filerw_res_path(const char *filename);
int rtgui_filerw_register_res(const char *path);
#endif

rtgui_filerw_t *rtgui_filerw_create_mem(const rt_uint8_t *mem, rt_size_t size);
/* get memory data from filerw memory object */
const rt_uint8_t *rtgui_filerw_mem_getdata(rtgui_filerw_t *context);

rtgui_filerw_t *rtgui_filerw_create(const char *filename, const char *mode);
int rtgui_filerw_read_data(const char *filename, void **buffer);
int rtgui_filerw_seek(rtgui_filerw_t *context, rt_off_t offset, int whence);
int rtgui_filerw_read(rtgui_filerw_t *context, void *buffer, rt_size_t size, rt_size_t count);
int rtgui_filerw_write(rtgui_filerw_t *context, const void *buffer, rt_size_t size, rt_size_t count);
void *rtgui_filerw_get(rtgui_filerw_t *context);
int rtgui_filerw_tell(rtgui_filerw_t *context);
int rtgui_filerw_eof(rtgui_filerw_t *context);
int rtgui_filerw_size(rtgui_filerw_t *context);
int rtgui_filerw_close(rtgui_filerw_t *context);
int rtgui_filerw_unlink(const char *filename);
const char* rtgui_filerw_path(const char *filename);

/* prc */
int rtgui_read_file(const char *filename, char **ptr);
int rtgui_load_resource(const char *path);
int rtgui_unload_resource(const char *path);
void rtgui_resource_clear(void);

#ifdef __cplusplus
}
#endif

#endif
