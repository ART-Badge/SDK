/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2020-09-01     tyustli  first version
 */

#ifndef __SIMULATOR_TOUCH_H
#define __SIMULATOR_TOUCH_H

#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"

#define TEST_TOUCH_LOOP_NUM     6
#define TEST_TOUCH_POINT_RIGHT  45
#define TEST_TOUCH_POINT_LEFT   195
#define TEST_TOUCH_POINT_UP     195
#define TEST_TOUCH_POINT_DOWN   45
#define TEST_TOUCH_POINT        15
#define TEST_TOUCH_DELAY_TIME   8
#define TOUCH_DOWN              1
#define TOUCH_UP                0

void right_touch(void);
void left_touch(void);
void up_touch(void);
void down_touch(void);
void in_touch(void);
void left_long_press_touch(void);
void right_long_press_touch(void);

#endif /* __SIMULATOR_TOUCH_H */
