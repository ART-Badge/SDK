/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2020-09-01     tyustli  first version
 */

#include "rtconfig.h"
#include <simulator_touch.h>

#if defined(PKG_USING_GUIENGINE) && defined(BSP_USING_SIMULATOR_TOUCH)
#include "board.h"
#include <rtgui/driver.h>

void right_touch(void)
{
    rt_uint32_t i = 0;

    for (i = 0; i < TEST_TOUCH_LOOP_NUM; i++)//45+0 45+30 45+60 45+90 45+120 45+150 45+180
    {
        rtgui_send_touch(TEST_TOUCH_POINT_RIGHT + i * 30, TEST_TOUCH_POINT_RIGHT, TOUCH_DOWN);
        rt_thread_mdelay(TEST_TOUCH_DELAY_TIME);
    }

    rtgui_send_touch(TEST_TOUCH_POINT_LEFT + 30, TEST_TOUCH_POINT_RIGHT, TOUCH_UP);
}
#ifdef RT_USING_FINSH
    #include <finsh.h>
    MSH_CMD_EXPORT(right_touch, soft right touch move);
#endif /* RT_USING_FINSH */

void left_touch(void)
{
    rt_uint32_t i = 0;

    for (i = 0; i < TEST_TOUCH_LOOP_NUM; i++)//195-0 195-30 195-60 195-90 195-120 195-150 195-180
    {
        rtgui_send_touch(TEST_TOUCH_POINT_LEFT - i * 30, TEST_TOUCH_POINT_LEFT, TOUCH_DOWN);
        rt_thread_mdelay(TEST_TOUCH_DELAY_TIME);
    }

    rtgui_send_touch(TEST_TOUCH_POINT_RIGHT - 30, TEST_TOUCH_POINT_LEFT, TOUCH_UP);
}
#ifdef RT_USING_FINSH
    #include <finsh.h>
    MSH_CMD_EXPORT(left_touch, soft left touch move);
#endif /* RT_USING_FINSH */

void up_touch(void)
{
    rt_uint32_t i = 0;

    for (i = 0; i < TEST_TOUCH_LOOP_NUM; i++)
    {
        rtgui_send_touch(TEST_TOUCH_POINT_UP, TEST_TOUCH_POINT_UP - i * 30, TOUCH_DOWN);
        rt_thread_mdelay(TEST_TOUCH_DELAY_TIME);
    }

    rtgui_send_touch(TEST_TOUCH_POINT_RIGHT - 30, TEST_TOUCH_POINT_UP, TOUCH_UP);
}
#ifdef RT_USING_FINSH
    #include <finsh.h>
    MSH_CMD_EXPORT(up_touch, soft up touch move);
#endif /* RT_USING_FINSH */

void down_touch(void)
{
    rt_uint32_t i = 0;

    for (i = 0; i < TEST_TOUCH_LOOP_NUM; i++)
    {
        rtgui_send_touch(TEST_TOUCH_POINT_DOWN, TEST_TOUCH_POINT_DOWN + i * 30, TOUCH_DOWN);
        rt_thread_mdelay(TEST_TOUCH_DELAY_TIME);
    }

    rtgui_send_touch(TEST_TOUCH_POINT_LEFT + 30, TEST_TOUCH_POINT_DOWN, TOUCH_UP);
}
#ifdef RT_USING_FINSH
    #include <finsh.h>
    MSH_CMD_EXPORT(down_touch, soft down touch move);
#endif /* RT_USING_FINSH */

void in_touch(void)
{
    rt_uint32_t i = 0;

    for (i = 0; i < TEST_TOUCH_LOOP_NUM; i++)
    {
        rtgui_send_touch(TEST_TOUCH_POINT, TEST_TOUCH_POINT, TOUCH_DOWN);
        rt_thread_mdelay(TEST_TOUCH_DELAY_TIME);
    }

    rtgui_send_touch(TEST_TOUCH_POINT, TEST_TOUCH_POINT, TOUCH_UP);
}
#ifdef RT_USING_FINSH
    #include <finsh.h>
    MSH_CMD_EXPORT(in_touch, soft in touch move);
#endif /* RT_USING_FINSH */

void left_long_press_touch(void)
{
    rt_uint32_t i = 0;

    for (i = 0; i < TEST_TOUCH_LOOP_NUM; i++)
    {
        rtgui_send_touch(TEST_TOUCH_POINT, 120, TOUCH_DOWN);
        rt_thread_mdelay(TEST_TOUCH_DELAY_TIME);
    }

    rtgui_send_touch(TEST_TOUCH_POINT, 120, TOUCH_UP);
}
#ifdef RT_USING_FINSH
    #include <finsh.h>
    MSH_CMD_EXPORT(left_long_press_touch, soft long press touch move);
#endif /* RT_USING_FINSH */

void right_long_press_touch(void)
{
    rt_uint32_t i = 0;

    for (i = 0; i < TEST_TOUCH_LOOP_NUM; i++)
    {
        rtgui_send_touch(230, 120, TOUCH_DOWN);
        rt_thread_mdelay(TEST_TOUCH_DELAY_TIME);
    }

    rtgui_send_touch(230, 120, TOUCH_UP);
}

#endif /* PKG_USING_GUIENGINE && BSP_USING_SIMULATOR_TOUCH */
