/*
 * File      : rtgui_res_public.h
 * This file is part of RT-Thread GUI Engine
 * COPYRIGHT (C) 2006 - 2020, RT-Thread Development Team
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-12     realthread   first version
 */

#ifndef __RTGUI_RES_P_H__
#define __RTGUI_RES_P_H__

#include <rtgui/rtgui_resource.h>

#ifdef __cplusplus
extern "C" {
#endif

const char *res_public_url(void);
void res_public_init(const char *path);
void res_public_deinit(void);

#ifdef GUIENGINE_USING_RES
struct res_lsit *res_public_list(void);
int res_public_load(const char *path);
int res_public_unload(const char *path);
void res_public_clear(void);
#endif

#ifdef __cplusplus
}
#endif

#endif
