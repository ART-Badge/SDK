/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2020-12-09     WillianChan   first version
 */

#include <rtthread.h>
#include <board.h>

#include "flexible_button.h"
#include "simulator_touch.h"

#ifndef PIN_KEY_UP
#define PIN_KEY_UP      P2_4
#endif

#ifndef PIN_KEY_DOWN
#define PIN_KEY_DOWN    P2_5
#endif

#ifndef PIN_KEY_LEFT
#define PIN_KEY_LEFT    P2_6
#endif

#ifndef PIN_KEY_RIGHT
#define PIN_KEY_RIGHT   P2_7
#endif

#define ENUM_TO_STR(e) (#e)

typedef enum
{
    USER_BUTTON_UP = 0,
    USER_BUTTON_DOWN,
    USER_BUTTON_LEFT,
    USER_BUTTON_RIGHT,
    USER_BUTTON_MAX
} user_button_t;

static char *enum_event_string[] = {
    ENUM_TO_STR(FLEX_BTN_PRESS_DOWN),
    ENUM_TO_STR(FLEX_BTN_PRESS_CLICK),
    ENUM_TO_STR(FLEX_BTN_PRESS_DOUBLE_CLICK),
    ENUM_TO_STR(FLEX_BTN_PRESS_REPEAT_CLICK),
    ENUM_TO_STR(FLEX_BTN_PRESS_SHORT_START),
    ENUM_TO_STR(FLEX_BTN_PRESS_SHORT_UP),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_START),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_UP),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_HOLD),
    ENUM_TO_STR(FLEX_BTN_PRESS_LONG_HOLD_UP),
    ENUM_TO_STR(FLEX_BTN_PRESS_MAX),
    ENUM_TO_STR(FLEX_BTN_PRESS_NONE),
};

static char *enum_btn_id_string[] = {
    ENUM_TO_STR(USER_BUTTON_UP),
    ENUM_TO_STR(USER_BUTTON_DOWN),
    ENUM_TO_STR(USER_BUTTON_LEFT),
    ENUM_TO_STR(USER_BUTTON_RIGHT),
    ENUM_TO_STR(USER_BUTTON_MAX),
};

static flex_button_t user_button[USER_BUTTON_MAX];

static uint8_t common_btn_read(void *arg)
{
    uint8_t value = 0;

    flex_button_t *btn = (flex_button_t *)arg;

    switch (btn->id)
    {
    case USER_BUTTON_UP:
        value = rt_pin_read(PIN_KEY_UP);
        break;
    case USER_BUTTON_DOWN:
        value = rt_pin_read(PIN_KEY_DOWN);
        break;
    case USER_BUTTON_LEFT:
        value = rt_pin_read(PIN_KEY_LEFT);
        break;
    case USER_BUTTON_RIGHT:
        value = rt_pin_read(PIN_KEY_RIGHT);
        break;
    default:
        RT_ASSERT(0);
    }

    return value;
}

static void common_btn_evt_cb(void *arg)
{
    flex_button_t *btn = (flex_button_t *)arg;

    /* simulate touching down */
    if ((flex_button_event_read(&user_button[USER_BUTTON_UP]) == FLEX_BTN_PRESS_DOWN))
    {
        down_touch();
    }
    
    /* simulate touching up */
    if ((flex_button_event_read(&user_button[USER_BUTTON_DOWN]) == FLEX_BTN_PRESS_DOWN))
    {
        up_touch();
    }
    
    /* simulate touching right */
    if ((flex_button_event_read(&user_button[USER_BUTTON_LEFT]) == FLEX_BTN_PRESS_DOWN))
    {
        right_touch();
    }
    
    /* simulate touching left */
    if ((flex_button_event_read(&user_button[USER_BUTTON_RIGHT]) == FLEX_BTN_PRESS_DOWN))
    {
        left_touch();
    }
    
    /* simulate touching in */
    if ((flex_button_event_read(&user_button[USER_BUTTON_UP]) == FLEX_BTN_PRESS_DOUBLE_CLICK)   ||  \
        (flex_button_event_read(&user_button[USER_BUTTON_DOWN]) == FLEX_BTN_PRESS_DOUBLE_CLICK) ||  \
        (flex_button_event_read(&user_button[USER_BUTTON_LEFT]) == FLEX_BTN_PRESS_DOUBLE_CLICK) ||  \
        (flex_button_event_read(&user_button[USER_BUTTON_RIGHT]) == FLEX_BTN_PRESS_DOUBLE_CLICK))
    {
        in_touch();
    }
    
    /* simulate touching left */
    if ((flex_button_event_read(&user_button[USER_BUTTON_UP]) == FLEX_BTN_PRESS_LONG_HOLD)      ||  \
        (flex_button_event_read(&user_button[USER_BUTTON_DOWN]) == FLEX_BTN_PRESS_LONG_HOLD)    ||  \
        (flex_button_event_read(&user_button[USER_BUTTON_LEFT]) == FLEX_BTN_PRESS_LONG_HOLD)    ||  \
        (flex_button_event_read(&user_button[USER_BUTTON_RIGHT]) == FLEX_BTN_PRESS_LONG_HOLD))
    {
        long_press_touch();
    }
}

static void button_scan(void *arg)
{
    while(1)
    {
        flex_button_scan();
        rt_thread_mdelay(20);
    }
}

static void user_button_init(void)
{
    int i;
    
    rt_memset(&user_button[0], 0x0, sizeof(user_button));

    rt_pin_mode(PIN_KEY_UP, PIN_MODE_INPUT_PULLUP);     /* set KEY pin mode to input */
    rt_pin_mode(PIN_KEY_DOWN, PIN_MODE_INPUT_PULLUP);   /* set KEY pin mode to input */
    rt_pin_mode(PIN_KEY_LEFT, PIN_MODE_INPUT_PULLUP);   /* set KEY pin mode to input */
    rt_pin_mode(PIN_KEY_RIGHT, PIN_MODE_INPUT_PULLUP);  /* set KEY pin mode to input */

    for (i = 0; i < USER_BUTTON_MAX; i ++)
    {
        user_button[i].id = i;
        user_button[i].usr_button_read = common_btn_read;
        user_button[i].cb = common_btn_evt_cb;
        user_button[i].pressed_logic_level = 0;
        user_button[i].short_press_start_tick = FLEX_MS_TO_SCAN_CNT(500);
        user_button[i].long_press_start_tick = FLEX_MS_TO_SCAN_CNT(1000);
        user_button[i].long_hold_start_tick = FLEX_MS_TO_SCAN_CNT(2000);

        flex_button_register(&user_button[i]);
    }
}

int flex_button_main(void)
{
    rt_thread_t tid = RT_NULL;

    user_button_init();

    tid = rt_thread_create("flex_btn", button_scan, RT_NULL, 1024, 10, 10);
    if(tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }

    return 0;
}
#ifdef FINSH_USING_MSH
INIT_APP_EXPORT(flex_button_main);
#endif
