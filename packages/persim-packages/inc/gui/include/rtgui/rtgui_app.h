/*
 * File      : rtgui_app.h
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
 * 2012-01-13     Grissiom     first version
 */

#ifndef __RTGUI_APP_H__
#define __RTGUI_APP_H__

#include <rtthread.h>
#include <rtgui/rtgui.h>
#include <rtgui/event.h>
#include <rtgui/rtgui_system.h>
//#include <rtgui/rtgui_object.h>

#ifdef __cplusplus
extern "C" {
#endif

struct rtgui_app
{
    /* application name */
    unsigned char *name;
    /* the thread id */
    rt_thread_t tid;
    /* the message queue of thread */
    rt_mq_t mq;
    /* event buffer */
    rt_uint8_t event_buffer[sizeof(union rtgui_event_generic)];

    void *user_data;
};

/**
 * create an application named @myname on current thread.
 *
 * @param name the name of the application
 *
 * @return a pointer to struct rtgui_app on success. RT_NULL on failure.
 */
struct rtgui_app *rtgui_app_create(const char *name);
void rtgui_app_destroy(struct rtgui_app *app);

/**
 * return the rtgui_app struct on current thread
 */
struct rtgui_app *rtgui_app_self(void);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __RTGUI_APP_H__ */

