/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-11     armink       the first version
 */

#ifndef _GUI_TEST_H_
#define _GUI_TEST_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

#define GUI_TEST_EVENT_TOUCH_ID 1
#define GUI_TEST_EVENT_KEY_ID   2
#define GUI_TEST_EVENT_PAGE_ID  3

/* replace the touch event send code */
#define rtgui_send_touch(x, y, pressed)  \
    {rtgui_send_touch(x, y, pressed); gui_test_touch_trigger(x, y, pressed);}

int gui_test_init(void);
bool gui_test_is_recording(void);
bool gui_test_is_replaing(void);
bool gui_test_is_monkey(void);
int gui_test_touch_trigger(int x, int y, int pressed);
int gui_test_key_trigger(uint32_t event_type, uint32_t key_id);

#ifdef __cplusplus
}
#endif

#endif /* _GUI_TEST_H_ */
