/*
 * File      : rtgui_server.h
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
#ifndef __RTGUI_SERVER_H__
#define __RTGUI_SERVER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <rtservice.h>
#include <rtgui/list.h>

void rtgui_server_init(void);
void rtgui_server_deinit(void);

void rtgui_server_set_current_app(struct rtgui_app* app);
struct rtgui_app* rtgui_server_current_app(void);

/* post an event to server */
void rtgui_server_post_touch(int x, int y, int pressed);

#ifdef __cplusplus
}
#endif

#endif
