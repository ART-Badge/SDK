/*
 * File      : event.h
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
#ifndef __RTGUI_EVENT_H__
#define __RTGUI_EVENT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <rtdevice.h>
#include <rtgui/rtgui.h>

/* NOTE: if you create a new event type, remember to add it into the union
 * rtgui_event_generic */
enum _rtgui_event_type
{
    RTGUI_EVENT_SHOW,                  /* the widget is going to be shown */
    RTGUI_EVENT_HIDE,                  /* the widget is going to be hidden */
    RTGUI_EVENT_PAINT,                 /* paint on screen       */
    RTGUI_EVENT_TIMER,                 /* timer                 */

    /* mouse and keyboard event */
    RTGUI_EVENT_MOUSE_MOTION,          /* mouse motion          */
    RTGUI_EVENT_MOUSE_BUTTON,          /* mouse button info     */
    RTGUI_EVENT_GESTURE,               /* gesture event         */

    /* lcd event */
    RTGUI_EVENT_TE,                    /* LCD TE sync signal    */

    /* user command event. It should always be the last command type. */
    RTGUI_EVENT_COMMAND = 0x0100,      /* user command          */

    RTGUI_EVENT_CALLBACK,
};
typedef enum _rtgui_event_type rtgui_event_type;


struct rtgui_event
{
    /* the event type */
    enum _rtgui_event_type type;
    /* user field of event */
    rt_uint16_t user;

    /* the event sender */
    struct rtgui_app *sender;

    /* mailbox to acknowledge request */
    rt_mailbox_t ack;
};
typedef struct rtgui_event rtgui_event_t;
#define RTGUI_EVENT(e)  ((struct rtgui_event*)(e))

extern struct rtgui_app* rtgui_app_self(void);

#define RTGUI_EVENT_INIT(e, t)  do      \
{                                       \
    (e)->type = (t);                    \
    (e)->user = 0;                      \
    (e)->sender = rtgui_app_self();     \
    (e)->ack = RT_NULL;                 \
} while (0)


/*
 * RTGUI Window Event
 */
#define _RTGUI_EVENT_WIN_ELEMENTS \
    struct rtgui_event parent; \

struct rtgui_event_paint
{
    _RTGUI_EVENT_WIN_ELEMENTS

    rtgui_rect_t rect;      /* rect to be updated */
};

struct rtgui_timer;
struct rtgui_event_timer
{
    struct rtgui_event parent;

    struct rtgui_timer *timer;
};
typedef struct rtgui_event_timer rtgui_event_timer_t;

#define RTGUI_EVENT_PAINT_INIT(e)           RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_PAINT)
#define RTGUI_EVENT_TIMER_INIT(e)           RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_TIMER)

#define rtgui_event_show rtgui_event
#define rtgui_event_hide rtgui_event

#define RTGUI_EVENT_SHOW_INIT(e)            RTGUI_EVENT_INIT((e), RTGUI_EVENT_SHOW)
#define RTGUI_EVENT_HIDE_INIT(e)            RTGUI_EVENT_INIT((e), RTGUI_EVENT_HIDE)

/**
 * RTGUI Gesture Event
 */
enum rtgui_gesture_type
{
    RTGUI_GESTURE_NONE      = 0x0000,
    RTGUI_GESTURE_TAP       = 0x0001,
    /* Usually used to zoom in and out. */
    RTGUI_GESTURE_LONGPRESS         = 0x0002,
    RTGUI_GESTURE_DRAG_HORIZONTAL   = 0x0004,
    RTGUI_GESTURE_DRAG_VERTICAL     = 0x0008,
    RTGUI_GESTURE_DRAG              = (RTGUI_GESTURE_DRAG_HORIZONTAL | RTGUI_GESTURE_DRAG_VERTICAL),
    /* PINCH, DRAG finished. */
    RTGUI_GESTURE_FINISH    = 0x8000,
    /* The corresponding gesture should be canceled. */
    RTGUI_GESTURE_CANCEL    = 0x4000,
    RTGUI_GESTURE_TYPE_MASK = 0x0FFF,
};

struct rtgui_event_gesture
{
    _RTGUI_EVENT_WIN_ELEMENTS
    rt_uint32_t type;
};

/*
 * RTGUI Mouse and Keyboard Event
 */
struct rtgui_event_mouse
{
    _RTGUI_EVENT_WIN_ELEMENTS

    rt_uint16_t x, y;
    rt_uint16_t button;

    /* Timestamp of this event sampled in driver. */
    rt_tick_t ts;
    /* id of touch session(from down to up). Different session should have
     * different id. id should never be 0. */
    rt_uint32_t id;
};
#define RTGUI_MOUSE_BUTTON_LEFT         0x01
#define RTGUI_MOUSE_BUTTON_RIGHT        0x02
#define RTGUI_MOUSE_BUTTON_MIDDLE       0x03
#define RTGUI_MOUSE_BUTTON_WHEELUP      0x04
#define RTGUI_MOUSE_BUTTON_WHEELDOWN    0x08

#define RTGUI_MOUSE_BUTTON_DOWN         0x10
#define RTGUI_MOUSE_BUTTON_UP           0x20

#define RTGUI_EVENT_GESTURE_INIT(e, gtype) \
    do { \
        RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_GESTURE); \
        (e)->type = gtype; \
    } while (0)

#ifndef GUIENGINE_CMD_STRING_MAX
#define GUIENGINE_CMD_STRING_MAX        16
#endif

struct rtgui_event_command
{
    _RTGUI_EVENT_WIN_ELEMENTS

    /* command type */
    rt_int32_t type;

    /* command id */
    rt_int32_t command_id;

    /* command string */
    char command_string[GUIENGINE_CMD_STRING_MAX];
};
#define RTGUI_EVENT_COMMAND_INIT(e) RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_COMMAND)

#define RTGUI_CMD_UNKNOWN       0x00
#define RTGUI_CMD_WM_CLOSE      0x10

#define RTGUI_CMD_USER_INT      0x20
#define RTGUI_CMD_USER_STRING   0x21

struct rtgui_event_callback
{
    _RTGUI_EVENT_WIN_ELEMENTS

    void *callback;     /* callback function */
    void *data;         /* callback function parameter */
};
#define RTGUI_EVENT_CALLBACK_INIT(e) RTGUI_EVENT_INIT(&((e)->parent), RTGUI_EVENT_CALLBACK)

#undef _RTGUI_EVENT_WIN_ELEMENTS

union rtgui_event_generic
{
    struct rtgui_event base;
    struct rtgui_event_paint paint;
    struct rtgui_event_timer timer;
    struct rtgui_event_mouse mouse;
    struct rtgui_event_gesture gesture;
    struct rtgui_event_command command;
    struct rtgui_event_callback callback;
};

#ifdef __cplusplus
}
#endif

#endif

